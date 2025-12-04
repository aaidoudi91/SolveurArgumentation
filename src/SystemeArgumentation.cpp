/* SystemeArgumentation.cpp
 * Implémentation de la classe SystemeArgumentation représentant un système d'argumentation abstrait F = ⟨A, R⟩ */

#include "SystemeArgumentation.hpp"
#include <iostream>  // pour std::cout (affichage)



// Ajoute un argument au système
bool SystemeArgumentation::ajouterArgument(const std::string& arg) {
    // std::set::insert retourne une std::pair<iterator, bool>
    // - iterator : pointe vers l'élément (inséré ou déjà existant)
    // - bool : true si insertion réussie, false si déjà présent
    auto resultat = arguments_.insert(arg);

    // resultat.second vaut true si l'argument a été ajouté
    // si l'argument existait déjà, std::set refuse l'insertion (pas de doublons)
    bool aEteAjoute = resultat.second;

    // Si l'argument est nouveau, on l'initialise dans nos dictionnaires
    // ce qui crée des entrées vides pour cet argument
    if (aEteAjoute) {
        // std::unordered_map::operator[] crée automatiquement une entrée
        // si la clé n'existe pas, avec une valeur par défaut (ici : set vide)
        // différent du .at() qui lancerait une exception si clé absente
        attaquants_[arg];  // Initialise à std::set<std::string>{}
        attaques_de_[arg];  // Initialise à std::set<std::string>{}
    }
    return aEteAjoute;
}

// Ajoute une attaque (source -> cible) au système
bool SystemeArgumentation::ajouterAttaque(const std::string& source,
                                          const std::string& cible) {
    // Valider les préconditions pour eviter d'avoir des attaques entre arguments inexistants
    if (!argumentExiste(source) || !argumentExiste(cible)) {
        // si l'un des arguments n'existe pas, on refuse l'ajout
        return false;
    }

    // std::pair<X, Y> représente une paire de valeurs
    // on peut le construire avec std::make_pair(x, y) ou {x, y} (C++11)
    auto resultat = attaques_.insert({source, cible});
    bool aEteAjoutee = resultat.second;

    // Si l'attaque est nouvelle, on met à jour nos structures d'accès rapide
    if (aEteAjoutee) {
        // "cible" a maintenant "source" comme attaquant
        attaquants_[cible].insert(source);
        // "source" attaque maintenant "cible"
        attaques_de_[source].insert(cible);
    }
    return aEteAjoutee;
}


const std::set<std::string>& SystemeArgumentation::getArguments() const {
    // On retourne une référence (pas de copie) pour la performance
    // "const" garantit que l'appelant ne peut pas modifier l'ensemble
    return arguments_;
}

// Retourne l'ensemble des attaques
const std::set<std::pair<std::string, std::string>>&
SystemeArgumentation::getAttaques() const {
    return attaques_;
}


// Retourne les attaquants d'un argument donné
std::set<std::string> SystemeArgumentation::getAttaquants(
    const std::string& arg) const {
    // std::unordered_map::find(key) retourne un itérateur :
    // - si la clé existe : itérateur vers l'élément (paire clé-valeur)
    // - si la clé n'existe pas : itérateur == map.end()
    auto it = attaquants_.find(arg);

    // Si l'argument existe dans le dictionnaire
    if (it != attaquants_.end()) {
        // un itérateur de map/unordered_map pointe vers une std::pair :
        // - it->first  : la clé (ici : le nom de l'argument)
        // - it->second : la valeur (ici : std::set des attaquants)
        return it->second;  // retourne le set des attaquants
    }

    // Si l'argument n'existe pas ou n'a pas d'attaquants alors retourne un ensemble vide
    return std::set<std::string>{};  // ensemble vide
}

// Retourne les arguments attaqués par un argument donné
std::set<std::string> SystemeArgumentation::getAttaquesDe(
    const std::string& arg) const {

    // Même logique que getAttaquants, mais avec l'autre dictionnaire
    auto it = attaques_de_.find(arg);
    if (it != attaques_de_.end()) {
        return it->second;
    }
    return std::set<std::string>{};
}


// Retourne le nombre d'arguments
size_t SystemeArgumentation::getNombreArguments() const {
    // size_t : type entier non-signé
    return arguments_.size();
}

// Retourne le nombre d'attaques
size_t SystemeArgumentation::getNombreAttaques() const {
    return attaques_.size();
}


// Vérifie si un argument existe dans le système
bool SystemeArgumentation::argumentExiste(const std::string& arg) const {
    // std::set::find(value) retourne un itérateur
    // - si trouvé : itérateur vers l'élément
    // - si non trouvé : itérateur == set.end()
    return arguments_.find(arg) != arguments_.end();
}

// Vérifie si une attaque existe
bool SystemeArgumentation::attaqueExiste(const std::string& source,
                                         const std::string& cible) const {
    // on cherche la paire (source, cible) dans l'ensemble des attaques
    return attaques_.find({source, cible}) != attaques_.end();
}


// Vide le système (supprime tous les arguments et attaques)
void SystemeArgumentation::vider() {
    // .clear() supprime tous les éléments et libère la mémoire associée
    arguments_.clear();
    attaques_.clear();
    attaquants_.clear();
    attaques_de_.clear();
    // après clear(), les conteneurs sont vides, mais toujours utilisables
}


// Affiche le système (pour debug et visualisation)
void SystemeArgumentation::afficher() const {
    std::cout << "Système d'Argumentation : " << std::endl;

    // Affichage des arguments
    std::cout << "\nArguments (" << arguments_.size() << ") :" << std::endl;
    // "const auto&" signifie :
    // - auto : le compilateur déduit le type automatiquement
    // - & : référence (pas de copie)
    // - const : on ne modifie pas l'élément
    for (const auto& arg : arguments_) {
        std::cout << "  - " << arg << std::endl;
    }

    // Affichage des attaques
    std::cout << "\nAttaques (" << attaques_.size() << ") :" << std::endl;
    for (const auto& attaque : attaques_) {
        // attaque est un std::pair<std::string, std::string>
        // attaque.first = source, attaque.second = cible
        std::cout << "  - " << attaque.first
                  << " → " << attaque.second << std::endl;
    }

    // Affichage des attaquants pour chaque argument
    std::cout << "\nDétail des attaquants :" << std::endl;
    for (const auto& arg : arguments_) {
        std::set<std::string> attaquants = getAttaquants(arg);
        std::cout << "  " << arg << " est attaqué par : ";
        if (attaquants.empty()) {
            std::cout << "(aucun)";
        } else {
            std::cout << "{";
            auto it = attaquants.begin();
            auto dernier = std::prev(attaquants.end());
            // std::prev(it) retourne l'itérateur précédent (STL <iterator>)
            for (; it != attaquants.end(); ++it) {
                std::cout << *it;
                if (it != dernier) {
                    std::cout << ", ";
                }
            }
            std::cout << "}";
        }
        std::cout << std::endl;
    }
}
