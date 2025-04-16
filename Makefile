.DEFAULT_GOAL := package

# === Variables ===
CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -I/opt/homebrew/include
LIBS    = -L/opt/homebrew/lib -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_gfx

SRC_DIR = src
OBJ_DIR = bin
EXEC    = tetris

# === Détection automatique des sources ===
SRC     = $(shell find $(SRC_DIR) -name '*.c')
OBJ     = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

# === Règles ===
.PHONY: all package run clean copy_ressources

all: $(OBJ_DIR)/$(EXEC)

$(OBJ_DIR)/$(EXEC): $(OBJ)
	@mkdir -p $(dir $@)
	$(CC) -o $@ $^ $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) -c $< -o $@ $(CFLAGS)

copy_ressources:
	@mkdir -p $(OBJ_DIR)/ressources
	@if ls ressources/* 1> /dev/null 2>&1; then \
		cp -R ressources/* $(OBJ_DIR)/ressources/; \
	else \
		echo "Aucun fichier dans ressources."; \
	fi

run:
	$(OBJ_DIR)/$(EXEC)

package: all copy_ressources
	$(MAKE) run

clean:
	rm -rf $(OBJ_DIR)
