#!/usr/bin/env python
""" dplot.py NWChem dplot block generator
Usage:
    dplot.py <nwchem.nw> <nwchem.movecs> 
        [-c <nwchem.civecs> | --civecs <nwchem.civecs>]
        [-l <mos> | --list <mos>]
        [-g <value> | --grid <value>]
    dplot.py (-h | --help)

Options:
    -h --help       Print this screen and exit
    -v --version    Print the version of code
    input.nw        The NWChem input file (required)
    input.movecs    The NWChem movecs file (required)
    -c --civecs     Print Transition Densities (TD) instead of MOs
    -l --list       List of MO's (movecs) or TD's (civecs) to plot
    -g --grid       Change default grid parameter [default=50]
"""

from docopt import docopt
arguments = docopt(__doc__, version='dplot.py version 0.0.1')
print(arguments)
