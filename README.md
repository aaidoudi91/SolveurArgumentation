# Solveur de Systèmes d'Argumentation

Ce projet implémente un solveur pour le raisonnement sur des **Systèmes d'Argumentation Abstraits**.
L'application permet de résoudre six problèmes décisionnels liés aux sémantiques **Stable (ST)** et **Préférée (PR)**.

L'objectif est de fournir une réponse sur des graphes pouvant atteindre quelques dizaines d'arguments, en traitant des 
problèmes de complexité allant de polynomiale à **$\Pi_2^p$-complet**. Le solveur s'appuie sur une architecture 
optimisée et des algorithmes de recherche.

Les problèmes traités sont :
1.  **VE (Verification)** : Vérifier si un ensemble $S$ est une extension valide.
2.  **DC (Credulous Acceptance)** : Est-ce qu'un argument $a$ appartient à *au moins une* extension ?
3.  **DS (Skeptical Acceptance)** : Est-ce qu'un argument $a$ appartient à *toutes* les extensions ?

## Architecture du Projet

```
SolveurArgumentation/   
│
├── Makefile                      # Script de compilation (gère les dépendances et l'édition de liens)
├── solveur                       # Exécutable final (généré après compilation)
│
├── README.md                     # Documentation
│
├── include/                      # Fichiers d'en-tête (.hpp)
│   ├── SystemeArgumentation.hpp  # Structure de données
│   ├── Parseur.hpp               # Analyse des fichiers .apx
│   ├── Utilitaires.hpp           # Propriétés (sans-conflit, défense, ...)
│   ├── Semantiques.hpp           # Moteur de résolution
│   └── Solveur.hpp               # Interface d'adaptation (String <-> Int)
│
├── src/                          # Fichiers sources (.cpp)
│   ├── main.cpp                  # Point d'entrée (CLI)
│   ├── SystemeArgumentation.cpp
│   ├── Parseur.cpp
│   ├── Utilitaires.cpp
│   ├── Semantiques.cpp
│   └── Solveur.cpp
│
├── obj/                           # Fichiers objets (.o) et dépendances (.d) générés
│
└── tests/                         # Jeux de tests
    ├── verifier_tout.py           # Script Python pour validation exhaustive
    └── cas_test/                  # Fichiers de tests (.apx)
        ├── traffic1.apx           
        └── ...
```

## Installation

Le projet est développé en **C++17**. Aucun gestionnaire de paquets externe n'est requis, seule la bibliothèque standard
est utilisée.

### Pré-requis

* Compilateur C++ compatible C++17 (`g++` ou `clang++`)

### Compilation

À la racine du projet, exécutez :

```bash
make
```

Pour nettoyer les fichiers de compilation et recompiler à zéro, exécutez :

```bash
make rebuild
```

## Utilisation

Après compilation, le programme s'exécute en ligne de commande et respecte le format suivant :

```bash
./solveur -p <PROBLEME> -f <FICHIER.apx> -a <ARGUMENTS>
```

### Paramètres

* `-p` : Type de problème : `VE-PR`, `DC-PR`, `DS-PR`, `VE-ST`, `DC-ST`, `DS-ST`.
* `-f` : Chemin vers le fichier `.apx` décrivant le graphe.
* `-a` : Arguments de la requête (séparés par des virgules).

### Exemple

Vérifier si {a, c, d} est une extension préférée du système défini dans `test_af5.apx` :
```bash
./solveur -p VE-PR -f tests/cas_test/test_af1.apx -a a,c,d
```

### Format de Sortie

Le programme affiche uniquement sur la sortie standard `stdout` :
* **YES** si la propriété est vérifiée.
* **NO** sinon.

Les erreurs éventuelles (fichier introuvable, syntaxe invalide) sont affichées sur la sortie d'erreur `stderr`.

## Tests

Le projet inclut un script de validation basé sur la force brute pour vérifier la correction logique sur de petits 
systèmes d'argumentation.

Pour lancer ce script (après la compilation) :
```bash
python tests/verifier_tout.py <FICHIER.apx>
```

## Auteur

**Aidoudi Aaron**

UE Représentation des Connaissances et Raisonnement - Master 1 Intelligence Artificielle Distribuée à l'Université 
Paris Cité

Année Universitaire 2025-2026
