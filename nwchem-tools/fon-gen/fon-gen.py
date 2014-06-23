#!/usr/bin/env python

import os
import sys
import argparse
import re

try:
  username=os.environ['USER']                                                                                                  
  print ('+++++++++++++++++++')
  print (' Hello '+username+'!')
  print ('+++++++++++++++++++')
  # Parse and process command line arguments
  parser = argparse.ArgumentParser(usage='fon-gen.py outfile',description='Delocalization Script Generator', prefix_chars='-')
  parser.add_argument('outfile', help='NWChem output file')
  parser.add_argument('-np', metavar='N', type=int, nargs=1, default=[4], help='Number of processors, default=4')
  parser.add_argument('-t', metavar='N', type=int, nargs=1, default=[2], help='Job Time(hours), default=2')
  parser.add_argument('-nosub',action='store_false', help='Generate NW/SLURM files but don\'t submit jobs')
  parser.add_argument('-sub',action='store_true', help='After -nosub, this keyword will submit jobs')
  args = parser.parse_args()

  #Set some variables
  inpFile=args.outfile
  fileName, fileExt = os.path.splitext(inpFile)
  movecsFile=fileName+'.movecs'
  ifile=open(inpFile,'r')
  aofile=open(fileName+'-sa-base.nw','w')
  cofile=open(fileName+'-sc-base.nw','w')
  numProcs=args.np[0]
  jobTime=args.t[0]
  submitJobs=args.nosub
  subSlurmOnly=args.sub
  HOMO=0

  if subSlurmOnly:
    print ('Submitting potentially modified SLURM jobs')
    for i in range(0,6):
      submitAFile=fileName+'-sa-'+str(i)+'.slurm'
      submitCFile=fileName+'-sc-'+str(i)+'.slurm'
      print ('Submitting --> '+submitAFile+' and '+submitCFile)
      os.system('sbatch '+submitAFile)
      os.system('sbatch '+submitCFile)
      print ('Another one bites the dust!, moving on...')
    print ('--> Fractional calculation jobs submitted :) <--')
    sys.exit()

  line=ifile.readline()
  while line:
    if line.find("Occ=2.00") != -1:
      orbitalLine=line.split()
      HOMO=orbitalLine[1]
    if line.find("Occ=1.00") != -1:
      orbitalLine=line.split()
      HOMO=orbitalLine[1]
    if line.find("Occ=0.00") != -1:
      break  
    line=ifile.readline()
  
  if HOMO == 0:
    print ('Could not find HOMO')
  else:
    print('The HOMO is -> '+HOMO)
  ifile.seek(0)
  match=0
  
  while line:
    if line.find("echo of input deck") != -1:
      line=ifile.readline()
      line=ifile.readline()
      while line.find("=============") == -1:
        if re.match('title',line):
          aofile.write(line.rstrip()+'-sa-FILENUM\n')
          cofile.write(line.rstrip()+'-sc-FILENUM\n')
          match=1
        if re.match('start',line):
          aofile.write(line.rstrip()+'-sa-FILENUM\n')
          cofile.write(line.rstrip()+'-sc-FILENUM\n')
          match=1
        if re.match('dft',line):
          aofile.write(line)
          cofile.write(line)
          aofile.write('odft\n')
          cofile.write('odft\n')
          aofile.write('convergence nolevelshifting\n')
          cofile.write('convergence nolevelshifting\n')
          aofile.write('vectors input '+fileName+'.movecs output '+fileName+'-sa-FILENUM.movecs\n')
          cofile.write('vectors input '+fileName+'.movecs output '+fileName+'-sc-FILENUM.movecs\n')
          #cation lines
          cofile.write('fon alpha partial 1 electrons 0 filled '+HOMO+'\n')
          cofile.write('fon beta partial 1 electrons PARAM filled '+str(int(HOMO)-1)+'\n')
          #anion lines
          aofile.write('fon alpha partial 1 electrons 0 filled '+HOMO+'\n')
          aofile.write('fon beta partial 1 electrons PARAM filled '+HOMO+'\n')
          match=1
        if match != 1:
          aofile.write(line)
          cofile.write(line)
        else:
          match = 0
        line=ifile.readline()
      break
    line=ifile.readline()
  
  ifile.close()
  aofile.close()
  cofile.close()
  
  aFile=fileName+'-sa-base.nw'
  cFile=fileName+'-sc-base.nw'
  aFileRead=open(aFile,'r')
  cFileRead=open(cFile,'r')
  aText=aFileRead.read()
  cText=cFileRead.read()
  
  #Should copy these to fileName-sa-PARAM.nw
  for i in range(0,6):
    param=float(i)/5
    newAFile=fileName+'-sa-'+str(i)+'.nw'
    newCFile=fileName+'-sc-'+str(i)+'.nw'
    tempA=aText.replace('PARAM',str(param))
    tempC=cText.replace('PARAM',str(param))
    tempA=tempA.replace('FILENUM',str(i))
    tempC=tempC.replace('FILENUM',str(i))
    tempAWrite=open(newAFile,'w')
    tempCWrite=open(newCFile,'w')
    tempAWrite.write(tempA)
    tempCWrite.write(tempC)
    tempAWrite.close()
    tempCWrite.close()
     
  aFileRead.close() 
  cFileRead.close() 
  
  #Now lets remove our base files and generate some SLURM files
  os.system('rm *base*')
  movecsFile=fileName+'.movecs'
  for i in range(0,6):
    newAFile=fileName+'-sa-'+str(i)+'.nw'
    newCFile=fileName+'-sc-'+str(i)+'.nw'
    os.system('sgen.py -i '+newAFile+' -np '+str(numProcs)+' -t '+str(jobTime)+' -add '+movecsFile+' > /dev/null 2>&1')
    os.system('sgen.py -i '+newCFile+' -np '+str(numProcs)+' -t '+str(jobTime)+' -add '+movecsFile+' > /dev/null 2>&1')
 
  if submitJobs:
    for i in range(0,6):
      submitAFile=fileName+'-sa-'+str(i)+'.slurm'
      submitCFile=fileName+'-sc-'+str(i)+'.slurm'
      print ('Submitting --> '+submitAFile+' and '+submitCFile)
      os.system('sbatch '+submitAFile)
      os.system('sbatch '+submitCFile)
      print ('Another one bites the dust!, moving on...')
    print ('--> Fractional calculation jobs submitted :) <--')
  else:
    print ('NOSUB detected --> NW/Slurm files generated...exiting')

except (KeyboardInterrupt):
  print ('Keyboard interrupt. Aborting')
  sys.exit()
