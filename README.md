#  TETRIS — C / SDL2

Ce projet est une version du jeu **Tetris** codée en **langage C** avec la bibliothèque **SDL2**.

Deux modes de jeu disponibles :
- **Marathon** : mode classique.
- **Duel** : mode 2 joueurs avec une **IA**.

---

##  Dépendances requises

Le jeu utilise les bibliothèques suivantes :

- `SDL2`
- `SDL2_image`
- `SDL2_ttf`
- `SDL2_mixer`
- `SDL2_gfx`

---

##  Installation

### ▶macOS

1. Installer [Homebrew](https://brew.sh) si ce n’est pas déjà fait :

   ```
   /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
   ```

2. Installer SDL2 et ses extensions :

   ```
   brew install sdl2 sdl2_image sdl2_ttf sdl2_mixer sdl2_gfx
   ```

3. Compiler avec `make`

---

###  Windows

1. Installer [MSYS2](https://www.msys2.org/) (recommandé).

2. Ouvrir **MSYS2 MinGW 64-bit** puis installer les bibliothèques SDL :

   ```
   pacman -S mingw-w64-x86_64-SDL2 \
               mingw-w64-x86_64-SDL2_image \
               mingw-w64-x86_64-SDL2_ttf \
               mingw-w64-x86_64-SDL2_mixer \
               mingw-w64-x86_64-SDL2_gfx
   ```

3. Compiler avec `make`

---

## Contrôles

| Action         | Touche     |
|----------------|------------|
| Gauche/Droite  | ← / → ou Q / D |
| Rotation       | ↑ ou R     |
| Accélérer      | ↓  ou S    |
| Pause/Menu     | Échap      |

---
 Créer par : Sabrina AL Hammuti
