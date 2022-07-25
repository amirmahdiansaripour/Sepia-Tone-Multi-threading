#!/bin/bash
make clean
make
rm filteredImages/*
./ImageFilters.out inputs/14.bmp run hundred 
wait
python3 plot.py
