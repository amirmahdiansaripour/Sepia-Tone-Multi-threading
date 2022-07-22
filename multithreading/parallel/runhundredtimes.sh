#!/bin/bash
make clean
make
rm results/*
for i in {1..30}; do
    ./ImageFilters.out palace-rotate.bmp; 
    echo ${i} running done!
done