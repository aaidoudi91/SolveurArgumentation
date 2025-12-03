/* Fichier : Parseur.hpp
 * Description : Parse les fichiers .apx pour créer un SystemeArgumentation
 * Format .apx attendu :
 *   arg(nom_argument).
 *   att(source,cible). */

#ifndef PARSEUR_HPP
#define PARSEUR_HPP

#include <string>
#include <stdexcept>  // std::runtime_error - Pour les exceptions

#include "SystemeArgumentation.hpp"



class ErreurParsing : public std::runtime_error {
public:
    // Constructeur prennant un message d'erreur
    // explicit empêche les conversions implicites (bonne pratique)
    // Le ": std::runtime_error(message)" appelle le constructeur parent
    explicit ErreurParsing(const std::string& message)
        : std::runtime_error(message) {}
};


class Parseur {
public:
    // Parse un fichier .apx et retourne le SystemeArgumentation correspondant
    // Lance une exception ErreurParsing si le fichier est invalide
    // "static" signifie que cette méthode appartient à la CLASSE, pas à une
    // instance. On peut l'appeler sans créer d'objet Parseur :
    //     SystemeArgumentation sa = Parseur::parserFichier("test.apx");
    // Approprié car le parseur n'a pas d'état interne à maintenir.
    static SystemeArgumentation parserFichier(const std::string& cheminFichier);


private:
    // Parse une ligne de type "arg(nom)."
    // Retourne le nom de l'argument extrait
    // Lance une exception si format invalide
    static std::string parserLigneArgument(const std::string& ligne);

    // Parse une ligne de type "att(source,cible)."
    // Retourne une paire (source, cible)
    // Lance une exception si format invalide
    static std::pair<std::string, std::string> parserLigneAttaque(const std::string& ligne);

    // Vérifie si un nom d'argument est valide
    // (lettres, chiffres, underscore, pas "arg" ni "att")
    static bool estNomValide(const std::string& nom);

    // Supprime les espaces en début et fin de chaîne (trim)
    static std::string trim(const std::string& str);
};

#endif // PARSEUR_HPP
