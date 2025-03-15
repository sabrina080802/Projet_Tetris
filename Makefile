.DEFAULT_GOAL := package

# Nom du compilateur
CC = gcc

# Flags de compilation
CFLAGS = -Wall -Wextra -std=c11 -I/opt/homebrew/include

# Flags de linkage (ajoutez -lSDL2_image pour SDL2_image)
LIBS = -L/opt/homebrew/lib -lSDL2 -lSDL2_image

# Répertoires
SRC_DIR = src
RES_DIR = ressources
BIN_DIR = bin

# Liste des sources et des objets (les objets seront placés dans bin/)
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(BIN_DIR)/%.o)
EXEC = tetris

.PHONY: all package run clean copy_ressources

# Cible all : compilation de l'exécutable
all: $(BIN_DIR)/$(EXEC)

$(BIN_DIR)/$(EXEC): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) -o $@ $^ $(LIBS)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BIN_DIR)
	$(CC) -c $< $(CFLAGS) -o $@

# Copie du dossier ressources dans bin/
copy_ressources:
	@echo "Copie du dossier ressources..."
	@mkdir -p $(BIN_DIR)/$(RES_DIR)
	@if ls $(RES_DIR)/* 1> /dev/null 2>&1; then \
		cp -R $(RES_DIR)/* $(BIN_DIR)/$(RES_DIR)/; \
	else \
		echo "Aucun fichier dans $(RES_DIR), rien à copier."; \
	fi

# Cible run : lance l'exécutable compilé
run:
	@echo "Lancement du projet..."
	$(BIN_DIR)/$(EXEC)

# Cible package : compile, copie les ressources, puis lance l'exécutable
package: all copy_ressources
	@echo "Packaging terminé dans le dossier $(BIN_DIR)"
	@$(MAKE) run

# Nettoyage du dossier bin/
clean:
	rm -rf $(BIN_DIR)
