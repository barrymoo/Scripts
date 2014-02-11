#!/usr/bin/env python
import argparse
from os import environ
from os.path import splitext
from sys import exit

#To Do
# 1) Add Tinker SLURM Generator
# 2) Add Quantum Espresso SLURM Generator (Maybe Better as Separate CPMD Script?)


def split_filename_ext(inpFile):
    fileName, fileExt = splitext(inpFile)
    return fileName, fileExt


def write_header(inpFile, numNodes, numProcs, jobTime, genComp, ofile):
    fileName, fileExt = split_filename_ext(inpFile)
    ofile.write('#!/bin/bash\n')
    if fileName == 'dplot':
        ofile.write('#SBATCH --nodes=1\n')
        ofile.write('#SBATCH --time=1:00:00\n')
        ofile.write('#SBATCH --ntasks-per-node=1\n')
    else:
        ofile.write('#SBATCH --nodes=' + str(numNodes) + '\n')
        ofile.write('#SBATCH --time='+str(jobTime)+':00:00\n')
        ofile.write('#SBATCH --ntasks-per-node=' + str(numProcs) + '\n')
    if genComp is not True:
        ofile.write('#SBATCH --cluster=chemistry\n')
        ofile.write('#SBATCH --partition=jochena\n')
        ofile.write('#SBATCH --account=pi-jochena\n')
    else:
        ofile.write('#SBATCH --account=jochena\n')
        ofile.write('#SBATCH --partition=general-compute\n')
        ofile.write('#SBATCH --cluster=ub-hpc\n')
    ofile.write('#SBATCH --job-name=' + fileName + '\n')
    ofile.write('#SBATCH --output=' + fileName + '.out\n')
    ofile.write('#SBATCH --mail-type=END\n')

def set_variables_export_variables(ofile):
    ofile.write('export SCRATCH_DIR=$SLURMTMPDIR\n')
    ofile.write('export NNODES=$SLURM_NNODES`\n')
    ofile.write('export NPROCS=$SLURM_NTASK_PER_NODE`\n')
    ofile.write('export | grep SLURM\n')


#Just a quick hello
username = environ['USER']
print('+++++++++++++++++++')
print(' Hello ' + username + '!')
print('+++++++++++++++++++')

# Parse and process command line arguments
parser = argparse.ArgumentParser(usage='./sgen.py infile [options]',
                                 description='SLURM Script Generator',
                                 prefix_chars='-')
parser.add_argument('infile', help='QM Job File-Supports ' +
                    'NWChem, Gaussian, QChem, and ADF')
parser.add_argument('-nn', '--nnodes', metavar='N', type=int, nargs=1,
                    default=[1], help='Number of Nodes')
parser.add_argument('-np', '--nprocs', metavar='N', type=int, nargs=1,
                    default=[1], help='Number of Processors')
parser.add_argument('-t', '--time', metavar='N', type=int, nargs=1,
                    default=[0], help='Job Time(hours)')
parser.add_argument('-g', '--general', action='store_true',
                    help='Submit to general compute nodes')
parser.add_argument('-e', '--empty', action='store_true',
                    help='Write empty script and exit')
parser.add_argument('-a', '--add', metavar='F', type=str, nargs='*',
                    default=[0], help='Additional Job Files (Must be last option!)')
args = parser.parse_args()

#Set some variables
inpFile = args.infile
fileName, fileExt = split_filename_ext(inpFile)
numProcs = args.nprocs[0]
numNodes = args.nnodes[0]
jobTime = args.time[0]
genComp = args.general
emptyScript = args.empty
addFileList = args.add

#Check for general compute
if jobTime == 0 and genComp == True:
    jobTime = 72
elif jobTime == 0 and genComp == False:
    jobTime = 2000

#Print empty script for user
if emptyScript:
    print('Printing SLURM headers and exiting')
    fileName, fileExt = split_filename_ext(inpFile)
    ofile = open(fileName+'.slurm', 'w')
    write_header(inpFile, numProcs, jobTime, genComp, ofile)    
    set_variables_export_variables(ofile)
    ofile.close()
    exit()

#Check to see if we get expected input file
if all(fileExt != x for x in ['.nw', '.qc', '.com', '.adf']):
    print('This program expects an input file with' +
          'the extension: nw, adf, com, qc')
    exit()

#NWChem SLURM Generation
if fileExt == 'nw':
    tagfile = os.environ['NWCHEM_TAG_FILE']
    print('I found an NWChem input --> Processing')
    ofile = open(fileName+'.slurm', 'w')
    write_header(inpFile, numProcs, jobTime, genComp, ofile)    
    set_variables_export_variables(ofile)
    ofile.write('cd $SLURMTMPDIR\n')
    ofile.write('source ' + tagfile + '\n')
    ofile.write('sbcast $SLURM_SUBMIT_DIR/' + inpFile +
                ' $SLURMTMPDIR/' + inpFile + '\n')
    if addFileList[0] != 0:
        for i in addFileList:
            ofile.write('sbcast $SLURM_SUBMIT_DIR/' + i + ' $SLURMTMPDIR/' + i + '\n')
    ofile.write('mpiexec.hydra -nn $SLURM_NNODES -np $SLURM_NTASKS_PER_NODE $NWCHEM_EXEC $SLURMTMPDIR/' + inpFile + '\n')
    ofile.write('cp *movecs $SLURM_SUBMIT_DIR\n')
    ofile.write('cp *civecs* $SLURM_SUBMIT_DIR\n')
    ofile.write('cp *gen $SLURM_SUBMIT_DIR\n')
    if fileName == 'dplot':
        print('Found dplot.nw --> Copying back cube files!')
        ofile.write('cp *cube $SLURM_SUBMIT_DIR\n')
    ofile.write('echo "All Done!"\n')
    ofile.write('echo "FINISHED AT " `date`\n')
#ADF SLURM Generation    
elif fileExt == 'adf':
    tagfile = os.environ['ADF_TAG_FILE']
    print('I found an ADF input --> Processing')
    ofile = open(fileName + '.slurm', 'w')
    write_header(inpFile, numProcs, jobTime, genComp, ofile)    
    set_variables_export_variables(ofile)
    ofile.write('export I_MPI_DEBUG=4\n')
    ofile.write('HOSTFILE=$SLURMTMPDIR/hosts.$SLURM_JOB_ID\n')
    ofile.write('srun hostname -s > $HOSTFILE\n')
    ofile.write('export SCM_MACHINEFILE=$HOSTFILE\n')
    ofile.write('export | grep SCM\n')
    ofile.write('cd $SLURMTMPDIR\n')
    ofile.write('export LC_COLLATE=C\n')
    ofile.write('source ' + tagfile + '\n')
    ofile.write('export SCM_TMPDIR=$SLURMTMPDIR\n')
    ofile.write('export NSCM=$NPROCS\n')
    ofile.write('export SCM_USETMPDIR=yes\n')
    ofile.write('ulimit\n')
    ofile.write('sbcast $SLURM_SUBMIT_DIR/' + inpFile +
                ' $SLURMTMPDIR/' + inpFile + '\n')
    if addFileList[0] != 0:
        for i in addFileList:
        #Check for TAPE21
            dummy, tape = split_filename_ext(i)
            if tape == '.t21' or dummy == 'TAPE21':
                print('Found TAPE21!')
                ofile.write('sbcast $SLURM_SUBMIT_DIR/' + i + ' $SLURMTMPDIR/TAPE21\n')
            else:
                ofile.write('sbcast $SLURM_SUBMIT_DIR/' + i + ' $SLURMTMPDIR/' + i + '\n')
    ofile.write('$ADFBIN/adf < ' + inpFile + '\n')
    ofile.write('mv TAPE21 $SLURM_SUBMIT_DIR/' + fileName + '.t21\n')
elif fileExt == 'com':
    tagfile = os.environ['G09_TAG_FILE']
    print('I found a Gaussian input --> Processing')
    ofile = open(fileName+'.slurm', 'w')
    write_header(inpFile, numProcs, jobTime, genComp, ofile)    
    set_variables_export_variables(ofile)
    ofile.write('cd $SLURMTMPDIR\n')
    ofile.write('export LC_COLLATE=C\n')
    ofile.write('source ' + tagfile + '\n')
    ofile.write('export LD_LIBRARY_PATH=/util/pgi/linux86/6.1' +
                '/lib:$LD_LIBRARY_PATH\n')
    ofile.write('sbcast $SLURM_SUBMIT_DIR/' + inpFile +
                ' $SLURMTMPDIR/' + inpFile + '\n')
    ofile.write('g09 < ' + inpFile + '\n')
    ofile.write('cp *chk $SLURM_SUBMIT_DIR\n')
elif fileExt == 'qc':
    tagfile = os.environ['QCHEM_TAG_FILE']
    print('I found a QChem input --> Processing')
    ofile = open(fileName+'.slurm', 'w')
    write_header(inpFile, numProcs, jobTime, genComp, ofile)    
    set_variables_export_variables(ofile)
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
