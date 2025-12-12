/* Utilitaires.cpp
 * Implémentation des fonctions utilitaires pour l'argumentation */

#include "Utilitaires.hpp"
#include <algorithm>  // std::sort
#include <sstream>  // std::stringstream


namespace Utilitaires {

// Convertit une liste de noms en une liste d'identifiants triée, en ignorant les noms inexistants
EnsembleIds convertirNomsEnIds(const std::vector<std::string>& noms, const SystemeArgumentation& sa) {
    EnsembleIds ids;
    ids.reserve(noms.size());  // Allocation préventive pour éviter les réallocations multiples
    for (const auto& nom : noms) {
        if (sa.argumentExiste(nom)) {
            ids.push_back(sa.getId(nom));  // Conversion O(1) via la hash map du système
        }
    }
    // Tri permettant d'utiliser la recherche binaire via std::binary_search
    // et garantit l'unicité des représentations d'ensembles
    std::sort(ids.begin(), ids.end());
    return ids;
}

// Convertit une liste d'identifiants en une liste de noms pour l'affichage
std::vector<std::string> convertirIdsEnNoms(const EnsembleIds& ids, const SystemeArgumentation& sa) {
    std::vector<std::string> noms;
    noms.reserve(ids.size());
    for (int id : ids) {
        noms.push_back(sa.getNom(id));  // Accès O(1) au vecteur de noms
    }
    return noms;
}

// Vérifie si un ensemble d'arguments est sans conflit
bool estSansConflit(const EnsembleIds& S, const SystemeArgumentation& sa) {
    // Double boucle imbriquée pour tester toutes les paires possibles
    for (size_t i = 0; i < S.size(); ++i) {
        for (size_t j = 0; j < S.size(); ++j) {
            // On teste si l'élément i attaque l'élément j
            if (sa.attaqueExiste(S[i], S[j])) {
                return false;  // Dès qu'une attaque interne est trouvée, l'ensemble est invalide
            }
        }
    }
    return true;
}

// Vérifie si un ensemble S défend un argument spécifique contre tous ses attaquants
bool defend(const EnsembleIds& S, int cibleId, const SystemeArgumentation& sa) {
    // Récupération optimisée des attaquants via le graphe inverse (parents_)
    const std::vector<int>& attaquants = sa.getParents()[cibleId];

    // Pour chaque attaquant, on doit trouver une contre-attaque venant de S
    for (int attaquant : attaquants) {
        bool estContreAttaque = false;
        for (int defenseur : S) {  // Recherche d'un défenseur dans S
            if (sa.attaqueExiste(defenseur, attaquant)) {
                estContreAttaque = true;  // L'attaquant est neutralisé
                break;  // Pas besoin de chercher d'autres défenseurs pour cet attaquant
            }
        }
        // Si un seul attaquant n'est pas neutralisé, la défense échoue
        if (!estContreAttaque) return false;
    }
    return true;
}

// Vérifie si un ensemble est admissible
bool estAdmissible(const EnsembleIds& S, const SystemeArgumentation& sa) {
    if (!estSansConflit(S, sa)) return false;  // Condition de base : cohérence interne

    for (int arg : S) {  // Condition de défense : S doit se protéger lui-même
        if (!defend(S, arg, sa)) return false;  // Si un membre n'est pas défendu, S n'est pas admissible
    }
    return true;
}

// Vérifie si un ensemble attaque tous les arguments qui ne lui appartiennent pas
bool attaqueToutExterieur(const EnsembleIds& S, const SystemeArgumentation& sa) {
    size_t nbArgs = sa.getNbArguments();

    std::vector<bool> estDansS(nbArgs, false);  // Masque booléen pour tester l'appartenance à S en O(1)
    for (int id : S) estDansS[id] = true;

    // Parcours de tous les arguments de l'univers
    for (size_t a = 0; a < nbArgs; ++a) {
        // On ne s'intéresse qu'aux arguments exterieurs à S (càd A\S)
        if (!estDansS[a]) {
            bool estAttaque = false;
            // On regarde les parents de a (ceux qui l'attaquent)
            const auto& attaquantsDeA = sa.getParents()[a];

            // Vérification : est-ce que l'un des attaquants appartient à S ?
            for (int attaquant : attaquantsDeA) {
                if (estDansS[attaquant]) {
                    estAttaque = true;
                    break;  // a est bien attaqué par S
                }
            }
            // Si un argument extérieur n'est attaqué par personne de S, la condition échoue
            if (!estAttaque) return false;
        }
    }
    return true;
}

// Calcule l'ensemble de tous les arguments défendus par S.
EnsembleIds fonctionCaracteristique(const EnsembleIds& S, const SystemeArgumentation& sa) {
    EnsembleIds result;
    size_t nbArgs = sa.getNbArguments();

    // Teste la défense pour chaque argument de l'univers
    for (size_t a = 0; a < nbArgs; ++a) {
        if (defend(S, static_cast<int>(a), sa)) {
            result.push_back(static_cast<int>(a));
        }
    }
    return result;
}

// Génère une représentation d'un ensemble d'identifiants
std::string afficher(const EnsembleIds& S, const SystemeArgumentation& sa) {
    if (S.empty()) return "{}";
    std::stringstream ss;
    ss << "{";
    for (size_t i = 0; i < S.size(); ++i) {
        ss << sa.getNom(S[i]);  // Retraduction identifiant vers nom
        if (i < S.size() - 1) ss << ", ";
    }
    ss << "}";
    return ss.str();
}

}
