#!/bin/bash
make clean
make
rm results/*
./ImageFilters.out palace-rotate.bmp
wait
python3 plot.py
