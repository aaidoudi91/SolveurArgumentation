/* main.cpp
 * Gère le parsing de la ligne de commande, l'appel des modules et l'affichage final. */

#include <iostream>  // std::cout, std::cerr
#include <string>  // std::string
#include <vector>  // std::vector
#include <sstream>  // std::stringstream
#include "Parseur.hpp"
#include "Solveur.hpp"
#include "SystemeArgumentation.hpp"


// Découpe une chaîne "a,b,c" en vecteur {"a", "b", "c"}
std::vector<std::string> decouperArguments(const std::string& chaineArgs) {
    std::vector<std::string> result;
    std::stringstream ss(chaineArgs);
    std::string segment;
    while (std::getline(ss, segment, ',')) {
        if (!segment.empty()) {
            result.push_back(segment);
        }
    }
    return result;
}

// Aide pour l'affichage de l'usage en cas d'erreur
void afficherUsage(const char* progName) {
    std::cerr << "Usage : " << progName << " -p PROBLEM -f FILE -a ARGUMENTS" << std::endl;
    std::cerr << "Exemple : " << progName << " -p VE-PR -f file.apx -a a,b,c" << std::endl;
    std::cerr << "Problemes supportés : VE-PR, DC-PR, DS-PR, VE-ST, DC-ST, DS-ST" << std::endl;
}


int main(int argc, char* argv[]) {
    // Initialisation des paramètres
    std::string typeProblemeStr;
    std::string cheminFichier;
    std::string argumentsStr;

    // Parsing de la ligne de commande
    // On attend : -p <prob> -f <file> -a <args>
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-p" && i + 1 < argc) {
            typeProblemeStr = argv[++i];
        } else if (arg == "-f" && i + 1 < argc) {
            cheminFichier = argv[++i];
        } else if (arg == "-a" && i + 1 < argc) {
            argumentsStr = argv[++i];
        }
    }

    // Vérification que tous les arguments nécessaires sont là
    if (typeProblemeStr.empty() || cheminFichier.empty() || argumentsStr.empty()) {
        std::cerr << "Erreur : Arguments manquants." << std::endl;
        afficherUsage(argv[0]);
        return 1;
    }

    // Conversion du type de problème
    try {
        TypeProbleme probleme;
        bool modeEnsemble = false; // true pour VE (ensemble), false pour DC/DS (un seul argument)
        if (typeProblemeStr == "VE-PR") {
            probleme = TypeProbleme::VE_PR;
            modeEnsemble = true;
        } else if (typeProblemeStr == "VE-ST") {
            probleme = TypeProbleme::VE_ST;
            modeEnsemble = true;
        } else if (typeProblemeStr == "DC-PR") {
            probleme = TypeProbleme::DC_PR;
        } else if (typeProblemeStr == "DS-PR") {
            probleme = TypeProbleme::DS_PR;
        } else if (typeProblemeStr == "DC-ST") {
            probleme = TypeProbleme::DC_ST;
        } else if (typeProblemeStr == "DS-ST") {
            probleme = TypeProbleme::DS_ST;
        } else {
            std::cerr << "Erreur : Probleme inconnu '" << typeProblemeStr << "'" << std::endl;
            return 1;
        }

        // Chargement et Construction du Système
        SystemeArgumentation sa = Parseur::parserFichier(cheminFichier);
        // Initialisation du solveur
        Solveur solveur(sa);
        // Préparation des données pour le solveur
        std::vector<std::string> argsVector = decouperArguments(argumentsStr);
        bool resultat = false;

        if (modeEnsemble) {
            // Pour VE, on passe tout le vecteur
            resultat = solveur.resoudre(probleme, argsVector, "");
        } else {
            // Pour DC et DS, on attend un seul argument
            if (argsVector.empty()) {
                std::cerr << "Erreur : Aucun argument fourni pour la requête." << std::endl;
                return 1;
            }
            // On prend le premier argument de la liste (même si l'utilisateur a mis a,b,c par erreur)
            const std::string& argCible = argsVector[0];
            resultat = solveur.resoudre(probleme, {}, argCible);
        }

        // Affichage du résultat final
        std::cout << (resultat ? "YES" : "NO") << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Erreur : " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
