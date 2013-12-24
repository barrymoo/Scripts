#!/usr/bin/env python
import sys
import os
import argparse


parser = argparse.ArgumentParser(usage='dplot.py nwchem.nw nwchem.movecs [options] ',
                                  description='dplot block generator',
                                 prefix_chars='-')
parser.add_argument('nwchem.nw', help='NWChem input file')
parser.add_argument('nwchem.movecs', help='NWChem converged movecs file')
parser.add_argument('-c', type=str, nargs=1,
                    default=0, help='NWChem civecs file')
parser.add_argument('-g', metavar='N', type=int, nargs=1,
                    default=50, help='Grid parameter (default = 50)')
parser.add_argument('-l', metavar='N', nargs='*',
                    default=[1], help='MOs or TDs to plot')
parser.add_argument('-b', action='store_true', help='Plot beta orbitals') 
parser.add_argument('-ab', action='store_true', help='Plot alpha and beta orbitals') 
args = parser.parse_args()

#This little bit of code seperates input into uniq sorted list
# Should make this seperate definition
for i in range(0, len(args.l)):
    if len(args.l[i]) > 1:
        spLine = args.l[i].split('-')
        print(spLine)
    else:
        print(args.l[i])
