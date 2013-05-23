#!/usr/bin/env python
import glob

cubeFiles = glob.glob("*cube")
cubeFiles.sort()
f = open('cubefiles', 'w')
f.write(str(len(cubeFiles)) + '\n')
for i in cubeFiles:
    f.write(i + '\n')
