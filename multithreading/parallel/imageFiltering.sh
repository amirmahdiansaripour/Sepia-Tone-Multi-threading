#!/bin/bash
make clean
make
rm filteredImages/*
./ImageFilters.out inputs/1.bmp
wait
python3 plot.py
