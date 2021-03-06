#!/usr/bin/env python
import os
import glob
import sys
import argparse
import time

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
    parser.add_argument('-np', metavar='N', type=int, nargs=1, default=[4],
                        help='Number of processors, default=4')
    parser.add_argument('-t', metavar='N', type=int, nargs=1, default=[2],
                        help='Job Time(hours), default=2')
    parser.add_argument('-nosub', action='store_false',
                        help='Generate NW/SLURM files but don\'t submit jobs')
    parser.add_argument('-sub', action='store_true',
                        help='After -nosub, this keyword will submit jobs')
    parser.add_argument('-d', metavar='N', type=int, nargs=1, default=[1],
                        help='Dimension of Tuning, default=1')
    parser.add_argument('-top', action='store_true', 
                        help='When specified with 2 dimensional tuning, submits the top half of tuning grid')
    parser.add_argument('-a', metavar='N', type=float, nargs=1, default=[0.],
                        help='Alpha Parameter (has no effect is -d is 2, default=0.0')
    parser.add_argument('-clean', action='store_true',
                        help='clean directory of *tuning*slurm, *tuning*nw, and *tuning*out and exit')
    parser.add_argument('-cancel', action='store_true',
                        help='Cancel any running tuning jobs')
    args = parser.parse_args()

    #Lets make sure everything is there!
    inpFile=args.inpFile
    fileName, fileExt = os.path.splitext(inpFile)
    jobName = fileName.split('-')[0]
    fileList = []
    fileList.append(jobName + '-neutral.nw')
    fileList.append(jobName + '-anion.nw')
    fileList.append(jobName + '-cation.nw')
    fileList.append(jobName + '-neutral-pbe0.movecs')
    fileList.append(jobName + '-anion-pbe0.movecs')
    fileList.append(jobName + '-cation-pbe0.movecs')
    if args.d[0] == 2 and args.top:
        for i in range(0, 11):
            gamma = str(float(i)/20)
            fileList.append(jobName + '-neutral-tuning-0.5-' + gamma + '.movecs')
            fileList.append(jobName + '-cation-tuning-0.5-' + gamma + '.movecs')
            fileList.append(jobName + '-anion-tuning-0.5-' + gamma + '.movecs')

    if args.clean:
        print('Cleaning up *tuning*slurm, *tuning*nw, and *tuning*out...exiting')
        os.system('rm *tuning*nw *tuning*slurm *tuning*out *tuning*movecs')
        sys.exit()

    if args.cancel:
        print('Cancelling current running tuning jobs')
        os.system('squeue -u ' + username + ' | grep ' + jobName[0:8] + ' | awk \'{print $1}\'  > jobList.dat')
        delJobs = open('jobList.dat', 'r').readlines()
        for i in delJobs:
            os.system('scancel ' + i)
        os.system('rm jobList.dat')
        sys.exit()

    if args.sub:
        if args.top and args.d[2] == 2:
            print ('Submitting potentially modified SLURM jobs')
            for files in glob.glob('*tuning-0.[56789]-*.slurm'):
                os.system('sbatch ' + files)
            for files in glob.glob('*tuning-1.0-*.slurm'):
                os.system('sbatch ' + files)
            sys.exit()
        else:    
            print ('Submitting potentially modified SLURM jobs')
            for files in glob.glob('*tuning*.slurm'):
                os.system('sbatch ' + files)
            sys.exit()

    if not all(os.path.isfile(x) for x in fileList):
        print('Error: You don\'t have the necessary files to run this script!')
        sys.exit()

    #Now we can start generating tuning jobs!
    neutLines = open(fileList[0], 'r').readlines()
    anLines = open(fileList[1], 'r').readlines()
    catLines = open(fileList[2], 'r').readlines()
    #We need to get the data in a format to easily sub in ALPHA BETA GAMMA
    for i in range(0, len(neutLines)):
        if neutLines[i].find('start') != -1:
            neutLines[i] = 'start ' + jobName + '-neutral-tuning-ALPHA-GAMMA\n'
        elif neutLines[i].find('title') != -1:
            neutLines[i] = 'title ' + jobName + '-neutral-tuning-ALPHA-GAMMA\n'
        elif neutLines[i].find('task scf') != -1:
            neutLines[i] = '#' + neutLines[i]
        elif neutLines[i].find('xc pbe0') != -1:
            neutLines[i] = 'xc xcampbe96 1.0 cpbe96 1.0 HFexch 1.0\ncam GAMMA cam_alpha ALPHA cam_beta BETA\n'
        elif neutLines[i].find('convergence') != -1:
            neutLines[i] = neutLines[i].strip() + ' nolevelshifting\n'
        elif neutLines[i].find('vectors input ' + jobName) != -1:
            if args.top and args.d[0] == 2:
                neutLines[i] = 'vectors input ' + jobName + '-neutral-tuning-0.5-GAMMA.movecs output ' + jobName + '-neutral-tuning-ALPHA-GAMMA.movecs\n'
            else:
                neutLines[i] = 'vectors input ' + jobName + '-neutral-pbe0.movecs output ' + jobName + '-neutral-tuning-ALPHA-GAMMA.movecs\n'
    for i in range(0, len(anLines)):
        if anLines[i].find('start') != -1:
            anLines[i] = 'start ' + jobName + '-anion-tuning-ALPHA-GAMMA\n'
        elif anLines[i].find('title') != -1:
            anLines[i] = 'title ' + jobName + '-anion-tuning-ALPHA-GAMMA\n'
        elif anLines[i].find('task scf') != -1:
            anLines[i] = '#' + anLines[i]
        elif anLines[i].find('xc pbe0') != -1:
            anLines[i] = 'xc xcampbe96 1.0 cpbe96 1.0 HFexch 1.0\ncam GAMMA cam_alpha ALPHA cam_beta BETA\n'
        elif anLines[i].find('convergence') != -1:
            anLines[i] = anLines[i].strip() + ' nolevelshifting\n'
        elif anLines[i].find('vectors input ' + jobName) != -1:
            if args.top and args.d[0] == 2:
                anLines[i] = 'vectors input ' + jobName + '-anion-tuning-0.5-GAMMA.movecs output ' + jobName + '-anion-tuning-ALPHA-GAMMA.movecs\n'
            else:
                anLines[i] = 'vectors input ' + jobName + '-anion-pbe0.movecs output ' + jobName + '-anion-tuning-ALPHA-GAMMA.movecs\n'
    for i in range(0, len(catLines)):
        if catLines[i].find('start') != -1:
            catLines[i] = 'start ' + jobName + '-cation-tuning-ALPHA-GAMMA\n'
        elif catLines[i].find('title') != -1:
            catLines[i] = 'title ' + jobName + '-cation-tuning-ALPHA-GAMMA\n'
        elif catLines[i].find('task scf') != -1:
            catLines[i] = '#' + catLines[i]
        elif catLines[i].find('xc pbe0') != -1:
            catLines[i] = 'xc xcampbe96 1.0 cpbe96 1.0 HFexch 1.0\ncam GAMMA cam_alpha ALPHA cam_beta BETA\n'
        elif catLines[i].find('convergence') != -1:
            catLines[i] = catLines[i].strip() + ' nolevelshifting\n'
        elif catLines[i].find('vectors input ' + jobName) != -1:
            if args.top and args.d[0] == 2:
                catLines[i] = 'vectors input ' + jobName + '-cation-tuning-0.5-GAMMA.movecs output ' + jobName + '-cation-tuning-ALPHA-GAMMA.movecs\n'
            else:
                catLines[i] = 'vectors input ' + jobName + '-cation-pbe0.movecs output ' + jobName + '-cation-tuning-ALPHA-GAMMA.movecs\n'

    #Now let's generate tuning jobs
    if args.d[0] == 2:
        if args.top:
            print('2D tuning detected (upper grid), generating jobs')
            for i in range(6, 11):
                for j in range(0, 11):
                    alpha = str(float(i)/10)
                    beta = '{:.2f}'.format(1.0 - float(alpha))
                    gamma = str(float(j)/20)
                    nwFileList = []
                    nwFileList.append(jobName + '-neutral-tuning-' + alpha + '-' + gamma + '.nw')
                    nwFileList.append(jobName + '-anion-tuning-' + alpha + '-' + gamma + '.nw')
                    nwFileList.append(jobName + '-cation-tuning-' + alpha + '-' + gamma + '.nw')
                    if not os.path.isfile(nwFileList[0]):
                        print(nwFileList[0], 'is not a file. Check outputs!..exiting')
                        sys.exit()
                    if not os.path.isfile(nwFileList[1]):
                        print(nwFileList[1], 'is not a file. Check outputs!..exiting')
                        sys.exit()
                    if not os.path.isfile(nwFileList[2]):
                        print(nwFileList[2], 'is not a file. Check outputs!..exiting')
                        sys.exit()
                    tmpNeutLines = [w.replace('ALPHA', alpha) for w in neutLines]
                    tmpAnLines = [w.replace('ALPHA', alpha) for w in anLines]
                    tmpCatLines = [w.replace('ALPHA', alpha) for w in catLines]
                    tmpNeutLines = [w.replace('BETA', beta) for w in tmpNeutLines]
                    tmpAnLines = [w.replace('BETA', beta) for w in tmpAnLines]
                    tmpCatLines = [w.replace('BETA', beta) for w in tmpCatLines]
                    tmpNeutLines = [w.replace('GAMMA', gamma) for w in tmpNeutLines]
                    tmpAnLines = [w.replace('GAMMA', gamma) for w in tmpAnLines]
                    tmpCatLines = [w.replace('GAMMA', gamma) for w in tmpCatLines]
                    fNeut = open(nwFileList[0], 'w')
                    fAn = open(nwFileList[1], 'w')
                    fCat = open(nwFileList[2], 'w')
                    for j in tmpNeutLines:
                        fNeut.write(j)
                    for j in tmpAnLines:
                        fAn.write(j)
                    for j in tmpCatLines:
                        fCat.write(j)
                    fNeut.close()
                    fAn.close()
                    fCat.close()
                    #Now lets sgen
                    os.system('sgen.py -g -np ' + str(args.np[0]) + ' -t ' + str(args.t[0]) + ' ' + nwFileList[0] + ' -add ' +
                              jobName + '-neutral-tuning-0.5-' + gamma + '.movecs > /dev/null')
                    os.system('sgen.py -g -np ' + str(args.np[0]) + ' -t ' + str(args.t[0]) + ' ' + nwFileList[1] + ' -add ' +
                              jobName + '-anion-tuning-0.5-' + gamma + '.movecs > /dev/null')
                    os.system('sgen.py -g -np ' + str(args.np[0]) + ' -t ' + str(args.t[0]) + ' ' + nwFileList[2] + ' -add ' +
                              jobName + '-cation-tuning-0.5-' + gamma + '.movecs > /dev/null')
        else:
            print('2D Tuning detected (lower grid), generating jobs')
            for i in range(0, 6):
                for j in range(0, 11):
                    alpha = str(float(i)/10)
                    beta = '{:.2f}'.format(1.0 - float(alpha))
                    gamma = str(float(j)/20)
                    nwFileList = []
                    nwFileList.append(jobName + '-neutral-tuning-' + alpha + '-' + gamma + '.nw')
                    nwFileList.append(jobName + '-anion-tuning-' + alpha + '-' + gamma + '.nw')
                    nwFileList.append(jobName + '-cation-tuning-' + alpha + '-' + gamma + '.nw')
                    tmpNeutLines = [w.replace('ALPHA', alpha) for w in neutLines]
                    tmpAnLines = [w.replace('ALPHA', alpha) for w in anLines]
                    tmpCatLines = [w.replace('ALPHA', alpha) for w in catLines]
                    tmpNeutLines = [w.replace('BETA', beta) for w in tmpNeutLines]
                    tmpAnLines = [w.replace('BETA', beta) for w in tmpAnLines]
                    tmpCatLines = [w.replace('BETA', beta) for w in tmpCatLines]
                    tmpNeutLines = [w.replace('GAMMA', gamma) for w in tmpNeutLines]
                    tmpAnLines = [w.replace('GAMMA', gamma) for w in tmpAnLines]
                    tmpCatLines = [w.replace('GAMMA', gamma) for w in tmpCatLines]
                    fNeut = open(nwFileList[0], 'w')
                    fAn = open(nwFileList[1], 'w')
                    fCat = open(nwFileList[2], 'w')
                    for j in tmpNeutLines:
                        fNeut.write(j)
                    for j in tmpAnLines:
                        fAn.write(j)
                    for j in tmpCatLines:
                        fCat.write(j)
                    fNeut.close()
                    fAn.close()
                    fCat.close()
                    #Now lets sgen
                    os.system('sgen.py -g -np ' + str(args.np[0]) + ' -t ' + str(args.t[0]) + ' ' + nwFileList[0] +
                              ' -add ' + jobName + '-neutral-pbe0.movecs > /dev/null')
                    os.system('sgen.py -g -np ' + str(args.np[0]) + ' -t ' + str(args.t[0]) + ' ' + nwFileList[1] +
                              ' -add ' + jobName + '-anion-pbe0.movecs > /dev/null')
                    os.system('sgen.py -g -np ' + str(args.np[0]) + ' -t ' + str(args.t[0]) + ' ' + nwFileList[2] +
                              ' -add ' + jobName + '-cation-pbe0.movecs > /dev/null')
                    
    else:
        print('1D Tuning detected, generating jobs')
        alpha = str(args.a[0])
        beta = str(1. - float(args.a[0]))
        #We only need to do this replacement once!!
        neutLines = [w.replace('ALPHA', alpha) for w in neutLines]
        neutLines = [w.replace('BETA', beta) for w in neutLines]
        anLines = [w.replace('ALPHA', alpha) for w in anLines]
        anLines = [w.replace('BETA', beta) for w in anLines]
        catLines = [w.replace('ALPHA', alpha) for w in catLines]
        catLines = [w.replace('BETA', beta) for w in catLines]
        #Lets write the tuning jobs!
        for i in range(0, 11):
            gamma = str(float(i)/20)
            nwFileList = []
            nwFileList.append(jobName + '-neutral-tuning-' + alpha + '-' + gamma + '.nw')
            nwFileList.append(jobName + '-anion-tuning-' + alpha + '-' + gamma + '.nw')
            nwFileList.append(jobName + '-cation-tuning-' + alpha + '-' + gamma + '.nw')
            tmpNeutLines = [w.replace('GAMMA', gamma) for w in neutLines]
            tmpAnLines = [w.replace('GAMMA', gamma) for w in anLines]
            tmpCatLines = [w.replace('GAMMA', gamma) for w in catLines]
            fNeut = open(nwFileList[0], 'w')
            fAn = open(nwFileList[1], 'w')
            fCat = open(nwFileList[2], 'w')
            for j in tmpNeutLines:
                fNeut.write(j)
            for j in tmpAnLines:
                fAn.write(j)
            for j in tmpCatLines:
                fCat.write(j)
            fNeut.close()
            fAn.close()
            fCat.close()
        #Time to sgen :)
        for i in range(0, 11):
            gamma = str(float(i)/20)
            nwFileList = []
            nwFileList.append(jobName + '-neutral-tuning-' + alpha + '-' + gamma + '.nw')
            nwFileList.append(jobName + '-anion-tuning-' + alpha + '-' + gamma + '.nw')
            nwFileList.append(jobName + '-cation-tuning-' + alpha + '-' + gamma + '.nw')
            os.system('sgen.py -g -np ' + str(args.np[0]) + ' -t ' + str(args.t[0]) + ' ' + nwFileList[0] + ' -add ' + jobName + '-neutral-pbe0.movecs > /dev/null')
            os.system('sgen.py -g -np ' + str(args.np[0]) + ' -t ' + str(args.t[0]) + ' ' + nwFileList[1] + ' -add ' + jobName + '-anion-pbe0.movecs > /dev/null')
            os.system('sgen.py -g -np ' + str(args.np[0]) + ' -t ' + str(args.t[0]) + ' ' + nwFileList[2] + ' -add ' + jobName + '-cation-pbe0.movecs > /dev/null')

    if not args.nosub:
        print ('NOSUB detected --> NW/Slurm files generated...exiting')
        sys.exit()
    else:
        if args.d[0] == 2:
            if args.top:
                for i in range(6, 11):
                    for j in range(0, 11):
                        alpha = str(float(i)/10)
                        gamma = str(float(j)/20)
                        os.system('sbatch ' + jobName + '-neutral-tuning-' + alpha + '-' + gamma + '.slurm')
                        os.system('sbatch ' + jobName + '-anion-tuning-' + alpha + '-' + gamma + '.slurm') 
                        os.system('sbatch ' + jobName + '-cation-tuning-' + alpha + '-' + gamma + '.slurm') 
            else:    
                print('Submitting 2D SLURM Tuning Jobs!')
                for i in range(0, 6):
                    for j in range(0, 11):
                        alpha = str(float(i)/10)
                        gamma = str(float(j)/20)
                        os.system('sbatch ' + jobName + '-neutral-tuning-' + alpha + '-' + gamma + '.slurm')
                        os.system('sbatch ' + jobName + '-anion-tuning-' + alpha + '-' + gamma + '.slurm') 
                        os.system('sbatch ' + jobName + '-cation-tuning-' + alpha + '-' + gamma + '.slurm') 
        else:
            print('Submitting 1D SLURM Tuning Jobs!')
            for i in range(0, 11):
                gamma = str(float(i)/20)
                slurmFileList = []
                slurmFileList.append(jobName + '-neutral-tuning-' + alpha + '-' + gamma + '.slurm')
                slurmFileList.append(jobName + '-anion-tuning-' + alpha + '-' + gamma + '.slurm')
                slurmFileList.append(jobName + '-cation-tuning-' + alpha + '-' + gamma + '.slurm')
                for j in slurmFileList:
                    os.system('sbatch ' + j)

except (KeyboardInterrupt):
  print ('Keyboard interrupt. Aborting')
  sys.exit()
