#!/usr/bin/env python
import CubeFile as cf

A = cf.CubeFile("cn9-bp-tzvp-excite-32.cube")
B = cf.CubeFile(A)
for i in range(0, 50):
    cube = A*B
print(cube.integrate())
