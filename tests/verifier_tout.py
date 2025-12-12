""" verifier_tout.py : génère exhaustivement tous les sous-ensembles d’arguments (soit 2^N), et en vérifie le statut
à l’aide du solveur. À ne lancer que sur des petits systèmes d'argumentation pour verifier la logique. """

import subprocess
import sys
import itertools
import os

SOLVEUR_PATH = "./solveur"  # Chemin vers l'exécutable
FICHIER_APX = "tests/cas_test/test_af5.apx"  # Fichier par défaut

def get_arguments(fichier_apx):
    """ Extrait la liste des arguments depuis le fichier apx. """
    args = []
    with open(fichier_apx, 'r') as f:
        for line in f:
            if line.startswith("arg("):
                # Extrait 'a' de "arg(a)."
                arg = line.split("(")[1].split(")")[0]
                args.append(arg)
    return sorted(args)

def powerset(iterable):
    """ Génère tous les sous-ensembles possibles. """
    s = list(iterable)
    return itertools.chain.from_iterable(itertools.combinations(s, r) for r in range(len(s)+1))

def call_solveur(probleme, fichier, args_str):
    """ Appelle le solveur C++ et retourne True si YES. """
    cmd = [SOLVEUR_PATH, "-p", probleme, "-f", fichier, "-a", args_str]
    try:
        result = subprocess.run(cmd, capture_output=True, text=True)
        return result.stdout.strip() == "YES"
    except Exception as e:
        print(f"Erreur appel solveur: {e}")
        return False

def main():
    fichier = FICHIER_APX
    if len(sys.argv) > 1:
        fichier = sys.argv[1]
    if not os.path.exists(fichier):
        print(f"Erreur: Fichier {fichier} introuvable.")
        return
    print(f"Analyse de : {fichier}")
    arguments = get_arguments(fichier)

    # Vérification VE
    extensions_pref = []
    extensions_stable = []
    for i, subset in enumerate(powerset(arguments)):  # On teste tous les sous-ensembles (2^N)
        args_str = ",".join(subset)
        if call_solveur("VE-PR", fichier, args_str):  # VE-PR
            extensions_pref.append(set(subset))
        if call_solveur("VE-ST", fichier, args_str):  # VE-ST
            extensions_stable.append(set(subset))
    print("Extensions :")
    print(f"  - VE-PR : {extensions_pref}")
    print(f"  - VE-ST : {extensions_stable}")

    # Décision DC/DS
    dc_pr = []
    ds_pr = []
    dc_st = []
    ds_st = []
    for arg in arguments:
        if call_solveur("DC-PR", fichier, arg):  # DC-PR
            dc_pr.append(arg)
        if call_solveur("DS-PR", fichier, arg):  # DS-PR
            ds_pr.append(arg)
        if call_solveur("DC-ST", fichier, arg):  # DC-ST
            dc_st.append(arg)
        if call_solveur("DS-ST", fichier, arg):  # DS-ST
            ds_st.append(arg)
    print("Acceptabilité crédule :")
    print(f"  - DC-PR : {dc_pr}")
    print(f"  - DC-ST : {dc_st}")
    print("Acceptabilité sceptique :")
    print(f"  - DS-PR : {ds_pr}")
    print(f"  - DS-ST : {ds_st}")

if __name__ == "__main__":
    main()
