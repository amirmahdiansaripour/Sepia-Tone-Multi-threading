#!/bin/bash
make clean
make
./ImageFilters.out princes.bmp
wait
python3 plot.py
