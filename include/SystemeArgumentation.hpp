/* Fichier : SystemeArgumentation.hpp
 * Description : Définit la structure d'un système d'argumentation abstrait
 * Un système d'argumentation est un couple F = <A, R> où :
 *   - A est un ensemble d'arguments abstraits
 *   - R ⊆ A × A est une relation d'attaque entre arguments */

#ifndef SYSTEME_ARGUMENTATION_HPP
#define SYSTEME_ARGUMENTATION_HPP


#include <string>  // std::string - Chaînes de caractères C++
#include <set>  // std::set - Ensemble ordonné sans doublons (Complexité O(log n) pour insertion/recherche)
#include <unordered_set>    // std::unordered_set - Ensemble non ordonné (hash) (Complexité O(1) en moyenne)
#include <unordered_map>    // std::unordered_map - Dictionnaire (hash map)
#include <vector>           // std::vector - Tableau dynamique
#include <utility>          // std::pair - Paire de valeurs



class SystemeArgumentation {
private:
    // L'ensemble A des arguments
    // std::set garantit : unicité des éléments et l'ordre lexicographique
    // pas vector car on vérifie souvent "a ∈ A" (O(log n) vs O(n))
    std::set<std::string> arguments_;

    // L'ensemble R des attaques, stocké comme ensemble de paires
    // std::pair<X,Y> est un tuple de 2 éléments de types X et Y
    // (a, b) signifie "a attaque b"
    std::set<std::pair<std::string, std::string>> attaques_;

    // Dictionnaire : pour chaque argument, qui l'attaque
    // Clé = argument cible, Valeur = ensemble de ses attaquants
    // permet un accès rapide "qui attaque a ?" en O(1)
    // Sans ça, il faudrait parcourir toutes les attaques à chaque fois
    std::unordered_map<std::string, std::set<std::string>> attaquants_;

    // Dictionnaire inverse : pour chaque argument, qui attaque-t-il
    // Clé = argument source, Valeur = ensemble des arguments qu'il attaque
    // Utile pour vérifier si un ensemble "attaque tous les autres"
    std::unordered_map<std::string, std::set<std::string>> attaques_de_;

public:
    // Constructeur par défaut
    // Crée un système d'argumentation vide (aucun argument, aucune attaque)
    // Le "= default" dit au compilateur de générer le constructeur standard
    SystemeArgumentation() = default;

    // Destructeur
    // Appelé automatiquement quand l'objet est détruit
    // "= default" car on n'a pas de ressources à libérer manuellement
    ~SystemeArgumentation() = default;


    // Ajoute un argument au système
    // Paramètre passé par RÉFÉRENCE CONSTANTE (const std::string&)
    // - Référence & : évite de copier la string (performance)
    // - Const : garantit qu'on ne modifie pas l'original
    // Retourne true si ajouté, false si déjà existant
    bool ajouterArgument(const std::string& arg);

    // Ajoute une attaque (source -> cible) au système
    // Retourne true si ajoutée, false si déjà existante ou arguments invalides
    bool ajouterAttaque(const std::string& source, const std::string& cible);

    // Retourne l'ensemble des arguments
    // Retour par référence constante pour éviter une copie coûteuse
    const std::set<std::string>& getArguments() const;

    // Retourne l'ensemble des attaques
    const std::set<std::pair<std::string, std::string>>& getAttaques() const;

    // Retourne les attaquants d'un argument donné
    // Si l'argument n'existe pas ou n'a pas d'attaquants, retourne ensemble vide
    std::set<std::string> getAttaquants(const std::string& arg) const;

    // Retourne les arguments attaqués par un argument donné
    std::set<std::string> getAttaquesDe(const std::string& arg) const;

    // Retourne le nombre d'arguments
    // size_t est un type entier non-signé, standard pour les tailles
    size_t getNombreArguments() const;

    // Retourne le nombre d'attaques
    size_t getNombreAttaques() const;


    // Vérifie si un argument existe dans le système
    bool argumentExiste(const std::string& arg) const;

    // Vérifie si une attaque existe
    bool attaqueExiste(const std::string& source, const std::string& cible) const;


    // Vide le système (supprime tous les arguments et attaques)
    void vider();

    // Affiche le système (pour debug)
    void afficher() const;
};


#endif // SYSTEME_ARGUMENTATION_HPP
