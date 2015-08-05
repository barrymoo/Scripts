#!/bin/bash

if [ "$1" = "-h" -o "$1" == "--help" ]; then
    echo "bqueue.sh [options]"
    echo "--> Options Include <--"
    echo "'-h/--help' show this screen and exit"
    echo "'-w/--watch' watch the output instead of print"
elif [ "$1" = "-w" -o "$1" = "--watch" ]; then
     watch 'squeue -M chemistry,ub-hpc,industry -u $USER -S S -o "%7i %3P %35j %2t %.12M %.6D %.4C %.R"'
else
    squeue -M chemistry,ub-hpc,industry -u $USER -o "%7i %3P %35j %2t %.12M %.6D %.4C %.R"
fi
