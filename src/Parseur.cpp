/* Parseur.cpp
 * Implémentation du parseur de fichiers .apx, lit et valide les fichiers d'argumentation
 * Format supporté :
 *   arg(nom_argument).
 *   att(source,cible). */

#include "Parseur.hpp"
#include <fstream>      // pour std::ifstream (lecture de fichiers)
#include <sstream>      // pour std::stringstream (parsing de strings)
#include <algorithm>    // pour std::remove_if, std::all_of



SystemeArgumentation Parseur::parserFichier(const std::string& cheminFichier) {

    std::ifstream fichier(cheminFichier);  // flux d'entrée pour lire des fichiers (if = input file)

    if (!fichier.is_open()) {  // vérifier si l'ouverture a réussi
        throw ErreurParsing("Impossible d'ouvrir le fichier : " + cheminFichier);
    }

    SystemeArgumentation systeme;  // le système d'argumentation qu'on va construire
    std::string ligne; // pour stocker chaque ligne lue
    size_t numeroLigne = 0;  // compteur de ligne (pour messages d'erreur)

    while (std::getline(fichier, ligne)) { // tant qu'on peut lire une ligne
        ++numeroLigne;
        ligne = trim(ligne);  // supprimer les espaces blancs en début et fin

        if (ligne.empty() || ligne[0] == '#') {  // ignorer les lignes vides ou commentaires
            continue;  // passer à la ligne suivante
        }

        try {
            if (ligne.substr(0, 4) == "arg(") {  // vérifier si la ligne commence par "arg("
                std::string nomArg = parserLigneArgument(ligne);
                bool ajoutReussi = systeme.ajouterArgument(nomArg);  // ajouter l'argument au système
                // Si l'argument existait déjà, on ignore
                (void)ajoutReussi;  // évite un warning "variable non utilisée"

            }
            else if (ligne.substr(0, 4) == "att(") {  // vérifier si la ligne commence par "att("
                auto [source, cible] = parserLigneAttaque(ligne);
                bool ajoutReussi = systeme.ajouterAttaque(source, cible);  // ajouter l'attaque au système
                if (!ajoutReussi) {  // si l'ajout échoue, c'est qu'un argument n'existe pas
                    throw ErreurParsing(
                        "Ligne " + std::to_string(numeroLigne) +
                        " : Attaque invalide (arguments inexistants) : " + ligne
                    );
                }

            } else {
                throw ErreurParsing(
                    "Ligne " + std::to_string(numeroLigne) +
                    " : Format invalide (doit commencer par 'arg(' ou 'att(') : " + ligne
                );
            }

        } catch (const ErreurParsing& e) {
            fichier.close();  // fermer le fichier en cas d'erreur
            throw ErreurParsing(  // relancer l'exception avec contexte supplémentaire
                "Erreur de parsing dans '" + cheminFichier +
                "' ligne " + std::to_string(numeroLigne) + " : " + e.what()
            );
        }
    }

    fichier.close();

    if (systeme.getNombreArguments() == 0) {  // vérifier qu'on a au moins un argument
        throw ErreurParsing(
            "Le fichier '" + cheminFichier +
            "' ne contient aucun argument valide"
        );
    }
    return systeme;
}


// Parse une ligne de type "arg(nom)."
std::string Parseur::parserLigneArgument(const std::string& ligne) {
    if (ligne.length() < 7) {  // minimum : "arg(a)." = 7 caractères
        throw ErreurParsing("Ligne trop courte pour un argument : " + ligne);
    }
    if (ligne.substr(0, 4) != "arg(") {  // vérifier que ça commence bien par "arg("
        throw ErreurParsing("L'argument doit commencer par 'arg(' : " + ligne);
    }
    if (ligne.substr(ligne.length() - 2) != ").") {  // Vérifier que ça se termine par ")."
        throw ErreurParsing("L'argument doit se terminer par ').' : " + ligne);
    }

    // Extrait la sous-chaîne entre "arg(" et ")." : 4 (après "arg(")
    size_t debut = 4;  // Position après "arg("
    size_t longueur = ligne.length() - 6;  // Tout sauf "arg(" et ")."
    std::string nom = ligne.substr(debut, longueur);

    if (nom.empty()) {
        throw ErreurParsing("Nom d'argument vide : " + ligne);
    }
    if (!estNomValide(nom)) {
        throw ErreurParsing("Nom d'argument invalide : " + nom);
    }

    return nom;
}

// Parse une ligne de type "att(source,cible)."
std::pair<std::string, std::string> Parseur::parserLigneAttaque(const std::string& ligne) {
    // Même logique que pour parserLigneArgument() ci-dessus
    if (ligne.length() < 9) {
        throw ErreurParsing("Ligne trop courte pour une attaque : " + ligne);
    }
    if (ligne.substr(0, 4) != "att(") {
        throw ErreurParsing("L'attaque doit commencer par 'att(' : " + ligne);
    }
    if (ligne.substr(ligne.length() - 2) != ").") {
        throw ErreurParsing("L'attaque doit se terminer par ').' : " + ligne);
    }

    size_t debut = 4;
    size_t longueur = ligne.length() - 6;
    std::string contenu = ligne.substr(debut, longueur);

    size_t posVirgule = contenu.find(',');  // retourne la position de la première occurrence

    if (posVirgule == std::string::npos) {
        throw ErreurParsing(
            "Virgule manquante dans l'attaque (format: att(source,cible).) : " + ligne
        );
    }

    std::string source = contenu.substr(0, posVirgule);
    std::string cible = contenu.substr(posVirgule + 1);

    // Supprimer les espaces éventuels
    source = trim(source);
    cible = trim(cible);

    if (source.empty()) {
        throw ErreurParsing("Source de l'attaque vide : " + ligne);
    }
    if (cible.empty()) {
        throw ErreurParsing("Cible de l'attaque vide : " + ligne);
    }
    if (!estNomValide(source)) {
        throw ErreurParsing("Nom de source invalide : " + source);
    }
    if (!estNomValide(cible)) {
        throw ErreurParsing("Nom de cible invalide : " + cible);
    }
    /*
    if (source == cible) {  // ne pas permettre les auto-attaques (optionnel)
        throw ErreurParsing("Auto-attaque non permise : " + ligne);
    } */

    return {source, cible};
}


// Vérifie si un nom d'argument est valide
bool Parseur::estNomValide(const std::string& nom) {
    // Un nom valide doit être : non vide, contenant uniquement lettres, chiffres, underscore,
    // pas un mot réservé (arg, att)

    if (nom.empty()) {
        return false;
    }
    if (nom == "arg" || nom == "att") {
        return false;
    }

    bool tousCaracteresValides = std::all_of(  // retourne true si le prédicat est vrai pour tous les éléments
        nom.begin(),
        nom.end(),
        [](char c) {  // fonction anonyme prenant un char et retourne un bool
            return std::isalnum(static_cast<unsigned char>(c)) || c == '_';  // cast évite un warning si char est signé
        }
    );

    return tousCaracteresValides;
}

// Supprime les espaces en début et fin de chaîne (trim)
std::string Parseur::trim(const std::string& str) {
    if (str.empty()) {
        return str;
    }
    auto debut = std::find_if(  // retourne un itérateur vers le premier élément satisfaisant
        str.begin(),
        str.end(),
        [](unsigned char c) { return !std::isspace(c); }  // cherche le premier caractère qui n'est pas un espace
    );
    if (debut == str.end()) {  // si toute la chaîne est composée d'espaces
        return "";
    }

    auto fin = std::find_if(  // parcourt à l'envers
        str.rbegin(),  // reverse begin (commence à la fin)
        str.rend(),  // reverse end (s'arrête au début)
        [](unsigned char c) { return !std::isspace(c); }
    ).base();  // convertit un reverse_iterator en iterator normal

    return std::string(debut, fin);  // crée une nouvelle chaîne contenant (debut, fin)
}
