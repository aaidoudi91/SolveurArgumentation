/* Parseur.hpp
 * Module responsable de l'analyse syntaxique des fichiers .apx pour construire le système d'argumentation. */

#include "Parseur.hpp"
#include <fstream>  // std::ifstream
#include <sstream>  // std::stringstream
#include <algorithm>  // std::remove_if, std::all_of


// Fonction principale : lecture ligne par ligne et du remplissage du système
SystemeArgumentation Parseur::parserFichier(const std::string& cheminFichier) {
    std::ifstream fichier(cheminFichier); // Ouverture du flux en lecture
    if (!fichier.is_open()) {
        throw ErreurParsing("Impossible d'ouvrir le fichier : " + cheminFichier);
    }

    SystemeArgumentation systeme;  // Le système d'argumentation qu'on va construire
    std::string ligne;  // Pour stocker chaque ligne lue
    size_t numeroLigne = 0;  // Pour indiquer l'emplacement exact de l'erreur

    // Lecture séquentielle du fichier
    while (std::getline(fichier, ligne)) {
        ++numeroLigne;
        ligne = trim(ligne);  // Supprimer les espaces blancs en début et fin

        if (ligne.empty() || ligne[0] == '#') {  // Ignorer les lignes vides ou commentaires
            continue;
        }

        try {
            if (ligne.substr(0, 4) == "arg(") {  // Vérifier si la ligne commence par arg(
                std::string nomArg = parserLigneArgument(ligne);
                (void)systeme.ajouterArgument(nomArg);  // Ajouter l'argument au système (retour booléen est ignoré)

            }
            else if (ligne.substr(0, 4) == "att(") {  // Vérifier si la ligne commence par att(
                auto [source, cible] = parserLigneAttaque(ligne);
                bool ajoutReussi = systeme.ajouterAttaque(source, cible);  // Ajouter l'attaque au système
                if (!ajoutReussi) {  // Si l'ajout échoue, c'est qu'un argument n'existe pas
                    throw ErreurParsing("Ligne " + std::to_string(numeroLigne) +
                        " : Attaque invalide (arguments inexistants) : " + ligne);
                }
            }
            else {
                throw ErreurParsing("Ligne " + std::to_string(numeroLigne) +
                    " : Format invalide (doit commencer par 'arg(' ou 'att(') : " + ligne);
            }
        }
        catch (const ErreurParsing& e) {
            fichier.close();  // Fermer le fichier en cas d'erreur
            throw ErreurParsing("Erreur de parsing dans '" + cheminFichier +
                "' ligne " + std::to_string(numeroLigne) + " : " + e.what());
        }
    }

    fichier.close();

    if (systeme.getNbArguments() == 0) {  // Vérifier qu'on a au moins un argument
        throw ErreurParsing("Le fichier '" + cheminFichier +"' ne contient aucun argument valide");
    }
    return systeme;
}

// Analyse une ligne déclarant un argument via arg(x). et extrait le nom x
std::string Parseur::parserLigneArgument(const std::string& ligne) {
    // Vérifications sur la structure attendue
    if (ligne.length() < 7) throw ErreurParsing("Ligne trop courte : " + ligne);
    if (ligne.substr(0, 4) != "arg(") throw ErreurParsing("Doit commencer par 'arg('");
    if (ligne.substr(ligne.length() - 2) != ").") throw ErreurParsing("Doit finir par ').'");

    std::string nom = ligne.substr(4, ligne.length() - 6);  // Extrait la sous-chaîne entre parenthèses

    if (nom.empty()) throw ErreurParsing("Nom d'argument vide : " + ligne);
    if (!estNomValide(nom)) throw ErreurParsing("Nom d'argument invalide : " + nom);

    return nom;
}

// Analyse une ligne déclarant une attaque att(x,y). et extrait la paire {x, y}
std::pair<std::string, std::string> Parseur::parserLigneAttaque(const std::string& ligne) {
    // Même logique que ci-dessus
    if (ligne.length() < 9) throw ErreurParsing("Ligne trop courte : " + ligne);
    if (ligne.substr(0, 4) != "att(") throw ErreurParsing("Doit commencer par 'att('");
    if (ligne.substr(ligne.length() - 2) != ").") throw ErreurParsing("Doit finir par ').'");

    std::string contenu = ligne.substr(4, ligne.length() - 6);  // Extrait la sous-chaîne entre parenthèses

    size_t posVirgule = contenu.find(',');  // Retourne la position de la première occurrence
    if (posVirgule == std::string::npos) throw ErreurParsing("Virgule manquante : " + ligne);

    std::string source = trim(contenu.substr(0, posVirgule));  // Supprimer les espaces éventuels
    std::string cible = trim(contenu.substr(posVirgule + 1));

    if (source.empty() || cible.empty()) throw ErreurParsing("Source ou cible vide");
    if (!estNomValide(source) || !estNomValide(cible)) throw ErreurParsing("Noms invalides dans l'attaque");

    return {source, cible};
}

// Vérifie si un nom d'argument est valide (lettres, chiffres, underscore, pas arg ni att)
bool Parseur::estNomValide(const std::string& nom) {
    if (nom.empty()) return false;
    if (nom == "arg" || nom == "att") return false;  // Mots réservés

    // std::all_of retourne true si le prédicat (lambda) est vrai pour tout caractère
    return std::all_of(nom.begin(), nom.end(), [](char c) {
        return std::isalnum(static_cast<unsigned char>(c)) || c == '_';
    });
}

// Supprime les espaces en début et fin de chaîne
std::string Parseur::trim(const std::string& str) {
    if (str.empty()) return str;

    // Recherche du premier caractère depuis le début
    auto debut = std::find_if(str.begin(), str.end(), [](unsigned char c) {
        return !std::isspace(c);
    });

    if (debut == str.end()) return ""; // Chaîne vide ou uniquement des espaces

    // Recherche du premier caractère depuis la fin (reverse iterator)
    auto fin = std::find_if(str.rbegin(), str.rend(), [](unsigned char c) {
        return !std::isspace(c);
    }).base(); // .base() reconvertit l'itérateur inverse en itérateur normal pour le constructeur de string

    return {debut, fin};
}
