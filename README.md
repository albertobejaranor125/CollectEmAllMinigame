# CollectEmAllMinigame

A small SDL2-based reaction minigame where players must press the correct key (W, A, S, D) before time runs out. The game includes visual feedback, a score system, lives (hearts), a countdown timer, and interactive menus with mouse support.

## 🕹️ Features

* 🎮 Interactive main menu and restart system (mouse-controlled)
* 💡 Game states: `MENU`, `PLAYING`, `PAUSED`, `VICTORY`, `GAME_OVER`
* ⏱️ Countdown timer (30 seconds)
* 💖 Heart-based life system (3 lives)
* ✅ Random target key generator (W, A, S, D)
* 📈 Score-based victory condition (reach 100 points)
* 🐭 Mouse events to control UI buttons

## 📁 Folder Structure

```text
CollectEmAllMinigame/
├── assets/
│   ├── font.ttf
│   └── heart.png
├── main.cpp
├── README.md
└── .gitignore


## 🧰 Requirements

- [SDL2](https://www.libsdl.org/)
- [SDL2_ttf](https://github.com/libsdl-org/SDL_ttf)
- [SDL2_image](https://github.com/libsdl-org/SDL_image)
- A C++ compiler

## 🛠️ How to Build

### Linux/macOS

```bash
g++ src/main.cpp -o CollectEmAllMinigame -lSDL2 -lSDL2_image -lSDL2_ttf
./CollectEmAllMinigame

Windows (MinGW)

Make sure SDL2, SDL2_image, and SDL2_ttf are set up properly.

g++ src/main.cpp -o CollectEmAllMinigame.exe -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

Font: font.ttf (https://fonts.google.com/specimen/Open+Sans)

Heart Icon: heart.png (https://www.freeiconspng.com/thumbs/heart-png/heart-png-15.png)
