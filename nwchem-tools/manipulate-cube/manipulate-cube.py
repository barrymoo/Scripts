#!/usr/bin/env python
import argparse
import os
import sys
import subroutines


try:
    #Parse Command Line Arguments
    parser = argparse.ArgumentParser(usage='manipulate-cube.py cubefile1 cubefile2 operation',
                                    description='This program allows mathematical operations' +
                                    ' on cube files by other cube files or scalars.',
                                    prefix_chars='-')
    parser.add_argument('cubefile1', metavar='N', type=str, nargs='?',
                        default=0, help='Gaussian cubefile')
    parser.add_argument('cubefile2', metavar='N', type=str, nargs='?',
                        default=0,help='Gaussian cubefile or scalar(ValueErrors yield 0 in output.cube)')
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument('-add', action='store_true', help='Add cubefile2/scalar to cubefile1')
    group.add_argument('-sub', action='store_true', help='Subtract cubefile2/scalar from cubefile1')
    group.add_argument('-mul', action='store_true', help='Multiple cubefile1 by cubefile2/scalar')
    group.add_argument('-div', action='store_true', help='Div cubefile1 by cubefile2/scalar')
    group.add_argument('-exp', action='store_true', help='Take cubefile1 to cubefile2/scalar power')
    group.add_argument('-abs', action='store_true', help='Take absolute value of cubefile1, must be a scalar value in second position')
    args = parser.parse_args()

    #Check for proper usage
    if not os.path.isfile(args.cubefile1):
        print('File: ' + args.cubefile1 + ' does not exist?')
        sys.exit()
    try:
        value = float(args.cubefile2)
    except ValueError:
        if not os.path.isfile(args.cubefile2):
            print('File: ' + args.cubefile2 + ' does not exist?')
            sys.exit()
        else:
            value = None

    #Whats the operation?
    if args.add:
        operation = 'add'
    elif args.sub:
        operation = 'sub'
    elif args.mul:
        operation = 'mul'
    elif args.div:
        operation = 'div'
    elif args.exp:
        operation = 'exp'
    elif args.abs:
        operation = 'abs'
        value = 0
   
    print(args.cubefile1, args.cubefile2, operation)

    #Now we call proper subroutine
    if value != None: 
        subroutines.cubscalar(args.cubefile1, value, operation)
    else:
        subroutines.cubcub(args.cubefile1, args.cubefile2, operation)

except KeyboardInterrupt:
    print('Interrupt detected...exiting')
    sys.exit()
