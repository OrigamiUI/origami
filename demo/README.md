# Origami UI Demo

A demo application showcasing all Origami UI controls using SDL2.

## Prerequisites

- GCC or Clang (C11 support)
- SDL2 development libraries

### Install SDL2

**macOS:**
```bash
brew install sdl2
```

**Ubuntu / Debian:**
```bash
sudo apt install libsdl2-dev
```

**Arch Linux:**
```bash
sudo pacman -S sdl2
```

**Windows (MSYS2):**
```bash
pacman -S mingw-w64-x86_64-SDL2
```

## Build

```bash
chmod +x build.sh
./build.sh
```

## Run

```bash
./origami_demo
```

## Demo Windows

The demo opens three windows:

- **Demo Window** — buttons, tree nodes, checkboxes, popups, background color sliders
- **Log Window** — scrollable text panel with text input
- **Style Editor** — live RGBA sliders for all 14 theme colors
