#!/bin/bash

CFLAGS="-I../src -Wall -Wextra -pedantic -std=c11 $(sdl2-config --cflags --libs) -lm -O3 -g"

gcc main.c renderer.c ../src/origami.c ../src/components/*.c $CFLAGS -o origami_demo
