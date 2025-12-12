/* Parseur.hpp
 * Module responsable de l'analyse syntaxique des fichiers .apx pour construire le système d'argumentation. */

#ifndef PARSEUR_HPP
#define PARSEUR_HPP

#include <string>  // std::string
#include <stdexcept>  // std::runtime_error
#include "SystemeArgumentation.hpp"

// Exception personnalisée pour distinguer les erreurs de syntaxe des erreurs système
class ErreurParsing : public std::runtime_error {
public:
    explicit ErreurParsing(const std::string& message)
        : std::runtime_error(message) {}  // Appelle le constructeur parent
};

// Classe utilitaire statique pour le parsing
class Parseur {
public:
    // Point d'entrée principal : lit un fichier et retourne l'objet SystemeArgumentation construit
    // Lance une exception ErreurParsing si le fichier est invalide
    static SystemeArgumentation parserFichier(const std::string& cheminFichier);

private:
    // Analyse une ligne déclarant un argument via arg(x). et extrait le nom x
    // Retourne le nom de l'argument extrait et lance une exception si format invalide
    static std::string parserLigneArgument(const std::string& ligne);
    // Analyse une ligne déclarant une attaque via att(x,y). et extrait la paire {x, y}
    // Retourne une paire (source, cible) et lance une exception si format invalide
    static std::pair<std::string, std::string> parserLigneAttaque(const std::string& ligne);
    // Vérifie si un nom d'argument est valide (càd lettres, chiffres, underscore, pas arg ni att)
    static bool estNomValide(const std::string& nom);
    // Supprime les espaces en début et fin de chaîne
    static std::string trim(const std::string& str);
};

#endif // PARSEUR_HPP
