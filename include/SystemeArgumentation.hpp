/* SystemeArgumentation.hpp
 * Définit la structure d'un système d'argumentation abstrait
 * Un système d'argumentation est un couple F = <A, R> où :
 *   - A est un ensemble d'arguments abstraits
 *   - R ⊆ A × A est une relation d'attaque entre arguments */

#ifndef SYSTEME_ARGUMENTATION_HPP
#define SYSTEME_ARGUMENTATION_HPP


#include <string>  // std::string
#include <set>  // std::set - ensemble ordonné sans doublons (complexité O(log n) pour insertion/recherche)
#include <unordered_map>    // std::unordered_map - dictionnaire (hash map)
#include <utility>          // std::pair - paire de valeurs



class SystemeArgumentation {
private:
    // Ensemble A des arguments
    // std::set garantit l'unicité des éléments et l'ordre lexicographique
    // pas vector car on vérifie souvent "a ∈ A" (O(log n) contre O(n))
    std::set<std::string> arguments_;

    // Ensemble R des attaques, stocké comme ensemble de paires
    // std::pair<X,Y> est un tuple de 2 éléments de types X et Y
    // (a, b) signifie "a attaque b"
    std::set<std::pair<std::string, std::string>> attaques_;

    // Dictionnaire : pour chaque argument, qui l'attaque ; permettant un accès en O(1)
    // clé = argument cible, valeur = ensemble de ses attaquants
    std::unordered_map<std::string, std::set<std::string>> attaquants_;

    // Dictionnaire inverse : pour chaque argument, qui attaque-t-il
    // clé = argument source, valeur = ensemble des arguments qu'il attaque
    std::unordered_map<std::string, std::set<std::string>> attaques_de_;

public:
    // Constructeur par défaut créant un système d'argumentation vide
    // = default permet de générer le constructeur standard
    SystemeArgumentation() = default;

    // Destructeur appelé automatiquement quand l'objet est détruit
    // = default car pas de ressources à libérer manuellement
    ~SystemeArgumentation() = default;
    
    // Ajoute un argument au système
    // référence & évite de copier le string et const garantit qu'on ne modifie pas l'original
    // retourne true si ajouté, false si déjà existant
    bool ajouterArgument(const std::string& arg);

    // Ajoute une attaque (source -> cible) au système
    // retourne true si ajoutée, false si déjà existante ou arguments invalides
    bool ajouterAttaque(const std::string& source, const std::string& cible);

    // Retourne l'ensemble des arguments
    // retour par référence constante pour éviter une copie coûteuse
    const std::set<std::string>& getArguments() const;

    // Retourne l'ensemble des attaques
    const std::set<std::pair<std::string, std::string>>& getAttaques() const;

    // Retourne les attaquants d'un argument donné
    // si l'argument n'existe pas ou n'a pas d'attaquants, retourne ensemble vide
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
