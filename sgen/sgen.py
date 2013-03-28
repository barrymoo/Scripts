#!/usr/bin/env python
import argparse
import os
import sys


username = os.environ['USER']
print('+++++++++++++++++++')
print(' Hello ' + username + '!')
print('+++++++++++++++++++')
# Parse and process command line arguments
parser = argparse.ArgumentParser(usage='./sgen.py infile [options]',
                                 description='SLURM Script Generator',
                                 prefix_chars='-')
parser.add_argument('infile', help='QM Job File-Supports' +
                    'NWChem, Gaussian, QChem, and ADF')
parser.add_argument('-np', metavar='N', type=int, nargs=1,
                    default=[1], help='Number of processors')
parser.add_argument('-t', metavar='N', type=int, nargs=1,
                    default=[2000], help='Job Time(hours)')
parser.add_argument('-add', metavar='F', type=str, nargs='*',
                    default=[0], help='Additional Job Files')
parser.add_argument('-g', action='store_true',
                    help='Submit to general compute nodes')
args = parser.parse_args()

#Set some variables
inpFile = args.infile
numProcs = args.np
jobTime = args.t
genComp = args.g
addFileList = args.add
fileName, fileExt = os.path.splitext(inpFile)
fileExt = fileExt.strip('.')
#Check to see if we get expected input file
if all(fileExt != x for x in ['nw', 'qc', 'com', 'adf']):
    print('This program expects an input file with' +
          'the extension: nw, adf, com, qc')
    sys.exit()

if fileExt == 'nw':
    tagfile = os.environ['NWCHEM_TAG_FILE']
    print('I found an NWChem input --> Processing')
    ofile = open(fileName+'.slurm', 'w')
    ofile.write('#!/bin/bash\n')
    if fileName == 'dplot':
        ofile.write('#SBATCH --time=1:00:00\n')
        ofile.write('#SBATCH --tasks-per-node=1\n')
    else:
        ofile.write('#SBATCH --time='+str(jobTime[0])+':00:00\n')
        ofile.write('#SBATCH --tasks-per-node='+str(numProcs[0])+'\n')
    ofile.write('#SBATCH --nodes=1\n')
    if genComp is not True:
        ofile.write('#SBATCH --partition=jochena\n')
        ofile.write('#SBATCH --account=pi-jochena\n')
    ofile.write('#SBATCH --job-name=' + fileName + '\n')
    ofile.write('#SBATCH --output=' + fileName + '.out\n')
    ofile.write('#SBATCH --mail-type=END\n')
    ofile.write('export SCRATCH_DIR=$SLURMTMPDIR\n')
    ofile.write('export NNODES=`srun -l hostname | uniq | wc -l`\n')
    ofile.write('export NPROCS=`srun -l hostname | wc -l`\n')
    ofile.write('export | grep SLURM\n')
    ofile.write('cd $SLURMTMPDIR\n')
    ofile.write('module load ' + tagfile + '\n')
    ofile.write('sbcast $SLURM_SUBMIT_DIR/' + inpFile +
                ' $SLURMTMPDIR/' + inpFile + '\n')
    if addFileList[0] != 0:
        for i in range(0, len(addFileList)):
            ofile.write('sbcast $SLURM_SUBMIT_DIR/' +
                        addFileList[i] + ' $SLURMTMPDIR/' +
                        addFileList[i]+'\n')
    ofile.write('mpirun -np $NPROCS $NWCHEM_EXEC ' + inpFile + '\n')
    ofile.write('cp -n *movecs $SLURM_SUBMIT_DIR\n')
    if fileName == 'dplot':
        print('Found dplot.nw --> Copying back cube files!')
        ofile.write('cp -n *cube $SLURM_SUBMIT_DIR\n')
    ofile.write('echo "All Done!"\n')
    ofile.write('echo "FINISHED AT " `date`\n')
elif fileExt == 'adf':
    print('I found an ADF input --> Processing')
    tagfile = os.environ['ADF_TAG_FILE']
    ofile = open(fileName + '.slurm', 'w')
    ofile.write('#!/bin/bash\n')
    ofile.write('#SBATCH --time=' + str(jobTime[0]) + ':00:00\n')
    ofile.write('#SBATCH --nodes=1\n')
    ofile.write('#SBATCH --tasks-per-node=' + str(numProcs[0]) + '\n')
    if genComp is not True:
        ofile.write('#SBATCH --partition=jochena\n')
        ofile.write('#SBATCH --account=pi-jochena\n')
    ofile.write('#SBATCH --job-name=' + fileName + '\n')
    ofile.write('#SBATCH --output=' + fileName + '.out\n')
    ofile.write('#SBATCH --mail-type=END\n')
    ofile.write('export SCRATCH_DIR=$SLURMTMPDIR\n')
    ofile.write('export I_MPI_DEBUG=4\n')
    ofile.write('export NNODES=`srun -l hostname | uniq | wc -l`\n')
    ofile.write('export NPROCS=`srun -l hostname | wc -l`\n')
    ofile.write('export NSCM=$NPROCS\n')
    ofile.write('export SCM_TMPDIR=$SLURMTMPDIR\n')
    ofile.write('export SCM_USETMPDIR=yes\n')
    ofile.write('HOSTFILE=$SLURMTMPDIR/hosts.$SLURM_JOB_ID\n')
    ofile.write('srun hostname -s > $HOSTFILE\n')
    ofile.write('export SCM_MACHINEFILE=$HOSTFILE\n')
    ofile.write('export | grep SLURM\n')
    ofile.write('export | grep SCM\n')
    ofile.write('cd $SLURMTMPDIR\n')
    ofile.write('export LC_COLLATE=C\n')
    ofile.write('source ' + tagfile + '\n')
    ofile.write('pwd\n')
    ofile.write('ulimit\n')
    ofile.write('date\n')
    ofile.write('pwd\n')
    ofile.write('sbcast $SLURM_SUBMIT_DIR/' + inpFile +
                ' $SLURMTMPDIR/' + inpFile + '\n')
    if addFileList[0] != 0:
        for i in range(0, len(addFileList)):
        #Check for TAPE21
            dummy, tape = os.path.splitext(addFileList[i])
            if tape == '.t21' or dummy == 'TAPE21':
                print('Found TAPE21!')
                ofile.write('sbcast $SLURM_SUBMIT_DIR/' + addFileList[i] +
                            ' $SLURMTMPDIR/TAPE21\n')
            else:
                ofile.write('sbcast $SLURM_SUBMIT_DIR/' + addFileList[i] +
                            ' $SLURMTMPDIR/' + addFileList[i] + '\n')
    ofile.write('$ADFBIN/adf < ' + inpFile + '\n')
    ofile.write('mv TAPE21 $SLURM_SUBMIT_DIR/' + fileName + '.t21\n')
elif fileExt == 'com':
    print('I found a Gaussian input --> Processing')
    tagfile = os.environ['G09_TAG_FILE']
    ofile = open(fileName+'.slurm', 'w')
    ofile.write('#!/bin/bash\n')
    ofile.write('#SBATCH --time='+str(jobTime[0])+':00:00\n')
    ofile.write('#SBATCH --nodes=1\n')
    ofile.write('#SBATCH --tasks-per-node='+str(numProcs[0])+'\n')
    if genComp is not True:
        ofile.write('#SBATCH --partition=jochena\n')
        ofile.write('#SBATCH --account=pi-jochena\n')
    ofile.write('#SBATCH --job-name=' + fileName + '\n')
    ofile.write('#SBATCH --output=' + fileName + '.out\n')
    ofile.write('#SBATCH --mail-type=END\n')
    ofile.write('export SCRATCH_DIR=$SLURMTMPDIR\n')
    ofile.write('export NNODES=`srun -l hostname | uniq | wc -l`\n')
    ofile.write('export NPROCS=`srun -l hostname | wc -l`\n')
    ofile.write('export | grep SLURM\n')
    ofile.write('cd $SLURMTMPDIR\n')
    ofile.write('export LC_COLLATE=C\n')
    ofile.write('source ' + tagfile + '\n')
    ofile.write('export LD_LIBRARY_PATH=/util/pgi/linux86/6.1' +
                '/lib:$LD_LIBRARY_PATH\n')
    ofile.write('sbcast $SLURM_SUBMIT_DIR/' + inpFile +
                ' $SLURMTMPDIR/' + inpFile + '\n')
    ofile.write('g09 < ' + inpFile + '\n')
elif fileExt == 'qc':
    print('I found a QChem input --> Processing')
    tagfile = os.environ['QCHEM_TAG_FILE']
    ofile = open(fileName+'.slurm', 'w')
    ofile.write('#!/bin/bash\n')
    ofile.write('#SBATCH --time=' + str(jobTime[0]) + ':00:00\n')
    ofile.write('#SBATCH --nodes=1\n')
    ofile.write('#SBATCH --tasks-per-node=' + str(numProcs[0]) + '\n')
    if genComp is not True:
        ofile.write('#SBATCH --partition=jochena\n')
        ofile.write('#SBATCH --account=pi-jochena\n')
    ofile.write('#SBATCH --job-name=' + fileName + '\n')
    ofile.write('#SBATCH --output=' + fileName + '.out\n')
    ofile.write('#SBATCH --mail-type=END\n')
    ofile.write('export SCRATCH_DIR=$SLURMTMPDIR\n')
    ofile.write('export NNODES=`srun -l hostname | uniq | wc -l`\n')
    ofile.write('export NPROCS=`srun -l hostname | wc -l`\n')
    ofile.write('export | grep SLURM\n')
    ofile.write('cd $SLURMTMPDIR\n')
    ofile.write('. $MODULESHOME/init/sh\n')
    ofile.write('module load ' + tagfile + '\n')
    ofile.write('module list\n')
    ofile.write('ulimit -s unlimited\n')
    ofile.write('export QCLOCALSCR=$SLURMTMPDIR\n')
    ofile.write('export PBS_NODEFILE=nodelist.$$\n')
    ofile.write('srun --nodes=${SLURM_NNODES} bash -c \'hostname\' ' +
                '> $PBS_NODEFILE\n')
    ofile.write('cat $PBS_NODEFILE\n')
    ofile.write('sbcast $SLURM_SUBMIT_DIR/' + inpFile +
                ' $SLURMTMPDIR/' + inpFile + '\n')
    ofile.write('qchem -pbs -np $NPROCS ' + inpFile + '\n')

ofile.close()
