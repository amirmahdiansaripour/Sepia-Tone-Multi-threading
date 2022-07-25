#!/bin/bash
cd parallel
make clean
make
rm ParallelFiltering/*
./ParellelFiltering.out ../inputs/1.bmp oneTime
wait
cd ../plot-results
python3 plot.py
