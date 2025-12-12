/* Utilitaires.hpp
 * Fonctions utilitaires pour manipuler les ensembles d'arguments et vérifier des propriétés */

#ifndef UTILITAIRES_HPP
#define UTILITAIRES_HPP

#include <vector>  // std::vector
#include <string>  // std::string
#include "SystemeArgumentation.hpp"


namespace Utilitaires {
    // Alias pour manipuler des ensembles d'arguments sous forme d'identifiants entiers
    using EnsembleIds = std::vector<int>;

    // Convertit une liste de noms en une liste d'identifiants triée, en ignorant les noms inexistants
    EnsembleIds convertirNomsEnIds(const std::vector<std::string>& noms, const SystemeArgumentation& sa);
    // Convertit une liste d'identifiants en une liste de noms pour l'affichage
    std::vector<std::string> convertirIdsEnNoms(const EnsembleIds& ids, const SystemeArgumentation& sa);

    // Vérifie si un ensemble d'arguments est sans conflit
    bool estSansConflit(const EnsembleIds& S, const SystemeArgumentation& sa);
    // Vérifie si un ensemble S défend un argument spécifique contre tous ses attaquants
    bool defend(const EnsembleIds& S, int cibleId, const SystemeArgumentation& sa);
    // Vérifie si un ensemble est admissible
    bool estAdmissible(const EnsembleIds& S, const SystemeArgumentation& sa);
    // Vérifie si un ensemble attaque tous les arguments qui ne lui appartiennent pas
    bool attaqueToutExterieur(const EnsembleIds& S, const SystemeArgumentation& sa);

    // Fonction caractéristique F(S) : retourne l'ensemble de tous les arguments défendus par S
    EnsembleIds fonctionCaracteristique(const EnsembleIds& S, const SystemeArgumentation& sa);

    // Génère une représentation textuelle d'un ensemble d'identifiants (exemple : "{a, b, c}")
    std::string afficher(const EnsembleIds& S, const SystemeArgumentation& sa);
}

#endif // UTILITAIRES_HPP
