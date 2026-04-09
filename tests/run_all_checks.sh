#!/usr/bin/env bash
set -euo pipefail

echo "[1/4] Unity unit + integration tests"
tests/run_tests.sh

echo "[2/4] Strict compile check (GCC)"
mkdir -p tests/.obj
for src in src/origami.c src/components/*.c; do
  name="$(basename "${src%.*}")"
  gcc -std=c11 -Wall -Wextra -pedantic -Isrc -c "$src" -o "tests/.obj/${name}.o"
done
echo "STRICT_COMPILE_OK"

echo "[3/4] Documentation consistency checks"
tests/run_doc_checks.sh

echo "[4/4] Demo build smoke check"
SDL_CFLAGS=""
SDL_LIBS=""

if command -v sdl2-config >/dev/null 2>&1; then
  SDL_CFLAGS="$(sdl2-config --cflags)"
  SDL_LIBS="$(sdl2-config --libs)"
elif [[ -f /mingw64/include/SDL2/SDL.h ]]; then
  SDL_CFLAGS="-I/mingw64/include -I/mingw64/include/SDL2"
  SDL_LIBS="-L/mingw64/lib -lmingw32 -lSDL2main -lSDL2"
fi

gcc demo/main.c demo/renderer.c src/origami.c src/components/*.c \
  -Isrc -Wall -Wextra -pedantic -std=c11 \
  ${SDL_CFLAGS} ${SDL_LIBS} -lm -o tests/demo_smoke
