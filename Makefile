# === Variables ===
CC = gcc
CFLAGS  = -Wall -Wextra -std=c11 -I/opt/homebrew/include
LDFLAGS = -L/opt/homebrew/lib -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_gfx -lSDL2_mixer -mwindows

SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin
TARGET = $(BIN_DIR)/tetris

RES_DIR = ressources
BIN_RES_DIR = $(BIN_DIR)/ressources

# === Sources & Objets ===
SRCS = $(shell find $(SRC_DIR) -name '*.c')
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# === Cibles principales ===
.PHONY: default all run clean

# Cible par défaut
default: run

# Compilation + ressources
all: $(TARGET)
	@echo "Copie des ressources dans $(BIN_RES_DIR)..."
	@mkdir -p $(BIN_RES_DIR)
	@cp -r $(RES_DIR)/* $(BIN_RES_DIR)/

# Lancement du jeu
run: all
	@echo "Lancement du jeu..."
	$(TARGET)

# === Compilation ===
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# === Création des dossiers ===
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# === Nettoyage ===
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
