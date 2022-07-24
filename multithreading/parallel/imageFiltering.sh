#!/bin/bash
make clean
make
rm filteredImages/*
./ImageFilters.out inputs/inputs2/inputs/6.bmp
wait
python3 plot.py
