#!/usr/bin/env python
from glob import glob
import re
from sys import exit


# This function replaces all non-digit('\D') characters with '' 
def find_digit(filename):
    return int(re.sub('\D', '', filename))


# Get a list of files ending in '.cube'
cubeFiles = glob("*.cube")

# If the list is not greater than 0 in length, we should exit
if not len(cubeFiles) > 0:
    exit('Error: There are no .cube files in this directory?')

# First let's split the cube files into two lists:
#  one that contains digits and one that does not
numeric_sort = []
alpha_sort = []
for i in cubeFiles:
    if re.search('\d', i):
        numeric_sort.append(i)
    else:
        alpha_sort.append(i)

# alpha_sort can be sorted with 'sorted' function
cubeFiles = sorted(alpha_sort)

# numeric_sort needs to be done with key in 'sorted'
cubeFiles += sorted(numeric_sort, key = find_digit)

# Finally write the cubefiles file if the number of cubes
#  is greater than 0
if len(cubeFiles) > 0:
    f = open('cubefiles', 'w')
    f.write("{0!s}\n".format(len(cubeFiles)))
    for i in cubeFiles:
        f.write("{0!s}\n".format(i))
