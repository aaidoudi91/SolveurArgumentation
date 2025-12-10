/* Solveur.cpp
* Implémentation de l'interface faisant le pont entre les requêtes utilisateur et les algorithmes de sémantiques. */

#include "Solveur.hpp"
#include "Semantiques.hpp"
#include "Utilitaires.hpp"


// Initialise le solveur avec une référence au système chargé
Solveur::Solveur(const SystemeArgumentation& sa) : systeme_(sa) {}

// Vérifie si l'ensemble d'arguments est une extension préférée
bool Solveur::verifierExtensionPreferee(const std::vector<std::string>& ensembleNoms) const {
    // Conversion des noms en IDs internes
    Utilitaires::EnsembleIds ids = Utilitaires::convertirNomsEnIds(ensembleNoms, systeme_);
    // Si la taille diffère, c'est qu'un argument demandé n'existe pas dans le graphe.
    // Un ensemble contenant un argument inconnu ne peut pas être une extension valide.
    if (ids.size() != ensembleNoms.size()) return false;
    return Semantiques::verifierPreferee(ids, systeme_);  // Appel du moteur
}
// Vérifie si l'ensemble est une extension stable
bool Solveur::verifierExtensionStable(const std::vector<std::string>& ensembleNoms) const {
    Utilitaires::EnsembleIds ids = Utilitaires::convertirNomsEnIds(ensembleNoms, systeme_);
    // Même vérification d'intégrité
    if (ids.size() != ensembleNoms.size()) return false;
    return Semantiques::verifierStable(ids, systeme_);  // Appel du moteur
}

    // Détermine si l'argument donné appartient à au moins une extension préférée
bool Solveur::acceptationCredulePreferee(const std::string& arg) const {
    // L'argument doit exister dans le graphe
    if (!systeme_.argumentExiste(arg)) return false;
    // Récupération de l'identifiant et appel du moteur
    int id = systeme_.getId(arg);
    return Semantiques::credulousPreferred(id, systeme_);
}
// Détermine si l'argument donné appartient à au moins une extension stable
bool Solveur::acceptationCreduleStable(const std::string& arg) const {
    // L'argument doit exister dans le graphe
    if (!systeme_.argumentExiste(arg)) return false;
    // Récupération de l'identifiant et appel du moteur
    int id = systeme_.getId(arg);
    return Semantiques::credulousStable(id, systeme_);
}

// Détermine si l'argument donné appartient à toutes les extensions préférées
bool Solveur::acceptationSceptiquePreferee(const std::string& arg) const {
    // L'argument doit exister dans le graphe
    if (!systeme_.argumentExiste(arg)) return false;
    // Récupération de l'identifiant et appel du moteur
    int id = systeme_.getId(arg);
    return Semantiques::skepticalPreferred(id, systeme_);
}
// Détermine si l'argument donné appartient à toutes les extensions stables
bool Solveur::acceptationSceptiqueStable(const std::string& arg) const {
    // L'argument doit exister dans le graphe
    if (!systeme_.argumentExiste(arg)) return false;
    // Récupération de l'identifiant et appel du moteur
    int id = systeme_.getId(arg);
    return Semantiques::skepticalStable(id, systeme_);
}

// Appelle la bonne méthode selon le TypeProbleme
bool Solveur::resoudre(TypeProbleme probleme, const std::vector<std::string>& ensembleArgs,
    const std::string& argument) const {

    switch (probleme) {
        // Problèmes de vérification (on utilise le vecteur d'arguments)
        case TypeProbleme::VE_PR:
            return verifierExtensionPreferee(ensembleArgs);
        case TypeProbleme::VE_ST:
            return verifierExtensionStable(ensembleArgs);

        // Problèmes de décision (on utilise l'argument unique)
        case TypeProbleme::DC_PR:
            return acceptationCredulePreferee(argument);
        case TypeProbleme::DS_PR:
            return acceptationSceptiquePreferee(argument);
        case TypeProbleme::DC_ST:
            return acceptationCreduleStable(argument);
        case TypeProbleme::DS_ST:
            return acceptationSceptiqueStable(argument);

        default:
            return false;
    }
}
