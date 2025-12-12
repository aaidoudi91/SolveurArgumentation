/* Semantiques.hpp
 * Moteur de raisonnement implémentant les algorithmes de résolution pour les sémantiques stables et préférées. */

#ifndef SEMANTIQUES_HPP
#define SEMANTIQUES_HPP

#include "SystemeArgumentation.hpp"
#include "Utilitaires.hpp"


class Semantiques {
public:
    // Vérification VE
    // Vérifie si S est une extension stable
    static bool verifierStable(const Utilitaires::EnsembleIds& S, const SystemeArgumentation& sa);
    // Vérifie si S est une extension préférée
    static bool verifierPreferee(const Utilitaires::EnsembleIds& S, const SystemeArgumentation& sa);

    // Decision Credulous DC
    // Acceptabilité crédule pour la sémantique stable
    static bool credulousStable(int argId, const SystemeArgumentation& sa);
    // Acceptabilité crédule pour la sémantique préférée
    static bool credulousPreferred(int argId, const SystemeArgumentation& sa);

    // Decision Skeptical DS
    // Acceptabilité sceptique pour la sémantique stable
    static bool skepticalStable(int argId, const SystemeArgumentation& sa);
    // Acceptabilité sceptique pour la sémantique préférée
    static bool skepticalPreferred(int argId, const SystemeArgumentation& sa);
};

#endif // SEMANTIQUES_HPP
