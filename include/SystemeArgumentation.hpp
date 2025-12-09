/* SystemeArgumentation.hpp
 * Définit la classe représentant un système d'argumentation abstrait F = <A, R> sous forme de graphe orienté. */

#ifndef SYSTEME_ARGUMENTATION_HPP
#define SYSTEME_ARGUMENTATION_HPP

#include <string>  // std::string
#include <vector>  // std::vector (tableau dynamique)
#include <unordered_map>  // std::unordered_map (table de hachage)
#include <utility>  // std::pair
#include <iostream>  // std::cout, std::ostream


class SystemeArgumentation {
private:
    // Associe chaque nom d'argument à un identifiant unique entier (pour accès en O(1))
    std::unordered_map<std::string, int> nomVersId_;
    // Tableau permettant de retrouver le nom d'un argument à partir de son identifiant entier
    std::vector<std::string> idVersNom_;
    // Graphe des attaques : adjacence_[i] contient la liste des cibles attaquées par l'argument i
    std::vector<std::vector<int>> adjacence_;
    // Graphe inverse : parents_[i] contient la liste des attaquants de l'argument i (optimisation pour la défense)
    std::vector<std::vector<int>> parents_;

public:
    SystemeArgumentation() = default;  // Constructeur par défaut
    ~SystemeArgumentation() = default;  // Destructeur par défaut

    // Ajoute un nouvel argument au système et lui assigne un identifiant unique
    // Retourne true si l'argument a été ajouté, false s'il existait déjà
    bool ajouterArgument(const std::string& arg);
    // Ajoute une relation d'attaque entre deux arguments existants
    // Retourne true si l'ajout est réussi, false si les arguments n'existent pas ou l'attaque existe déjà
    bool ajouterAttaque(const std::string& source, const std::string& cible);

    // Retourne le nombre total d'arguments
    size_t getNbArguments() const;
    // Retourne l'identifiant entier associé à un nom d'argument (lance une exception si introuvable)
    int getId(const std::string& nom) const;
    // Retourne le nom de l'argument correspondant à l'identifiant donné
    const std::string& getNom(int id) const;
    // Retourne une référence constante vers le graphe d'adjacence (pour les algorithmes)
    const std::vector<std::vector<int>>& getAdjacence() const;
    // Retourne une référence constante vers le graphe des parents (pour vérifier la défense)
    const std::vector<std::vector<int>>& getParents() const;

    // Vérifie si un argument est présent dans le système
    bool argumentExiste(const std::string& arg) const;
    // Vérifie si une attaque existe entre deux arguments donnés par leurs noms
    bool attaqueExiste(const std::string& source, const std::string& cible) const;
    // Vérifie si une attaque existe entre deux arguments donnés par leurs identifiants
    bool attaqueExiste(int idSource, int idCible) const;
    // Retourne la liste complète des noms des arguments
    const std::vector<std::string>& getArguments() const;
    // Construit et retourne la liste de toutes les attaques sous forme de paires de noms
    std::vector<std::pair<std::string, std::string>> getAttaques() const;
    // Retourne la liste des noms des arguments qui attaquent l'argument donné
    std::vector<std::string> getAttaquants(const std::string& arg) const;
    // Retourne la liste des noms des arguments attaqués par l'argument donné
    std::vector<std::string> getCibles(const std::string& arg) const;
    // Réinitialise le système en supprimant tous les arguments et attaques
    void vider();
    // Affiche le contenu du système (pour débug)
    void afficher() const;
};

#endif // SYSTEME_ARGUMENTATION_HPP
