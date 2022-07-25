#!/bin/bash
cd parallel
make clean
make
rm parallelFiltering/*
./ParellelFiltering.out ../inputs/16.bmp oneTime
wait
cd ../plot-results
python3 plot.py
