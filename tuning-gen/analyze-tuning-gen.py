#!/usr/bin/env python
import os
import sys
import argparse

try:
    username=os.environ['USER']                                                                                                  
    print ('+++++++++++++++++++')
    print (' Hello '+username+'!')
    print ('+++++++++++++++++++')
    # Parse and process command line arguments
    parser = argparse.ArgumentParser(usage='tuning-gen.py -d [1,2] outfile [options]',
                                    description='Tuning Script Generator',
                                    prefix_chars='-')
    parser.add_argument('inpFile', help='NWChem file, jobname-neutral.nw')
    parser.add_argument('-d', metavar='N', type=int, nargs=1, default=[1],
                        help='Dimension of Tuning, default=1')
    parser.add_argument('-a', metavar='N', type=float, nargs=1, default=[0.],
                        help='Alpha Parameter (has no effect is -d is 2, default=0.0')
    args = parser.parse_args()

    #Lets make sure everything is there!
    inpFile=args.inpFile
    fileName, fileExt = os.path.splitext(inpFile)
    jobName = fileName.split('-')[0]
    fileList = []
    if args.d[0] == 2:
        print('Two dimensional tuning not yet supported, contact barrymoo@buffalo.edu')
        sys.exit()
    else:
        #First lets create the fileList and check to see if all the files exist
        for i in range(0, 11):
            addFileName = jobName + '-neutral-' + str(args.a[0]) + '-' + str(float(i)/20) + '.out'
            fileList.append(addFileName)
        for i in range(0, 11):
            addFileName = jobName + '-anion-' + str(args.a[0]) + '-' + str(float(i)/20) + '.out'
            fileList.append(addFileName)
        for i in range(0, 11):
            addFileName = jobName + '-cation-' + str(args.a[0]) + '-' + str(float(i)/20) + '.out'
            fileList.append(addFileName)
        for i in fileList:
            if not os.path.isfile(i):
                print('Error: missing file - ', i)
        for i in range(0, 11):
            #Get all output for neutral system
        for i in range(11, 21):
            #Get all output for anion system
        for i in range(21, 33):
            #Get all output for cation system

except (KeyboardInterrupt):
  print ('Keyboard interrupt. Aborting')
  sys.exit()
