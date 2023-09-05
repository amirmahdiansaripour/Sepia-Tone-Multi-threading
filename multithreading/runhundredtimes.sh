#!/bin/bash
# rm optimized-number-of-threads.csv
cd parallel
for i in {21..25}; do
    make clean
    make
    rm parallelFiltering/*
    for j in {1..100}; do
        ./ParellelFiltering.out ../inputs/${i}.bmp hundredTimes
        if (($j == 1)); then
            echo ${j} running on inputs/${i}.bmp done!;
        else 
            echo ${j} runnings on inputs/${i}.bmp done!;
        fi
    done
done
cd ../plot-results
python3 histogram.py