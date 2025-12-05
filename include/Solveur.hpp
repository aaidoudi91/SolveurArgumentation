/* Solveur.hpp
 * Résout les 6 problèmes d'argumentation demandés : VE-PR, DC-PR, DS-PR, VE-ST, DC-ST, DS-ST
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
    using EnsembleArguments = std::set<std::string>;

    // Construit un solveur pour un système d'argumentation donné
    // le système est passé par référence constante et stocké comme référence, évitant de copier tout le système
    explicit Solveur(const SystemeArgumentation& sa);

    
    // VE-PR : Vérifie si S est une extension préférée
    // retourne true si oui, false sinon
    bool verifierExtensionPreferee(const EnsembleArguments& S) const;

    // VE-ST : Vérifie si S est une extension stable
    // retourne true si oui, false sinon
    bool verifierExtensionStable(const EnsembleArguments& S) const;

    // DC-PR : Acceptabilité crédule pour la sémantique préférée
    // retourne true si oui, false sinon
    bool acceptationCredulePreferee(const std::string& arg) const;

    // DS-PR : Acceptabilité sceptique pour la sémantique préférée
    // retourne true si oui, false sinon
    bool acceptationSceptiquePreferee(const std::string& arg) const;

    // DC-ST : Acceptabilité crédule pour la sémantique stable
    // retourne true si oui, false sinon
    bool acceptationCreduleStable(const std::string& arg) const;

    // DS-ST : Acceptabilité sceptique pour la sémantique stable
    // retourne true si oui, false sinon (si aucune extension stable, retourne true)
    bool acceptationSceptiqueStable(const std::string& arg) const;

    
    // Résout un problème donné (dispatch vers la bonne méthode)
    bool resoudre(TypeProbleme probleme,
                  const EnsembleArguments& ensembleArgs,
                  const std::string& argument) const;


private:
    // Référence constante vers le système d'argumentation
    // const ne modifie pas le système ; & est une référence pas une copie
    const SystemeArgumentation& systeme_;

};

#endif // SOLVEUR_HPP
