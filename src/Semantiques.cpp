/* Semantiques.cpp
 * Implémentation des algorithmes de résolution utilisant une approche par labelling et backtracking. */

#include "Semantiques.hpp"
#include <algorithm>  // std::sort, std::binary_search
#include <functional>

// Fonctions internes encapsulées dans un namespace anonyme pour ne pas polluer l'espace global
namespace {
    using namespace std;
    using namespace Utilitaires;

    // États possibles pour un argument lors du labelling
    enum Label { UNDEC = 0, IN = 1, OUT = 2 };

    // Vérifie si l'ajout de candidat à S conserve l'admissibilité, utilisé pour vérifier VE-PR
    bool estExtensibleAvec(const EnsembleIds& S, int candidat, const SystemeArgumentation& sa) {
        // S est trié. On vérifie si candidat est déjà dedans
        if (std::binary_search(S.begin(), S.end(), candidat)) {
            return false;  // Déjà présent donc pas une extension stricte
        }

        // On crée S' = S U {candidat}
        EnsembleIds S_prime = S;
        S_prime.push_back(candidat);
        // On maintient trie pour faciliter les algos utilitaires
        std::sort(S_prime.begin(), S_prime.end());

        // Vérification rapide : S' doit être admissible
        return estAdmissible(S_prime, sa);
    }

    // Étend un ensemble admissible de manière gloutonne pour en faire une extension maximale
    void etendreEnMaximal(EnsembleIds& S, const SystemeArgumentation& sa) {
        size_t nbArgs = sa.getNbArguments();
        for (size_t i = 0; i < nbArgs; ++i) {
            // On tente d'ajouter l'argument i s'il est compatible
            // estExtensibleAvec vérifie déjà la présence, pas besoin de revérifié
            if (estExtensibleAvec(S, static_cast<int>(i), sa)) {
                S.push_back(static_cast<int>(i));
                std::sort(S.begin(), S.end()); // On re-trie après insertion
            }
        }
    }

    // Cherche à construire un ensemble admissible valide respectant les contraintes labels.
    // Retourne true si une solution existe.
    bool trouverAdmissibleRecursive(vector<Label>& labels, const vector<vector<int>>& parents,
        const vector<vector<int>>& adjacence, const SystemeArgumentation& sa) {

        int argAProbleme = -1;

        // 1. Détection de conflits ou de besoin de défense
        // On cherche un argument IN qui n'est pas correctement défendu
        for (size_t i = 0; i < labels.size(); ++i) {
            if (labels[i] == IN) {
                for (int attaquant : parents[i]) {
                    if (labels[attaquant] == IN) return false;  // Conflit interne implique une branche morte
                    if (labels[attaquant] != OUT) {  // L'attaquant n'est ni IN ni OUT donc UNDEC
                        // Il faut le passer à OUT pour défendre i
                        argAProbleme = static_cast<int>(i);
                        break; // On a trouvé un problème à résoudre
                    }
                }
            }
            if (argAProbleme != -1) break;  // On sort de la boucle
        }
        // Si aucun problème détecté, l'ensemble courant est admissible
        if (argAProbleme == -1) return true;

        // 2. Résolution du problème : backtracking
        // id est l'argument IN qui est attaqué par des arguments non-OUT
        int id = argAProbleme;
        // On cherche l'agresseur spécifique qui pose problème
        for (int agresseur : parents[id]) {
            if (labels[agresseur] != OUT) {
                // Pour mettre attaquant à OUT, il faut qu'un de ses parents devienne IN
                const vector<int>& defenseursPotentiels = parents[agresseur];
                if (defenseursPotentiels.empty()) return false;  // Impossible de défendre, pas de contre-attaque

                // On essaie chaque défenseur potentiel
                 for (int defenseur : defenseursPotentiels) {
                     // Si le défenseur est déjà OUT ou s'auto-attaque, inutile de le choisir : élagage
                     if (labels[defenseur] == OUT) continue;
                     if (sa.attaqueExiste(defenseur, defenseur)) continue;

                     vector<Label> backup = labels;
                     labels[defenseur] = IN;

                     // On propage : les cibles du nouveau défenseur deviennent OUT
                     bool conflitImmediat = false;
                     for (int cible : adjacence[defenseur]) {
                         if (labels[cible] == IN) { conflitImmediat = true; break; }
                         labels[cible] = OUT;
                     }
                     // Récursion
                     if (!conflitImmediat) {
                         if (trouverAdmissibleRecursive(labels, parents, adjacence, sa)) return true;
                     }
                     labels = backup;
                 }
                // Si aucun défenseur n'a fonctionné pour cet agresseur, cette branche est un échec
                return false;
            }
        }
        return true;
    }


    // Parcourt les arguments un par un pour construire une extension stable.
    bool trouverStableRecursive(vector<Label>& labels, int index, const SystemeArgumentation& sa,
        const vector<vector<int>>& parents) {
        size_t n = labels.size();

        // Cas de base : tous les arguments ont été traités
        if (static_cast<size_t>(index) == n) {
            // Vérification finale : L'ensemble des IN doit attaquer tous les OUT
            EnsembleIds S;
            for(size_t i=0; i<n; ++i) if(labels[i] == IN) S.push_back(static_cast<int>(i));
            return attaqueToutExterieur(S, sa);
        }

        // Si l'argument est déjà décidé par propagation précédente
        if (labels[index] != UNDEC) {
            // Vérification de cohérence
            if (labels[index] == IN) {
                // S'il est IN, aucun attaquant ne peut être IN
                for (int p : parents[index]) if (labels[p] == IN) return false;
            }
            return trouverStableRecursive(labels, index + 1, sa, parents);
        }

        // Branche 1 - Tenter de mettre l'argument à IN
        {
            bool possibleIN = true;
            for (int p : parents[index]) {
                if (labels[p] == IN) { possibleIN = false; break; }
            }

            // Propagation : Tous les voisins attaqués deviennent OUT
            if (possibleIN) {
                vector<Label> backup = labels;
                labels[index] = IN;

                // Propagation : Tous les voisins attaqués deviennent OUT
                const auto& cibles = sa.getAdjacence()[index];
                bool conflit = false;
                for (int c : cibles) {
                    if (labels[c] == IN) { conflit = true; break; }
                    labels[c] = OUT;
                }

                if (!conflit) {
                    if (trouverStableRecursive(labels, index + 1, sa, parents)) return true;
                }
                labels = backup; // Backtrack
            }
        }

        // Branche 2 - Tenter de mettre l'argument à OUT
        {
            vector<Label> backup = labels;
            labels[index] = OUT;
            if (trouverStableRecursive(labels, index + 1, sa, parents)) return true;
            labels = backup;
        }

        return false;
    }
} // namespace


// Vérification (VE)
// Vérifie si S est une extension stable
bool Semantiques::verifierStable(const EnsembleIds& S, const SystemeArgumentation& sa) {
    // Une extension stable est sans conflit et attaque tous les arguments extérieurs
    if (!estSansConflit(S, sa)) return false;
    return attaqueToutExterieur(S, sa);
}

// Vérifie si S est une extension préférée
bool Semantiques::verifierPreferee(const EnsembleIds& S, const SystemeArgumentation& sa) {
    // 1. S doit être un ensemble admissible
    if (!estAdmissible(S, sa)) return false;

    // 2. S doit être maximal
    // Heuristique : on vérifie si on peut ajouter un seul argument externe i
    // tel que S U {i} reste admissible : si oui alors S n'est pas maximal
    size_t nbArgs = sa.getNbArguments();
    vector<bool> estDansS(nbArgs, false);  // Vecteur booléen pour éviter la recherche linéaire dans S
    for (int id : S) estDansS[id] = true;

    for (size_t i = 0; i < nbArgs; ++i) {
        if (!estDansS[i]) {
            // Si on peut étendre avec i, alors S n'est pas une extension préférée
            if (estExtensibleAvec(S, static_cast<int>(i), sa)) {
                return false;
            }
        }
    }
    return true;
}


// Decision Credulous (DC)
// Acceptabilité crédule pour la sémantique stable
bool Semantiques::credulousStable(int argId, const SystemeArgumentation& sa) {
    size_t n = sa.getNbArguments();
    vector<Label> labels(n, UNDEC);

    // Initialisation : on force l'argument cible à IN
    labels[argId] = IN;

    // Propagation : ses cibles deviennent OUT
    const auto& cibles = sa.getAdjacence()[argId];
    for (int c : cibles) {
        if (c == argId) return false; // Auto-attaque
        labels[c] = OUT;
    }

    // Vérification : ses parents ne peuvent pas être IN
    for (int p : sa.getParents()[argId]) {
        if (p == argId) return false;
    }

    return trouverStableRecursive(labels, 0, sa, sa.getParents());
}

// Acceptabilité crédule pour la sémantique préférée
bool Semantiques::credulousPreferred(int argId, const SystemeArgumentation& sa) {
    // On cherche une extension admissible contenant argId
    size_t n = sa.getNbArguments();
    vector<Label> labels(n, UNDEC);

    // 1. Hypothèse : argId est IN
    labels[argId] = IN;

    // 2. Propagation aux cibles (elles deviennent OUT)
    const auto& cibles = sa.getAdjacence()[argId];
    for (int c : cibles) {
        if (c == argId) return false; // Auto-attaque
        labels[c] = OUT;
    }

    // 3. Vérification de base (pas d'auto-attaque parentale)
    for (int p : sa.getParents()[argId]) {
        if (p == argId) return false; // Auto-attaque
    }

    // On ne force pas les attaquants à OUT ici. On laisse le solveur trouverAdmissibleRecursive
    // détecter qu'ils ne sont pas OUT et chercher des défenseurs.
    return trouverAdmissibleRecursive(labels, sa.getParents(), sa.getAdjacence(), sa);
}

// Decision Skeptical (DS)
// Acceptabilité sceptique pour la sémantique stable
bool Semantiques::skepticalStable(int argId, const SystemeArgumentation& sa) {
    // On cherche de contre-exemple : si on trouve une extension stable où argId n'est pas IN, alors return FALSE

    size_t n = sa.getNbArguments();
    vector<Label> labels(n, UNDEC);
    // On force argId à être OUT
    labels[argId] = OUT;

    if (trouverStableRecursive(labels, 0, sa, sa.getParents())) {
        return false; // Contre-exemple trouvé
    }
    // Si aucune extension stable ne peut contenir argId à OUT, alors il est dans toutes
    return true;
}

// Acceptabilité sceptique pour la sémantique préférée
bool Semantiques::skepticalPreferred(int argId, const SystemeArgumentation& sa) {
    // Il doit être au moins crédule une fois
    if (!credulousPreferred(argId, sa)) return false;

    // On cherche un contre-exemple par exploration en essayant de construire une ext pr qui ne contient pas argId

    size_t n = sa.getNbArguments();
    vector<Label> labels(n, UNDEC);
    // On force argId à être OUT
    labels[argId] = OUT;

    // On lance le solveur pour trouver n'importe quel ensemble admissible qui respecte argId = OUT
    // On itère sur tous les autres arguments pour tenter de construire une extension sans argId

    // Fonction interne pour tenter de trouver un contre-exemple depuis un point de départ
    auto chercherContreExemple = [&](int startNode) -> bool {
        if (startNode == argId) return false;

        vector<Label> localLabels = labels; // Copie avec argId = OUT
        localLabels[startNode] = IN;

        // Petite propagation locale
        for(int c : sa.getAdjacence()[startNode]) localLabels[c] = OUT;
        for(int p : sa.getParents()[startNode]) if(p == startNode) return false;

        if (trouverAdmissibleRecursive(localLabels, sa.getParents(), sa.getAdjacence(), sa)) {
            // On a trouvé un ensemble admissible S ne contenant pas argId
            EnsembleIds S;
            for(size_t i=0; i<n; ++i) if(localLabels[i] == IN) S.push_back((int)i);

            // On l'étend au maximum
            etendreEnMaximal(S, sa);

            // Vérification finale : est-ce que l'extension maximale contient argId
            // (Il est possible qu'en étendant S, on ait été obligé d'ajouter argId pour défendre le reste)
            bool contientArgId = false;
            for(int x : S) if(x == argId) { contientArgId = true; break; }

            if (!contientArgId) return true; // C'est un contre-exemple valide
        }
        return false;
    };

    // 1. Test de l'ensemble vide (cas où argId n'est pas nécessaire)
    EnsembleIds vide;
    etendreEnMaximal(vide, sa);
    if (!std::binary_search(vide.begin(), vide.end(), argId)) return false;

    // 2. Exploration depuis chaque autre argument
    for (size_t i = 0; i < n; ++i) {
        if (static_cast<int>(i) == argId) continue;
        if (chercherContreExemple(static_cast<int>(i))) {
            return false; // Contre-exemple trouvé
        }
    }

    // Si aucun contre-exemple n'est trouvé après exploration exhaustive des points de départ,
    // on déduit que argId est sceptiquement accepté
    return true;
}
