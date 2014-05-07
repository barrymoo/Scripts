#!/usr/bin/env python
from glob import glob
from re import split
from operator import itemgetter
from sys import exit

cubeFiles = glob("*cube")
for i in range(len(cubeFiles)):
    sp = cubeFiles[i].split('-')
    prefix = '-'.join(sp[0:-1])
    num = sp[-1].replace('.cube','')
    cubeFiles[i] = [prefix, int(num)]

cubeFiles = sorted(cubeFiles, key=itemgetter(1))

f = open('cubefiles', 'w')
f.write("{0!s}\n".format(len(cubeFiles)))
for i in cubeFiles:
    f.write("{0}-{1!s}.cube\n".format(i[0], i[1]))
