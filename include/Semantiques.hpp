/* Fichier : Semantiques.hpp
 * Description : Calcul des extensions selon les sémantiques de Dung
 *               (préférée et stable)
 * Sémantiques implémentées :
 *   - Préférée (preferred) : extensions admissibles maximales
 *   - Stable : ensembles sans conflit qui attaquent tout le reste */

#ifndef SEMANTIQUES_HPP
#define SEMANTIQUES_HPP


#include <vector>
#include <set>
#include <string>

#include "SystemeArgumentation.hpp"
#include "Utilitaires.hpp"



class Semantiques {

public:
    // Alias pour la lisibilité
    using EnsembleArguments = std::set<std::string>;
    using ListeExtensions = std::vector<EnsembleArguments>;


    // Calcule toutes les extensions préférées du système
    // Algorithme : Utilise la fonction caractéristique de Dung
    //   1. Partir des ensembles admissibles
    //   2. Étendre itérativement avec F jusqu'aux points fixes
    //   3. Garder les maximaux
    // Retourne : vecteur de toutes les extensions préférées
    // Complexité : Exponentielle dans le pire cas (inévitable)
    static ListeExtensions calculerExtensionsPreferees(
        const SystemeArgumentation& sa);


    // Calcule TOUTES les extensions stables du système
    // Algorithme :
    //   1. Énumérer les candidats (ensembles sans conflit)
    //   2. Vérifier que chaque candidat attaque tous les autres
    // Retourne : vecteur des extensions stables (peut être vide)
    // Note : Un système peut n'avoir aucune extension stable (cycles impairs)
    static ListeExtensions calculerExtensionsStables(
        const SystemeArgumentation& sa);


    // Vérifie si S est une extension préférée
    // Conditions :
    //   1. S est admissible
    //   2. S est maximal (aucun sur-ensemble admissible)
    static bool estExtensionPreferee(const EnsembleArguments& S,
                                     const SystemeArgumentation& sa);


    // Conditions :
    //   1. S est sans conflit
    //   2. S attaque tout argument hors de S
    static bool estExtensionStable(const EnsembleArguments& S,
                                   const SystemeArgumentation& sa);


private:
    // Calcule une extension complète à partir d'un ensemble admissible
    // en appliquant itérativement la fonction caractéristique
    static EnsembleArguments calculerExtensionComplete(
        const EnsembleArguments& base,
        const SystemeArgumentation& sa);

    // Génère tous les sous-ensembles d'un ensemble
    // Complexité O(2^n)
    static std::vector<EnsembleArguments> genererSousEnsembles(
        const EnsembleArguments& ensemble);

    // Vérifie si une extension est maximale parmi une liste d'extensions
    static bool estMaximale(const EnsembleArguments& S,
                            const ListeExtensions& extensions);

};

#endif // SEMANTIQUES_HPP
