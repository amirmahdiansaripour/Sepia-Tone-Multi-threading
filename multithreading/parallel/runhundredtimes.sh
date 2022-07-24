#!/bin/bash
# rm optimized-number-of-threads.csv
for i in {8..14}; do
    make clean
    make
    rm filteredImages/*
    for j in {1..100}; do
        ./ImageFilters.out inputs/${i}.bmp; 
        if (($j == 1)); then
            echo ${j} running on inputs/${i}.bmp done!;
        else 
            echo ${j} runnings on inputs/${i}.bmp done!;
        fi
    done
done
python3 histogram.py