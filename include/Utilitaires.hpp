/* Utilitaires.hpp
 * Fonctions utilitaires pour manipuler les ensembles d'arguments et vérifier des propriétés (sans-conflit, etc.) */

#ifndef UTILITAIRES_HPP
#define UTILITAIRES_HPP


#include <set>
#include <string>
#include "SystemeArgumentation.hpp"



// Utilisation : Utilitaires::estSansConflit(S, sa)
namespace Utilitaires {
    using EnsembleArguments = std::set<std::string>;

    // Vérifie si un ensemble S est sans conflit dans le système (aucun argument de S n'attaque un autre de S)
    bool estSansConflit(const EnsembleArguments& S,
                        const SystemeArgumentation& sa);

    // Vérifie si un ensemble S défend un argument
    bool defend(const EnsembleArguments& S,
                const std::string& arg,
                const SystemeArgumentation& sa);

    // Vérifie si un ensemble S est admissible (sans conflit et défend tous ses éléments)
    bool estAdmissible(const EnsembleArguments& S,
                       const SystemeArgumentation& sa);

    // Vérifie si un ensemble S attaque tous les arguments hors de S
    // utilisé pour la sémantique stable (S est stable ssi S est sans conflit et attaque tout A\S)
    bool attaqueTousLesAutres(const EnsembleArguments& S,
                              const SystemeArgumentation& sa);


    // Différence ensembliste (A\B)
    EnsembleArguments difference(const EnsembleArguments& A,
                                 const EnsembleArguments& B);

    // Union ensembliste (A ∪ B)
    EnsembleArguments unionEnsembles(const EnsembleArguments& A,
                                     const EnsembleArguments& B);

    // Intersection ensembliste (A ∩ B)
    EnsembleArguments intersection(const EnsembleArguments& A,
                                   const EnsembleArguments& B);

    // Inclusion ensembliste (A ⊆ B)
    bool estSousEnsemble(const EnsembleArguments& A,
                         const EnsembleArguments& B);

    
    // Retourne l'ensemble des arguments défendus par S
    EnsembleArguments fonctionCaracteristique(const EnsembleArguments& S,
                                               const SystemeArgumentation& sa);


    // Convertit un ensemble en string pour affichage : "{a, b, c}"
    std::string ensembleVersString(const EnsembleArguments& S);

}

#endif // UTILITAIRES_HPP
