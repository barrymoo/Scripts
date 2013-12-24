#!/usr/bin/env python
import argparse
import os
import sys


class Vector:
    def  __init__(self, x, y, z):
        self.x = x
        self.y = y
        self.z = z


def subtractVector(vec1, vec2):
    vec3 = Vector(vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z)
    return vec3


#Let's parse some arguments -->
#This program takes an sph file and solvent shell size
parser = argparse.ArgumentParser(description='Centers atom N in an XYZ file to the origin')
parser.add_argument('inputXYZFile', type=str, help='XYZ file to adjust')
parser.add_argument('atom', type=int, help='Atom to be placed at origin')
args = parser.parse_args()

try:
    inpFile = args.inputXYZFile
    atomNum = args.atom
    xyzFile = open(inpFile,'r')
    line = xyzFile.readline()
    numAtoms = int(line)
    if numAtoms < atomNum:
        print('There are only ' + str(numAtoms) + ' atoms')
        print('Specify a valid atom number...Exiting')
        sys.exit()
    line = xyzFile.readline()
    for i in range(1,atomNum+1):
        line = xyzFile.readline()
    #Now we have the atom to center to but we need its coordinates
    spLine = line.split()
    centerVector = Vector(float(spLine[1]), float(spLine[2]), float(spLine[3]))
    #Start from the top and subtract the centerVector
    xyzFile.seek(0)
    ofName = 'centered-on-atom-' + str(atomNum) + '.xyz'
    outFile = open(ofName, 'w')
    line = xyzFile.readline()
    outFile.write(line)
    line = xyzFile.readline()
    outFile.write(line)
    for i in range(0,numAtoms):
        line = xyzFile.readline()
        spLine = line.split()
        tmpVector = Vector(float(spLine[1]), float(spLine[2]), float(spLine[3]))
        newVector = subtractVector(tmpVector, centerVector)
        outFile.write(spLine[0] + ' ' + str(newVector.x) + ' ' + 
                      str(newVector.y) + ' ' + str(newVector.z) + '\n')
    xyzFile.close()
except KeyboardInterrupt:
    print('Interrupt detected...')
