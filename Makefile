# Makefile

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -I include -O3  # Ajout de -O3 pour la performance finale
DEPFLAGS = -MMD -MP  # -MMD -MP génèrent des fichiers .d pour gérer les dépendances header automatiquement

LDFLAGS =
TARGET = solveur
SRC_DIR = src
OBJ_DIR = obj
INC_DIR = include

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))
DEPS = $(OBJECTS:.o=.d)  # Liste des fichiers de dépendances générés (.d)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)
	@echo "Compilation terminée : $(TARGET)"

-include $(DEPS)  # Inclusion des fichiers de dépendances générés

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@echo "Compilation de $< :"
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -c $< -o $@

$(OBJ_DIR):
	@echo "Nettoyage :"
	mkdir -p $(OBJ_DIR)

.PHONY: clean test rebuild

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

rebuild: clean all
