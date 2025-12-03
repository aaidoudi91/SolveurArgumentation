# Makefile Projet SolveurArgumentation


# Compilateur C++ à utiliser
CXX = g++

# Flags de compilation :
#   -std=c++17  : Utiliser le standard C++17
#   -Wall       : Activer tous les warnings courants
#   -Wextra     : Warnings supplémentaires
#   -g          : Inclure infos de debug (pour gdb)
#   -I include  : Chercher les headers dans le dossier "include"
CXXFLAGS = -std=c++17 -Wall -Wextra -g -I include

# Flags pour l'édition de liens (linking)
# Pour l'instant vide, mais utile si on ajoute des bibliothèques
LDFLAGS =

# Nom de l'exécutable final
TARGET = solveur

# Dossiers
SRC_DIR = src
OBJ_DIR = obj
INC_DIR = include

# Liste de tous les fichiers sources (.cpp)
# $(wildcard ...) trouve tous les fichiers correspondant au pattern
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)

# Liste des fichiers objets (.o) correspondants
# On remplace src/X.cpp par obj/X.o
# $(patsubst pattern,replacement,text) fait cette substitution
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))


# Règle par défaut (première règle = celle exécutée par "make" sans argument)
# "all" est une cible "phony" (pas un vrai fichier)
all: $(TARGET)

# Règle pour créer l'exécutable
# $@ = nom de la cible (ici: solveur)
# $^ = toutes les dépendances (ici: tous les .o)
$(TARGET): $(OBJECTS)
	@echo "Édition des liens."
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)
	@echo "Compilation terminée : $(TARGET)"

# Règle générique pour compiler les .cpp en .o
# $< = première dépendance (le .cpp)
# $@ = cible (le .o)
# | $(OBJ_DIR) = le dossier obj doit exister (order-only prerequisite)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@echo "Compilation de $<."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Créer le dossier obj s'il n'existe pas
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Règle pour nettoyer les fichiers générés
# .PHONY indique que "clean" n'est pas un fichier
.PHONY: clean
clean:
	@echo "Nettoyage..."
	rm -rf $(OBJ_DIR) $(TARGET)
	@echo "Nettoyage terminé."

# Règle pour tout recompiler from scratch
.PHONY: rebuild
rebuild: clean all

# Règle pour exécuter un test rapide
.PHONY: test
test: $(TARGET)
	@echo "Test avec l'exemple du sujet..."
	./$(TARGET) -p VE-PR -f tests/cas_test/exemple_sujet.apx -a a,c,d


# Ces règles indiquent que si un header change, il faut recompiler les .cpp
# qui l'incluent. Sans ça, modifier un .hpp ne déclencherait pas de recompilation.

$(OBJ_DIR)/main.o: $(INC_DIR)/Parseur.hpp $(INC_DIR)/Solveur.hpp
$(OBJ_DIR)/SystemeArgumentation.o: $(INC_DIR)/SystemeArgumentation.hpp
$(OBJ_DIR)/Parseur.o: $(INC_DIR)/Parseur.hpp $(INC_DIR)/SystemeArgumentation.hpp
$(OBJ_DIR)/Utilitaires.o: $(INC_DIR)/Utilitaires.hpp $(INC_DIR)/SystemeArgumentation.hpp
$(OBJ_DIR)/Semantiques.o: $(INC_DIR)/Semantiques.hpp $(INC_DIR)/Utilitaires.hpp
$(OBJ_DIR)/Solveur.o: $(INC_DIR)/Solveur.hpp $(INC_DIR)/Semantiques.hpp
