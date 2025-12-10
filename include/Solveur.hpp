/* Solveur.hpp
 * Interface faisant le pont entre les requêtes utilisateur et les algorithmes de sémantiques. */

#ifndef SOLVEUR_HPP
#define SOLVEUR_HPP

#include <vector>  // std::vector
#include <string>  // std::string
#include "SystemeArgumentation.hpp"

// Enumération pour identifier le type de tâche demandée
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
    // Constructeur pour garder une référence vers le système
    explicit Solveur(const SystemeArgumentation& sa);

    // Vérifie si l'ensemble d'arguments est une extension préférée
    bool verifierExtensionPreferee(const std::vector<std::string>& ensembleNoms) const;
    // Vérifie si l'ensemble est une extension stable
    bool verifierExtensionStable(const std::vector<std::string>& ensembleNoms) const;

    // Détermine si l'argument donné appartient à au moins une extension préférée
    bool acceptationCredulePreferee(const std::string& arg) const;
    // Détermine si l'argument donné appartient à au moins une extension stable
    bool acceptationCreduleStable(const std::string& arg) const;

    // Détermine si l'argument donné appartient à toutes les extensions préférées
    bool acceptationSceptiquePreferee(const std::string& arg) const;
    // Détermine si l'argument donné appartient à toutes les extensions stables
    bool acceptationSceptiqueStable(const std::string& arg) const;

    // Appelle la bonne méthode selon le TypeProbleme
    bool resoudre(TypeProbleme probleme,
                  const std::vector<std::string>& ensembleArgs,
                  const std::string& argument) const;

private:
    // Référence constante vers le graphe
    const SystemeArgumentation& systeme_;
};

#endif // SOLVEUR_HPP
