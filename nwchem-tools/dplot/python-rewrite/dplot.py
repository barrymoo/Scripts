#!/usr/bin/env python
""" dplot.py -- A NWChem dplot block generator
Usage:
    dplot.py [options]
        (-i <nwchem.nw> | --input <nwchem.nw>)
        (-m <nwchem.movecs> | --movecs <nwchem.movecs>)
        (-l <mos> | --list <mos>)

Positional Arguments:
    -i, --input <nwchem.nw>         A NWChem input file
    -m, --movecs <nwchem.movecs>    A NWChem movecs file generated from input
    -l, --list <mos>                List of MO's (movecs) or TD's (civecs) to plot
                                    (no spaces allowed, example: 1-4,8-12,6,24 )

Options:
    -h, --help                      Print this screen and exit
    -v, --version                   Print the version of dplot.py
    -c, --civecs <nwchem.civecs>    Print Transition Densities (TD) instead of MOs
    -g, --grid <value>              Change default grid parameter [default=50]
"""


def split_mo_list(mos):
    '''
    This function converts a string such as '1-2,7,10-20'
    to a sorted list of integers, then outputs that list
    '''
    out = []
    for i in mos.split(','):
        if '-' in i:
            sp = i.split('-')
            [out.append(x) for x in range(int(sp[0]), int(sp[1]) + 1)]
        else:
            out.append(int(i))
    return sorted(out)


if __name__ == '__main__':
    # docopt parses the command line via the doc string above
    from docopt import docopt
    arguments = docopt(__doc__, version='dplot.py version 0.0.1')
