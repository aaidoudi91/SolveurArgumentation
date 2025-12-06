/* Utilitaires.cpp
 * Implémentation des fonctions utilitaires pour l'argumentation */

#include "Utilitaires.hpp"
#include <algorithm>    // Pour std::set_difference, std::set_union, etc.
#include <iterator>     // Pour std::inserter
#include <sstream>      // Pour std::stringstream (conversion string)


namespace Utilitaires {

// Vérifie si un ensemble S est sans conflit (aucun argument de S n'attaque un autre argument de S)
bool estSansConflit(const EnsembleArguments& S,
                    const SystemeArgumentation& sa) {

    // Si S est vide ou a 1 élément, toujours sans conflit
    if (S.size() <= 1) {
        return true;
    }

    // Pour chaque (a,b) dans S, si a attaque b alors retourner false, sinon si aucune attaque trouvée retourner true
    for (const auto& a : S) {
        for (const auto& b : S) {
            if (sa.attaqueExiste(a, b)) {  // vérifier si a attaque b
                return false;  // conflit détecté
            }
        }
    }

    return true;  // aucun conflit trouvé
}


// Vérifie si un ensemble S défend un argument arg (pour chaque attaquant b de arg, un c de S contre-attaque b)
bool defend(const EnsembleArguments& S,
            const std::string& arg,
            const SystemeArgumentation& sa) {

    // Récupérer l'ensemble des attaquants de arg
    EnsembleArguments attaquants = sa.getAttaquants(arg);

    if (attaquants.empty()) {  // si arg n'a aucun attaquant, il est défendu par tout
        return true;
    }

    // Pour chaque attaquant b : Vérifier qu'au moins un élément de S attaque b
    for (const auto& attaquant : attaquants) {
        bool attaquantEstContreAttaque = false;
        for (const auto& defenseur : S) {
            if (sa.attaqueExiste(defenseur, attaquant)) {
                attaquantEstContreAttaque = true;  // défenseur dans S qui attaque cet attaquant
                break;  // pas besoin de chercher d'autres défenseurs
            }
        }

        if (!attaquantEstContreAttaque) {  // si cet attaquant n'est pas contre-attaqué, S ne défend pas arg
            return false;
        }
    }

    return true; // Tous les attaquants sont contre-attaqués donc S défend arg
}


// Vérifie si un ensemble S est admissible (sans conflit et défend tous ses éléments)
bool estAdmissible(const EnsembleArguments& S,
                   const SystemeArgumentation& sa) {

    if (!estSansConflit(S, sa)) { // Sans conflit
        return false;  // Si S a un conflit, pas admissible
    }

    // Défend tous ses éléments
    for (const auto& arg : S) {
        if (!defend(S, arg, sa)) {
            return false;  // S ne défend pas arg, donc S n'est pas admissible
        }
    }

    return true;
}


// Vérifie si un ensemble S attaque tous les autres arguments
bool attaqueTousLesAutres(const EnsembleArguments& S,
                          const SystemeArgumentation& sa) {

    const EnsembleArguments& tousLesArguments = sa.getArguments(); // récupérer tous les arguments du système
    EnsembleArguments horsDeS = difference(tousLesArguments, S); // calculer A\S (arguments hors de S)

    for (const auto& arg : horsDeS) {  // pour chaque a dans A\S
        bool argEstAttaque = false;
        for (const auto& attaquant : S) {
            if (sa.attaqueExiste(attaquant, arg)) {
                argEstAttaque = true;  // arg est attaqué par attaquant
                break;  // pas besoin de chercher d'autres attaquants
            }
        }

        if (!argEstAttaque) {  // si arg n'est attaqué par personne dans S, échec
            return false;
        }
    }

    return true;  // tous les arguments hors de S sont attaqués donc succès
}



// Différence ensembliste (A\B)
EnsembleArguments difference(const EnsembleArguments& A,
                             const EnsembleArguments& B) {

    EnsembleArguments resultat;

    std::set_difference(
        A.begin(), A.end(),
        B.begin(), B.end(),
        std::inserter(resultat, resultat.begin())
    );

    return resultat;
}


// Union ensembliste (A ∪ B)
EnsembleArguments unionEnsembles(const EnsembleArguments& A,
                                 const EnsembleArguments& B) {

    EnsembleArguments resultat;

    std::set_union(
        A.begin(), A.end(),
        B.begin(), B.end(),
        std::inserter(resultat, resultat.begin())
    );

    return resultat;
}


// Intersection ensembliste (A ∩ B)
EnsembleArguments intersection(const EnsembleArguments& A,
                               const EnsembleArguments& B) {

    EnsembleArguments resultat;

    std::set_intersection(
        A.begin(), A.end(),
        B.begin(), B.end(),
        std::inserter(resultat, resultat.begin())
    );

    return resultat;
}


// Vérifie si A est sous-ensemble de B
bool estSousEnsemble(const EnsembleArguments& A,
                     const EnsembleArguments& B) {

    return std::includes(
        B.begin(), B.end(),  // Plage "conteneur" (B)
        A.begin(), A.end()   // Plage "contenu" (A)
    );
}

// Calcule F(S) = {a dans A tq S défend a} (ensemble de tous les arguments défendus par S)
EnsembleArguments fonctionCaracteristique(const EnsembleArguments& S,
                                          const SystemeArgumentation& sa) {

    EnsembleArguments resultat;

    const EnsembleArguments& tousLesArguments = sa.getArguments();

    for (const auto& arg : tousLesArguments) {  // pour chaque argument a du système
        if (defend(S, arg, sa)) {  // si S défend a
            resultat.insert(arg);  // ajouter a au résultat
        }
    }

    return resultat;
}


// Convertit un ensemble en string pour affichage
std::string ensembleVersString(const EnsembleArguments& S) {
    if (S.empty()) {
        return "{}";
    }

    std::stringstream ss;  // flux qui écrit dans une chaîne
    ss << "{";

    auto it = S.begin();
    auto dernier = std::prev(S.end());  // itérateur vers le dernier élément

    for (; it != S.end(); ++it) {
        ss << *it;
        if (it != dernier) {
            ss << ", ";
        }
    }

    ss << "}";

    return ss.str();
}
}  // namespace Utilitaires


