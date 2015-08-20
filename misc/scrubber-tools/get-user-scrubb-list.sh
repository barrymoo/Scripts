#!/usr/bin/env bash

group=$(groups | awk '{print $1}')
users=($(grep $group *.out | awk '{print $3}' | sort -u))

for i in ${users[*]}; do
    array=($(grep $i *.out | cut -d: -f1 | sort -u))
    echo "=========================="
    echo $i
    for j in ${array[*]}; do
        echo $j
    done
done

if [ ${#users[*]} -gt 0 ]; then
    echo "=========================="
fi
