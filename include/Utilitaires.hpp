/* Fichier : Utilitaires.hpp
 * Description : Fonctions utilitaires pour manipuler les ensembles d'arguments
 *               et vérifier les propriétés fondamentales (sans-conflit, etc.) */

#ifndef UTILITAIRES_HPP
#define UTILITAIRES_HPP


#include <set>
#include <string>

#include "SystemeArgumentation.hpp"


// Utilisation : Utilitaires::estSansConflit(S, sa)
namespace Utilitaires {

    // "using" crée un alias pour un type complexe (plus lisible)
    using EnsembleArguments = std::set<std::string>;

    // Vérifie si un ensemble S est SANS CONFLIT dans le système sa
    // Définition : S est sans conflit ssi ∄ a,b ∈ S tels que (a,b) ∈ R
    // En clair : aucun argument de S n'attaque un autre argument de S
    // Complexité : O(|S|² × log|R|) dans le pire cas
    bool estSansConflit(const EnsembleArguments& S,
                        const SystemeArgumentation& sa);

    // Vérifie si un ensemble S DÉFEND un argument 'arg'
    // Définition : S défend a ssi ∀b ∈ Att(a), ∃c ∈ S tel que (c,b) ∈ R
    bool defend(const EnsembleArguments& S,
                const std::string& arg,
                const SystemeArgumentation& sa);

    // Vérifie si un ensemble S est ADMISSIBLE
    // Définition : S est admissible ssi S est sans conflit et défend tous ses éléments
    bool estAdmissible(const EnsembleArguments& S,
                       const SystemeArgumentation& sa);

    // Vérifie si un ensemble S ATTAQUE TOUS les arguments hors de S
    // Utilisé pour la sémantique stable :
    // S est stable ssi S est sans conflit ET attaque tout A\S
    bool attaqueTousLesAutres(const EnsembleArguments& S,
                              const SystemeArgumentation& sa);


    // Différence ensembliste : A \ B (éléments dans A mais pas dans B)
    EnsembleArguments difference(const EnsembleArguments& A,
                                 const EnsembleArguments& B);

    // Union ensembliste : A ∪ B
    EnsembleArguments unionEnsembles(const EnsembleArguments& A,
                                     const EnsembleArguments& B);

    // Intersection ensembliste : A ∩ B
    EnsembleArguments intersection(const EnsembleArguments& A,
                                   const EnsembleArguments& B);

    // Vérifie si A ⊆ B (A est sous-ensemble de B)
    bool estSousEnsemble(const EnsembleArguments& A,
                         const EnsembleArguments& B);


    // F(S) = {a ∈ A | S défend a}
    // Retourne l'ensemble des arguments défendus par S
    EnsembleArguments fonctionCaracteristique(const EnsembleArguments& S,
                                               const SystemeArgumentation& sa);


    // Convertit un ensemble en string pour affichage : "{a, b, c}"
    std::string ensembleVersString(const EnsembleArguments& S);

}

#endif // UTILITAIRES_HPP
