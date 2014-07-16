#!/usr/bin/env python
from glob import glob
import re
from sys import exit

# This function is natural sorting algorithm
def natural_sort(fileList):
    is_int = lambda text: int(text) if re.match('\d', text) else False
    natural_key = lambda key: [is_int(x) for x in re.split('(\d+)', key)]
    return sorted(fileList, key = natural_key)

# Attempt to generate a list of cubefiles
cubeFiles = glob("*.cube")

# If the list is not greater than 0 in length, we should exit
if not len(cubeFiles) > 0:
    exit('Error: There are no .cube files in this directory?')

print(cubeFiles)

# Use our natural sort algorithm to sort the files
cubeFiles = natural_sort(cubeFiles)

print(cubeFiles)

# Finally write the cubefiles file if the number of cubes
#  is greater than 0
f = open('cubefiles', 'w')
f.write("{0!s}\n".format(len(cubeFiles)))
for i in cubeFiles:
    f.write("{0!s}\n".format(i))
