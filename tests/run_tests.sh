#!/usr/bin/env bash
set -euo pipefail

CC_BIN="${CC:-gcc}"
OUT="tests/origami_tests"

"${CC_BIN}" \
  tests/test_*.c \
  tests/components/test_*.c \
  src/origami.c \
  src/components/*.c \
  third_party/unity/src/unity.c \
  -Isrc \
  -Itests \
  -Ithird_party/unity/src \
  -std=c11 -Wall -Wextra -pedantic \
  -lm \
  -o "${OUT}"

"./${OUT}"
