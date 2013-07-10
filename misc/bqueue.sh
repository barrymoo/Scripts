#!/bin/bash

squeue -u $USER -o "%6i %3P %35j %2t %.12M %.6D %.4C %.R"
