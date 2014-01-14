#!/bin/bash

GROUP=`groups | cut -d\  -f1`
if ["$1" = "watch"]; then
    watch squeue -M chemistry -p $GROUP -o "%7i %30j %3u %2t %.12M %.6D %.4C %.R" --sort=S
else
    squeue -M chemistry -p $GROUP -o "%7i %30j %3u %2t %.12M %.6D %.4C %.R"
fi
