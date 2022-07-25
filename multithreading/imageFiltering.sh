#!/bin/bash
cd parallel
make clean
make
rm filteredImages/*
./ImageFilters.out inputs/14.bmp oneTime 
wait
python3 plot.py
