#!/usr/bin/env python
from sys import argv, exit

if len(argv) != 2:
    exit('Usage: pt2-analyze.py pt2.out')

with open(argv[1], 'r') as f:
    lines = f.readlines()

c = 0
r = 0
while c < len(lines):
    if 'FINAL CASPT2 RESULT' in lines[c]:
        while c < len(lines):
            if 'CASPT2 PROPERTY SECTION' in lines[c]:
                break
            if 'Denominators' in lines[c]:
                esum = 0.0
                c += 4
                if '--' in lines[c]:
                    print('=== Root {} ===\nNo low energy denominators for this root.'.format(r))
                    r += 1
                else:
                    print('=== Root {} ==='.format(r))
                    r += 1
                    while c < len(lines):
                        if '--' in lines[c]:
                            print('Total contrib to E2 (var): {:>10.6}, % contrib to E2: {:>10.6f}'.format(esum, 100 * esum / e2))
                            break
                        else:
                            sp = lines[c].split()
                            i = int(sp[2].split('.')[-1])
                            j = int(sp[3].split('.')[-1])
                            denom = float(sp[-4])
                            contrib = float(sp[-1])
                            esum += contrib
                            print('i: {:>3d}, j: {:>3d}, denom: {:>15.6e}, contrib: {:>15.6e}, e2 (var): {:>15.6e}, % of e2: {:>10.6f}'.format(i, j, denom, contrib, e2, 100 * contrib / e2))
                        c += 1
            if 'E2 (Variational)' in lines[c]:
                e2 = float(lines[c].split()[-1])
            c += 1
    c += 1
