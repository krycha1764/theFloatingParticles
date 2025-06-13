#!/bin/bash
rm *.o
rm FRAMES/*
./compile.sh
./particles.elf
./movie.sh
