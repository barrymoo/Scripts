#!/usr/bin/env python
''' sgen.py -- A slurm script generator
Usage:
    sgen.py (-i <file> | --input <file>) [-hvgdy] [-p <value>] [-n <value>] [-t <value>] [-m <value>] [-a <file>...]

Positional Arguments:
    -i, --input <file>              QM input file, supports ADF, NWChem, Gaussian, QChem

Options:
    -h --help                       Print this screen and exit
    -v --version                    Print the version of sgen.py
    -g --general-compute            Submit job to general-compute on ub-hpc cluster
                                        default submit to jochena/ezurek on chemistry cluster
    -y --industry                   Submit job to scavenger on industry cluster
    -d --debug                      Submit job to debug on ub-hpc cluster
                                        default submit to jochena/ezurek on chemistry cluster
    -p --num-procs <value>          Number of processors job will run on (default = 1)
    -n --num-nodes <value>          Number of processors job will run on (default = 1)
    -t --time <value>               How long would you like the job to run (in hours)
                                        max allowed time is default, see below:
                                            general-compute = 72
                                            debug = 1
                                            chemistry = 2000
    -a --add <file>...              Additional to copy to $SLURMTMPDIR
    -m --memory <value>             Specify the memory for your job (in MB, defaults are complicated)
'''


def write_slurm_header(f, args, basename, group):
    '''
    Write the generic slurm header to filestream f with dict args
    '''
    # First write the generic stuff
    f.write('#!/bin/bash\n')
    f.write('#SBATCH --time={0}:00:00\n'.format(args['--time']))
    f.write('#SBATCH --nodes={0}\n'.format(args['--num-nodes']))
    f.write('#SBATCH --ntasks-per-node={0}\n'.format(args['--num-procs']))
    f.write('#SBATCH --job-name={0}\n'.format(basename))
    f.write('#SBATCH --output={0}.out\n'.format(basename))
    f.write('#SBATCH --mail-type=END\n')
    f.write('#SBATCH --mem={}\n'.format(arguments['--memory']))

    # cluster logic
    ext = splitext(arguments['--input'])[-1]
    if args['--general-compute']:
        f.write('#SBATCH --cluster=ub-hpc\n')
        f.write('#SBATCH --partition=general-compute\n')
    elif args['--debug']:
        f.write('#SBATCH --cluster=ub-hpc\n')
        f.write('#SBATCH --partition=debug\n')
    elif args['--industry']:
        f.write('#SBATCH --cluster=industry\n')
        f.write('#SBATCH --partition=scavenger\n')
    else:
        f.write('#SBATCH --cluster=chemistry\n')
        f.write('#SBATCH --partition={0}\n'.format(group))

def write_slurm_generic_environment(f):
    '''
    This functions prints the generic slurm environment variables to the slurm file
    '''
    f.write('export | grep SLURM\n')
    f.write('export SCRATCH_DIR=$SLURMTMPDIR\n')
    f.write('cd $SLURMTMPDIR\n')
    f.write('ulimit -s unlimited\n')
    f.write('export LC_COLLATE=C\n')

def write_adf_environment(f, tag):
    '''
    This function prints the adf specific options to the slurm file
    '''
    f.write('export NSCM=$NPROCS\n')
    f.write('export SCM_TMPDIR=$SLURMTMPDIR\n')
    f.write('export SCM_USETMPDIR=yes\n')
    f.write('HOSTFILE=$SLURMTMPDIR/hosts.$SLURM_JOB_ID\n')
    f.write('srun hostname -s > $HOSTFILE\n')
    f.write('export SCM_MACHINEFILE=$HOSTFILE\n')
    f.write('source {0}\n'.format(tag))
    f.write('export SCM_TMPDIR=$SLURMTMPDIR\n')
    write_slurm_generic_environment(f)
    f.write('$ADFBIN/adf < $SLURM_JOB_NAME.adf\n')
    f.write('cp $SLURMTMPDIR/TAPE21 $SLURM_SUBMIT_DIR/$SLURM_JOB_NAME.t21\n')

def write_nwchem_environment(f, tag):
    '''
    This function prints the nwchem specific options to the slurm file
    '''
    f.write('source {0}\n'.format(tag))
    write_slurm_generic_environment(f)
    f.write('export I_MPI_PMI_LIBRARY=/usr/lib64/libpmi.so\n')
    f.write('srun -n $SLURM_NPROCS $NWCHEM_EXEC $SLURM_JOB_NAME.nw\n')
    f.write('cp $SLURM_JOB_NAME.movecs $SLURM_JOB_NAME.civecs* $SLURM_JOB_NAME.gen *.cube $SLURM_SUBMIT_DIR\n')

def write_g09_environment(f, tag):
    '''
    This function prints the gaussian specific options to the slurm file
    '''
    f.write('source {0}\n'.format(tag))
    write_slurm_generic_environment(f)
    f.write('export LD_LIBRARY_PATH=/util/pgi/linux86/6.1/lib:$LD_LIBRARY_PATH\n')
    f.write('g09 < $SLURM_JOB_NAME.g09\n')
    f.write('cp $SLURM_JOB_NAME.chk $SLURM_SUBMIT_DIR\n')

def write_qchem_environment(f, tag):
    '''
    This function prints the qchem specific options to the slurm file
    '''
    f.write('module purge')
    f.write('module load {0}\n'.format(tag))
    write_slurm_generic_environment(f)
    f.write('export QCSCRATCH=$SLURMTMPDIR\n')
    f.write('export I_MPI_PMI_LIBRARY=/usr/lib64/libpmi.so\n')
    f.write('qchem-srun -nt $SLURM_NPROCS $QCPROG $SLURM_JOB_NAME.qc $SLURM_JOB_NAME.out\n')
    f.write('cp $SLURM_JOB_NAME.out $SLURM_SUBMIT_DIR\n')

def write_sbcast_directives(f, args):
    '''
    This function prints the sbcast directives to the slurm file
    '''
    f.write('sbcast $SLURM_SUBMIT_DIR/{0} $SLURMTMPDIR/{0}\n'.format(args['--input']))
    for i in args['--add']:
        f.write('sbcast $SLURM_SUBMIT_DIR/{0} $SLURMTMPDIR/{0}\n'.format(i))


try:
    # Some imports functions and libraries
    from docopt import docopt
    from os import environ
    from os.path import isfile, splitext
    from subprocess import check_output
    from sys import exit

    # Using docstring above, parse the arguments with docopt
    arguments = docopt(__doc__, version='sgen.py version 0.0.1')

    # Input check 1: does the input file exist and have a compatible file extension ['.adf', '.nw', '.g09', '.qc']
    #   No reason to continue otherwise
    while True:
        if not isfile(arguments['--input']) or splitext(arguments['--input']) in ['.adf', '.nw', '.g09', '.qc']:
                arguments['--input'] = input(("'{0}' does not exist or does not have the expected file extension!\n"
                    + "Please enter an appropriate input file to continue, or try '-h/--help' option:\n").format(arguments['--input']))
        else:
            # Environment Variable Check 1: using file extension, let's see if we can find the TAG environment variables
            ext = splitext(arguments['--input'])[-1]
            try:
                if ext == '.adf':
                    tag = environ['ADF_TAG_FILE']
                elif ext == '.nw':
                    tag = environ['NWCHEM_TAG_FILE']
                elif ext == '.g09':
                    tag = environ['G09_TAG_FILE']
                elif ext == '.qc':
                    tag = environ['QCHEM_TAG_FILE']
            except KeyError:
                exit('Environment Variable Error: This code expects environment variables for tag files! See README')

            # break from 'Input Check 1'
            break

    # Input check 2: cluster choice logic, can't use '-g' and '-d' keywords together
    #   if not '--time' set default time
    #   with '--debug' option one cannot specify '--time' > 1
    if (arguments['--general-compute'] and arguments['--debug']) or \
       (arguments['--general-compute'] and arguments['--industry']) or \
       (arguments['--debug'] and arguments['--industry']):
        exit('Input Error: One can submit to either the general compute (-g/--general-compute),' +
                ' debug (-d/--debug), or industry (-i, --industry) queues!')
    elif arguments['--general-compute'] and not arguments['--time']:
        arguments['--time'] = '72'
    elif arguments['--industry'] and not arguments['--time']:
        arguments['--time'] = '72'
    elif arguments['--debug'] and arguments['--time']:
        print('--> Resetting time to 1 hour due to restrictions on debug queue <--')
        arguments['--time'] = '1'
    elif arguments['--debug'] and not arguments['--time']:
        arguments['--time'] = '1'
    elif not arguments['--time']:
        arguments['--time'] = '2000'

    # Input Check 3: if not '--num-nodes' set num-nodes, if not '--num-procs' set num-procs
    if not arguments['--num-nodes']:
        arguments['--num-nodes'] = 1
    if not arguments['--num-procs']:
        arguments['--num-procs'] = 1

    # Input Check 4: Check Processor Numbers and Set Complicated Memory Defaults
    if not arguments['--memory']:
        num_procs = int(arguments['--num-procs'])

        if arguments['--general-compute'] or arguments['--debug']:
            # Check number of processors
            if arguments['--debug'] and num_procs > 12:
                exit('Input Error: Cannot ask for more than 12 cores on debug cluster')
            elif num_procs > 32:
                exit('Input Error: Cannot ask for more than 32 cores on general compute cluster')

            # Set Memory Defaults
            if num_procs <= 8:
                arguments['--memory'] = int(23000 * (num_procs / 8))
            elif num_procs <= 12:
                arguments['--memory'] = int(48000 * (num_procs / 12))
            elif num_procs <= 16:
                arguments['--memory'] = int(128000 * (num_procs / 16))
            elif num_procs <= 32:
                arguments['--memory'] = int(256000 * (num_procs / 32))
        elif arguments['--industry']:
            # Check number of processors
            if num_procs > 16:
                exit('Input Error: Cannot ask for more that 16 cores on industry cluster')

            # Set Memory Defaults
            arguments['--memory'] = int(64000 * (num_procs / 16))
        else:
            # Check number of processors
            if num_procs > 16:
                exit('Input Error: Cannot ask for more that 16 cores on Chemistry Cluster')

            # Set Memory Default
            if num_procs <= 8:
                arguments['--memory'] = int(23000 * (num_procs / 8))
            elif num_procs <= 16:
                arguments['--memory'] = int(125000 * (num_procs / 16))
    else:
        print("Warning: Set memory to {} MB! SLURM expects this number in MB".format(arguments['--memory']))

    # We need to get the username from environ['USER'] and group (i.e. jochena/ezurek) from first entry in groups command
    #   groupname is "tricky":
    #       1) use check_output from subprocess to generate a byte array
    #       2) decode('utf-8') converts the byte array to an ascii string
    #       3) split() takes the ascii string and splits via black spaces, we need the zero'th element
    username = environ['USER']
    groupname = check_output('groups', shell=True).decode('utf-8').split()[0]

    # We are now in a position to generate the '.slurm' file
    slurmname = '{0}.slurm'.format(splitext(arguments['--input'])[0])
    jobname, ext = splitext(arguments['--input'])
    with open(slurmname, 'w') as f:
        write_slurm_header(f, arguments, jobname, groupname)
        write_sbcast_directives(f, arguments)
        if ext == '.adf':
            write_adf_environment(f, tag)
        elif ext == '.nw':
            write_nwchem_environment(f, tag)
        elif ext == '.g09':
            write_g09_environment(f, tag)
        elif ext == '.qc':
            write_qchem_environment(f, tag)

    # Finally we can thank the user for using our program
    #print('The computer master race thanks you for using sgen.py {0}'.format(username))

except KeyboardInterrupt:
    exit('Interrupt detected! exiting...')
