# Makefile

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -I include -O3 # Ajout de -O3 pour la performance finale
# -MMD -MP génèrent des fichiers .d pour gérer les dépendances header automatiquement
DEPFLAGS = -MMD -MP

LDFLAGS =
TARGET = solveur
SRC_DIR = src
OBJ_DIR = obj
INC_DIR = include

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))
# Liste des fichiers de dépendances générés (.d)
DEPS = $(OBJECTS:.o=.d)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@echo "Édition des liens..."
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)
	@echo "Compilation terminée : $(TARGET)"

# Inclusion des fichiers de dépendances générés
-include $(DEPS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@echo "Compilation de $<..."
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

.PHONY: clean test rebuild

clean:
	@echo "Nettoyage..."
	rm -rf $(OBJ_DIR) $(TARGET)

rebuild: clean all
