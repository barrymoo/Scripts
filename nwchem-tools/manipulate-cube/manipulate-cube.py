#!/usr/bin/env python
import argparse
import os
import sys
import subroutines


try:
    #Parse Command Line Arguments
    parser = argparse.ArgumentParser(usage='manipulate-cube.py cubefile1 cubefile2 operation',
                                    description='This program allows mathematical operations' +
                                    ' on cube files by other cube files or scalars (2 or 0.5' +
                                    ' for example)',
                                    prefix_chars='-')
    parser.add_argument('cubefile1', metavar='N', type=str, nargs=1,
                        default=[0],help='Gaussian cubefile')
    parser.add_argument('cubefile2', metavar='N', type=str, nargs=1,
                        default=[0],help='Gaussian cubefile or scalar(ValueErrors yield 0 in output.cube)')
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument('-add', action='store_true', help='Add cubefile2/scalar to cubefile1')
    group.add_argument('-sub', action='store_true', help='Subtract cubefile2/scalar from cubefile1')
    group.add_argument('-mul', action='store_true', help='Multiple cubefile1 by cubefile2/scalar')
    group.add_argument('-div', action='store_true', help='Div cubefile1 by cubefile2/scalar')
    group.add_argument('-exp', action='store_true', help='Take cubefile1 to cubefile2/scalar power')
    args = parser.parse_args()

    #Check for proper usage
    if not os.path.isfile(args.cubefile1[0]):
        print('File: ' + args.cubefile1[0] + ' does not exist?')
        sys.exit()
    try:
        value = float(args.cubefile2[0])
    except ValueError:
        if not os.path.isfile(args.cubefile2[0]):
            print('File: ' + args.cubefile2[0] + ' does not exist?')
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
    
    #Now we call proper subroutine
    if value: 
        subroutines.filescalar(args.cubefile1[0], value, operation)
    else:
        subroutines.filefile(args.cubefile1[0], args.cubefile2[0], operation)

except KeyboardInterrupt:
    print('Interrupt detected...exiting')
    sys.exit()
