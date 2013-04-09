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
        fOut = open(jobName + '.dat', 'w')
        for i in range(0, 11):
            for j in range(0, 11):
                alpha = str(float(i)/10)
                beta = str(1.0 - float(alpha))
                gamma = str(float(j)/20)
                neutFileName = jobName + '-neutral-tuning-' + alpha + '-' + gamma + '.out'
                anFileName = jobName + '-anion-tuning-' + alpha + '-' + gamma + '.out'
                catFileName = jobName + '-cation-tuning-' + alpha + '-' + gamma + '.out'
                if i == 0 and j == 0:
                    f = open(neutFileName, 'r')
                    line = f.readline()
                    homo = 0
                    while line:
                        if line.find('Occ=2.00') != -1:
                            spLine = line.split()
                            homo = spLine[1]
                            if line.find('Occ=0.00') != -1:
                                break
                        line = f.readline()
                    f.close()
                #Now that we have the HOMO we can continue
                anHomo = str(int(homo) + 1)
                fn = open(neutFileName, 'r')
                fa = open(anFileName, 'r')
                fc = open(catFileName, 'r')
                #Neutral Lines
                line = fn.readline()
                while line:
                    if line.find('Total DFT') != -1:
                        spLine = line.split()
                        neutDftE = spLine[4].replace('D', 'E')
                    if line.find(' ' + homo + ' ') != -1 and line.find('Occ=2.00') != -1:
                        spLine = line.split()
                        neutHomoE = spLine[3].split('=')[1].replace('D', 'E')
                        break
                    line = fn.readline()
                fn.close()
                #Anion Lines
                line = fa.readline()
                while line:
                    if line.find('Total DFT') != -1:
                        spLine = line.split()
                        anDftE = spLine[4].replace('D', 'E')
                    if line.find(' ' + anHomo + ' ') != -1 and line.find('Occ=1.00') != -1:
                        spLine = line.split()
                        if spLine[3] == 'E=':
                            anHomoE = spLine[4].replace('D', 'E')
                        else:
                            anHomoE = spLine[3].split('=')[1].replace('D', 'E')
                        break
                    line = fa.readline()
                fa.close()
                #Cation Lines
                line = fc.readline()
                while line:
                    if line.find('Total DFT') != -1:
                        spLine = line.split()
                        catDftE = spLine[4].replace('D', 'E')
                        break
                    line = fc.readline()
                fc.close()
                #Put it all together 
                pOne = float(neutHomoE) + float(catDftE) - float(neutDftE)
                pTwo = float(anHomoE) + float(neutDftE) - float(anDftE)
                jSquared = 27.2116 ** 2 * (pOne ** 2 + pTwo ** 2)
                fOut.write(alpha + ' ' + beta + ' ' + gamma + ' ' + neutHomoE + ' '
                        + anHomoE + ' ' + neutDftE + ' ' + anDftE + ' ' + catDftE + ' '
                        + str(jSquared) + '\n')
        fOut.close()

    else:
        #First lets create the fileList and check to see if all the files exist
        for i in range(0, 11):
            addFileName = jobName + '-neutral-tuning-' + str(args.a[0]) + '-' + str(float(i)/20) + '.out'
            fileList.append(addFileName)
        for i in range(0, 11):
            addFileName = jobName + '-anion-tuning-' + str(args.a[0]) + '-' + str(float(i)/20) + '.out'
            fileList.append(addFileName)
        for i in range(0, 11):
            addFileName = jobName + '-cation-tuning-' + str(args.a[0]) + '-' + str(float(i)/20) + '.out'
            fileList.append(addFileName)
        for i in fileList:
            if not os.path.isfile(i):
                print('Error: missing file - ', i)
        #First lets get the HOMO
        f = open(fileList[0], 'r')
        line = f.readline()
        homo = 0
        while line:
            if line.find('Occ=2.00') != -1:
                spLine = line.split()
                homo = spLine[1]
                if line.find('Occ=0.00') != -1:
                    break
            line = f.readline()
        f.close()
        #Now let's get all the data into appropriate lists
        neutHomoE = []
        anHomoE = []
        neutDftE = []
        anDftE = []
        catDftE = []
        for i in range(0, 11):
            f = open(fileList[i], 'r')
            line = f.readline()
            while line:
                if line.find('Total DFT') != -1:
                    spLine = line.split()
                    neutDftE.append(spLine[4].replace('D', 'E'))
                if line.find(' ' + homo + ' ') != -1 and line.find('Occ=2.00') != -1:
                    spLine = line.split()
                    neutHomoE.append(spLine[3].split('=')[1].replace('D', 'E'))
                    break
                line = f.readline()
            f.close()
        for i in range(11, 22):
            f = open(fileList[i], 'r')
            line = f.readline()
            while line:
                anHomo = str(int(homo) + 1)
                if line.find('Total DFT') != -1:
                    spLine = line.split()
                    anDftE.append(spLine[4].replace('D', 'E'))
                if line.find(' ' + anHomo + ' ') != -1 and line.find('Occ=1.00') != -1:
                    spLine = line.split()
                    if spLine[3] == 'E=':
                        anHomoE.append(spLine[4].replace('D', 'E'))
                    else:
                        anHomoE.append(spLine[3].split('=')[1].replace('D', 'E'))
                    break
                line = f.readline()
            f.close()
        for i in range(22, 33):
            f = open(fileList[i], 'r')
            line = f.readline()
            while line:
                if line.find('Total DFT') != -1:
                    spLine = line.split()
                    catDftE.append(spLine[4].replace('D', 'E'))
                    break
                line = f.readline()
            f.close()
        #Now we can put all the J^2 together
        f = open(jobName + '.dat', 'w')
        f.write('#Alpha Beta Gamma neutHomoE anHomoE neutDftE anDftE catDftE J^2(ev^2)\n')
        for i in range(0, 11):
            pOne = float(neutHomoE[i]) + float(catDftE[i]) - float(neutDftE[i])
            pTwo = float(anHomoE[i]) + float(neutDftE[i]) - float(anDftE[i])
            jSquared = 27.2116 ** 2 * (pOne ** 2 + pTwo ** 2)
            f.write(str(args.a[0]) + ' ' + str(1-args.a[0]) + ' ' + str(float(i)/20) + ' ' + neutHomoE[i] + ' '
                    + anHomoE[i] + ' ' + neutDftE[i] + ' ' + anDftE[i] + ' ' + catDftE[i] + ' '
                    + str(jSquared) + '\n')
        f.close()

except (KeyboardInterrupt):
  print ('Keyboard interrupt. Aborting')
  sys.exit()
