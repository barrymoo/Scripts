#!/usr/bin/env python

import sys, os, glob

files = sorted(glob.glob('*[0-5].out'))

dftEnergy=[]
for i in range(len(files)):
  inpFile=open(files[i],'r')
  line = inpFile.readline()
  while line:
    if line.find('Total DFT') != -1:
     splitLine=line.split()
     dftEnergy.append(splitLine[4])  
     break
    line=inpFile.readline()
  inpFile.close()

neutDiff=float(dftEnergy[11])-float(dftEnergy[0])
print ('#The cation-anion neutral difference is -> '+str(neutDiff))

for i in reversed(range(1,6)):
  param=-float(i)/5
  deltaE=float(dftEnergy[(10-i)+1])-float(dftEnergy[0])
  print (str(param)+' '+str(deltaE))

for i in range(0,6):
  param=float(i)/5
  deltaE=float(dftEnergy[i])-float(dftEnergy[0])
  print (' '+str(param)+' '+str(deltaE))
  
