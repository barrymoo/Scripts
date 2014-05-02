#!/usr/bin/env python
from glob import glob
from re import split
from operator import itemgetter

cubeFiles = glob("*cube")
for i in range(len(cubeFiles)):
    sp = split('-|\.', cubeFiles[i])
    cubeFiles[i] = [sp[0], int(sp[1]), sp[2]]

cubeFiles = sorted(cubeFiles, key=itemgetter(1))

f = open('cubefiles', 'w')
f.write("{0!s}\n".format(len(cubeFiles)))
for i in cubeFiles:
    f.write("{0}-{1!s}.{2}\n".format(i[0], i[1], i[2]))
