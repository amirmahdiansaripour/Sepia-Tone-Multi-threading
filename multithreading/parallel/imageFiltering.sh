#!/bin/bash
make clean
make
./ImageFilters.out ut.bmp
wait
python3 plot.py
