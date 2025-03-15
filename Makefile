.DEFAULT_GOAL := package

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -I/opt/homebrew/include
LIBS = -L/opt/homebrew/lib -lSDL2 -lSDL2_image
SRC_DIR = src
OBJ_DIR = bin
SRC = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/views/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
EXEC = tetris

.PHONY: all package run clean

all: $(OBJ_DIR)/$(EXEC)

$(OBJ_DIR)/$(EXEC): $(OBJ)
	@mkdir -p $(OBJ_DIR)
	$(CC) -o $@ $^ $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) -c $< $(CFLAGS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/views/%.c
	@mkdir -p $(dir $@)
	$(CC) -c $< $(CFLAGS) -o $@

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
