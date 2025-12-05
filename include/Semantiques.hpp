/* Semantiques.hpp
 * Calcul des extensions préférées et stables */

#ifndef SEMANTIQUES_HPP
#define SEMANTIQUES_HPP


#include <vector>
#include <set>
#include <string>
#include "SystemeArgumentation.hpp"



class Semantiques {
public:
    using EnsembleArguments = std::set<std::string>;
    using ListeExtensions = std::vector<EnsembleArguments>;
    
    // Calcule toutes les extensions préférées du système
    // retourne : vecteur de toutes les extensions préférées
    static ListeExtensions calculerExtensionsPreferees(
        const SystemeArgumentation& sa);
    
    // Calcule toutes les extensions stables du système
    // retourne : vecteur des extensions stables (peut être vide, par exemple pour les cycles impairs)
    static ListeExtensions calculerExtensionsStables(
        const SystemeArgumentation& sa);


    // Vérifie si S est une extension préférée
    static bool estExtensionPreferee(const EnsembleArguments& S,
                                     const SystemeArgumentation& sa);
    
    // Vérifie si S est une extension stable
    static bool estExtensionStable(const EnsembleArguments& S,
                                   const SystemeArgumentation& sa);

private:
    // Calcule une extension complète à partir d'un ensemble admissible en appliquant la fonction caractéristique
    static EnsembleArguments calculerExtensionComplete(
        const EnsembleArguments& base,
        const SystemeArgumentation& sa);

    // Génère tous les sous-ensembles d'un ensemble (complexité O(2^n))
    static std::vector<EnsembleArguments> genererSousEnsembles(
        const EnsembleArguments& ensemble);

    // Vérifie si une extension est maximale parmi une liste d'extensions
    static bool estMaximale(const EnsembleArguments& S,
                            const ListeExtensions& extensions);

};

#endif // SEMANTIQUES_HPP
