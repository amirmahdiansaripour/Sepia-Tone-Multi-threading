#!/bin/bash
cd parallel
make clean
make
rm parallelFiltering/*
rm ../samples.csv
./ParellelFiltering.out ../inputs/25.bmp oneTime
cd ../serial
make clean
make
./SerialFiltering.out ../inputs/25.bmp
wait
cd ../plot-results
python3 plot.py
