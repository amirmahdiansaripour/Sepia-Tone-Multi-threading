#!/bin/bash
rm optimized-number-of-threads.csv
for i in {1..4}; do
    make clean
    make
    rm filteredImages/*
    for j in {1..3}; do
        ./ImageFilters.out inputs/${i}.bmp; 
        echo ${j} running on inputs/${i}.bmp done!;
    done
done
python3 histogram.py