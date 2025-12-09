/* SystemeArgumentation.cpp
 * Implémentation de la classe gérant le graphe d'argumentation. */

#include "SystemeArgumentation.hpp"
#include <algorithm>  // pour std::find
#include <stdexcept>  // pour std::runtime_error


// Ajoute un nouvel argument s'il n'existe pas déjà
bool SystemeArgumentation::ajouterArgument(const std::string& arg) {
    if (nomVersId_.find(arg) != nomVersId_.end()) {  // Vérification de l'existence
        return false;
    }
    int id = static_cast<int>(idVersNom_.size());   // ID correspond à l'index dans le vecteur
    nomVersId_[arg] = id;  // Enregistrement dans la table de hachage
    idVersNom_.push_back(arg);  // Enregistrement dans le vecteur de noms

    adjacence_.resize(id + 1); // Agrandit le vecteur d'adjacence pour le nouveau nœud
    parents_.resize(id + 1);   // Agrandit le vecteur des parents de même

    return true;
}

// Ajoute une attaque entre deux arguments existants
bool SystemeArgumentation::ajouterAttaque(const std::string& source, const std::string& cible) {
    // Vérification de l'existence des arguments
    if (nomVersId_.find(source) == nomVersId_.end() || nomVersId_.find(cible) == nomVersId_.end()) {
        return false;
    }
    int u = nomVersId_[source]; // Récupération ID source
    int v = nomVersId_[cible];  // Récupération ID cible

    for (int cibleExistante : adjacence_[u]) {  // Vérification de doublon d'attaque
        if (cibleExistante == v) return false;
    }

    adjacence_[u].push_back(v); // Ajout de l'arc u -> v
    parents_[v].push_back(u);   // Ajout de l'arc inverse v <- u

    return true;
}

// Retourne le nombre total d'arguments
size_t SystemeArgumentation::getNbArguments() const {
    return idVersNom_.size();
}

// Récupère l'identifiant associé à un nom d'argument
int SystemeArgumentation::getId(const std::string& nom) const {
    auto it = nomVersId_.find(nom);
    if (it == nomVersId_.end()) {
        throw std::runtime_error("Argument introuvable : " + nom);
    }
    return it->second;
}

// Récupère le nom associé à un identifiant
const std::string& SystemeArgumentation::getNom(int id) const {
    return idVersNom_.at(id);
}

// Getter pour le graphe d'adjacence
const std::vector<std::vector<int>>& SystemeArgumentation::getAdjacence() const {
    return adjacence_;
}

// Getter pour le graphe des parents
const std::vector<std::vector<int>>& SystemeArgumentation::getParents() const {
    return parents_;
}

// Vérifie si un argument existe via la map
bool SystemeArgumentation::argumentExiste(const std::string& arg) const {
    return nomVersId_.find(arg) != nomVersId_.end();
}

// Vérifie l'existence d'une attaque via les noms
bool SystemeArgumentation::attaqueExiste(const std::string& source, const std::string& cible) const {
    if (!argumentExiste(source) || !argumentExiste(cible)) return false;
    return attaqueExiste(nomVersId_.at(source), nomVersId_.at(cible));  // Renvoie à la version optimisée
}

// Vérifie l'existence d'une attaque via les identifiants
bool SystemeArgumentation::attaqueExiste(int idSource, int idCible) const {
    const auto& cibles = adjacence_[idSource];
    // Recherche linéaire dans les voisins
    return std::find(cibles.begin(), cibles.end(), idCible) != cibles.end();
}

// Retourne la liste des noms d'arguments
const std::vector<std::string>& SystemeArgumentation::getArguments() const {
    return idVersNom_;
}

// Reconstruit la liste de toutes les attaques (paires de noms)
std::vector<std::pair<std::string, std::string>> SystemeArgumentation::getAttaques() const {
    std::vector<std::pair<std::string, std::string>> liste;
    // Parcours complet du graphe d'adjacence
    for (size_t u = 0; u < adjacence_.size(); ++u) {
        for (int v : adjacence_[u]) {
            liste.emplace_back(idVersNom_[u], idVersNom_[v]);  // Construction de la paire
        }
    }
    return liste;
}

// Retourne les noms des attaquants d'un argument
std::vector<std::string> SystemeArgumentation::getAttaquants(const std::string& arg) const {
    std::vector<std::string> result;
    if (!argumentExiste(arg)) return result;

    int id = nomVersId_.at(arg);
    // Utilisation du graphe inverse parents_ pour éviter de parcourir tout le graphe
    for (int attaquantId : parents_[id]) {
        result.push_back(idVersNom_[attaquantId]);
    }
    return result;
}

// Retourne les noms des cibles d'un argument
std::vector<std::string> SystemeArgumentation::getCibles(const std::string& arg) const {
    std::vector<std::string> result;
    if (!argumentExiste(arg)) return result;

    int id = nomVersId_.at(arg);
    for (int cibleId : adjacence_[id]) {
        result.push_back(idVersNom_[cibleId]);
    }
    return result;
}

// Vide toutes les structures de données.
void SystemeArgumentation::vider() {
    nomVersId_.clear();
    idVersNom_.clear();
    adjacence_.clear();
    parents_.clear();
}

// Affiche le système (pour débug)
void SystemeArgumentation::afficher() const {
    std::cout << "Système d'Argumentation : " << std::endl;
    std::cout << getNbArguments() << " Arguments : " << std::endl;
    for (size_t i = 0; i < idVersNom_.size(); ++i) {
        std::cout << "  - " << idVersNom_[i] << " (ID: " << i << ")" << std::endl;
    }
    std::cout << "\nAttaques :" << std::endl;
    for (size_t i = 0; i < adjacence_.size(); ++i) {
        for (int cible : adjacence_[i]) {
            std::cout << "  - " << idVersNom_[i] << " -> " << idVersNom_[cible] << std::endl;
        }
    }
}
