#!/bin/bash

squeue -M chemistry,ub-hpc -u $USER -o "%7i %3P %35j %2t %.12M %.6D %.4C %.R"
