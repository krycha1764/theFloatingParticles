#!/bin/bash

LINKER_LIBS="-lm -fopenmp"
LINKER_ARGS="-I . -O3 -Wall"
COMP_ARGS="-c -I . -O3 -Wall -fopenmp"

echo "Compiling..."
gcc $COMP_ARGS main.c -o main.o
gcc $COMP_ARGS particle.c -o particle.o
gcc $COMP_ARGS render.c -o render.o

echo "Linking..."
gcc main.o particle.o render.o -o particles.elf $LINKER_ARGS $LINKER_LIBS

echo "DONE"
