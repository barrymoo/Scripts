#!/bin/bash

GROUP=`groups | cut -d\  -f1`
squeue -M chemistry -p $GROUP -o "%7i %30j %3u %2t %.12M %.6D %.4C %.R"
