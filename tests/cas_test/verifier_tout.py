import subprocess 
import sys
import itertools
import os

SOLVEUR_PATH = "./solveur"  # Chemin vers l'exécutable
FICHIER_APX = "tests/cas_test/test_af1.apx"  # Fichier par défaut

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
    """ Appelle le solveur C++ et retourne True si YES". """
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
    print(f"Analyse complète de : {fichier}")
    arguments = get_arguments(fichier)
    print(f"Arguments trouvés : {arguments}")

    extensions_pref = []
    extensions_stable = []

    # On teste tous les sous-ensembles (2^N)
    for i, subset in enumerate(powerset(arguments)):
        args_str = ",".join(subset)
        # Test PRÉFÉRÉE (VE-PR)
        if call_solveur("VE-PR", fichier, args_str):
            extensions_pref.append(set(subset))
        # Test STABLE (VE-ST)
        if call_solveur("VE-ST", fichier, args_str):
            extensions_stable.append(set(subset))

    print(f"Extensions préférées ({len(extensions_pref)}) :")
    for ext in extensions_pref:
        print(f"  - {sorted(list(ext)) if ext else '{}'}")

    print(f"Extensions stables ({len(extensions_stable)}) :")
    for ext in extensions_stable:
        print(f"  - {sorted(list(ext)) if ext else '{}'}")

if __name__ == "__main__":
    main()
