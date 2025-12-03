/* Fichier : Solveur.hpp
 * Description : Résout les 6 problèmes d'argumentation demandés :
 *               VE-PR, DC-PR, DS-PR, VE-ST, DC-ST, DS-ST
 * Notation :
 *   - VE = Verify Extension (vérifier si S est une extension)
 *   - DC = Decide Credulous (acceptabilité crédule : ∃ extension contenant a)
 *   - DS = Decide Skeptical (acceptabilité sceptique : ∀ extensions contiennent a)
 *   - PR = Preferred (sémantique préférée)
 *   - ST = Stable (sémantique stable) */

#ifndef SOLVEUR_HPP
#define SOLVEUR_HPP


#include <set>
#include <string>

#include "SystemeArgumentation.hpp"



// Les enum class (C++11) sont des énumérations fortement typées.
// Avantages sur les enum classiques :
//   - Pas de pollution de l'espace de noms
//   - Pas de conversion implicite vers int
//   - Doit être préfixé : TypeProbleme::VE_PR
enum class TypeProbleme {
    VE_PR,  // Verify Extension - Preferred
    DC_PR,  // Decide Credulous - Preferred
    DS_PR,  // Decide Skeptical - Preferred
    VE_ST,  // Verify Extension - Stable
    DC_ST,  // Decide Credulous - Stable
    DS_ST   // Decide Skeptical - Stable
};


class Solveur {

public:
    // Alias pour la lisibilité
    using EnsembleArguments = std::set<std::string>;

    // Construit un solveur pour un système d'argumentation donné
    // Le système est passé par référence constante et stocké comme référence
    // Cela évite de copier tout le système
    explicit Solveur(const SystemeArgumentation& sa);


    // VE-PR : Vérifie si S est une extension préférée
    // Retourne true si oui, false sinon
    bool verifierExtensionPreferee(const EnsembleArguments& S) const;

    // VE-ST : Vérifie si S est une extension stable
    // Retourne true si oui, false sinon
    bool verifierExtensionStable(const EnsembleArguments& S) const;

    // DC-PR : Acceptabilité crédule pour la sémantique préférée
    // L'argument est-il dans AU MOINS UNE extension préférée ?
    bool acceptationCredulePreferee(const std::string& arg) const;

    // DS-PR : Acceptabilité sceptique pour la sémantique préférée
    // L'argument est-il dans TOUTES les extensions préférées ?
    bool acceptationSceptiquePreferee(const std::string& arg) const;

    // DC-ST : Acceptabilité crédule pour la sémantique stable
    // L'argument est-il dans AU MOINS UNE extension stable ?
    bool acceptationCreduleStable(const std::string& arg) const;

    // DS-ST : Acceptabilité sceptique pour la sémantique stable
    // L'argument est-il dans TOUTES les extensions stables ?
    // Attention : si aucune extension stable, retourne true par convention
    // (vacuously true : ∀x ∈ ∅, P(x) est vrai)
    bool acceptationSceptiqueStable(const std::string& arg) const;


    // Résout un problème donné (dispatch vers la bonne méthode)
    // - Pour VE : ensembleArgs contient l'ensemble à vérifier
    // - Pour DC/DS : argument contient l'argument à tester
    bool resoudre(TypeProbleme probleme,
                  const EnsembleArguments& ensembleArgs,
                  const std::string& argument) const;


private:
    // Référence constante vers le système d'argumentation
    // "const" : on ne modifie pas le système
    // "&" : c'est une référence, pas une copie
    // Une référence DOIT être initialisée et ne peut pas changer
    // Approprié ici car le solveur est lié à UN système
    const SystemeArgumentation& systeme_;

};

#endif // SOLVEUR_HPP
