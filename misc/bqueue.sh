#!/bin/bash

if [ "$1" = "w" ]; then
    watch 'squeue -M chemistry,ub-hpc -u $USER -S S -o "%7i %3P %35j %2t %.12M %.6D %.4C %.R"'
else
    squeue -M chemistry,ub-hpc -u $USER -o "%7i %3P %35j %2t %.12M %.6D %.4C %.R"
fi
