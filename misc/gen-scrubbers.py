#!/usr/bin/env python
import argparse
from sys import exit

parser = argparse.ArgumentParser(usage='submit-scrubbers.py --nodes/-n N N ...',
                                 description='submits scrubbing jobs any number of nodes')
parser.add_argument('--nodes','-n', default=[0], nargs='+', type=str, help='One or more nodes')
args = parser.parse_args()

if args.nodes == [0]:
    print('Scrubber expects to be given the --nodes/-n option')
    exit()
else:
    for i in args.nodes:
        f = open('scrub-' + i + '.slurm', 'w')
        f.write('#!/bin/bash\n')
        f.write('#SBATCH --time=00:05:00\n')
        f.write('#SBATCH --nodes=1\n')
        f.write('#SBATCH --ntasks-per-node=1\n')
        f.write('#SBATCH --partition=jochena\n')
        f.write('#SBATCH --account=pi-jochena\n')
        f.write('#SBATCH --job-name=scrubber-' + i + '\n')
        f.write('#SBATCH --output=/dev/null\n')
        f.write('#SBATCH --error=/dev/null\n')
        f.write('#SBATCH --mail-type=END\n')
        f.write('#SBATCH --nodelist=' + i + '\n')
        f.write('/ifs/projects/jochena/shared/software/barrymoo-scripts/misc/scrubber.py\n')
        f.close()
