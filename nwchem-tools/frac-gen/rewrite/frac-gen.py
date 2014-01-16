#!/usr/bin/env python
from os import environ
from os.path import splitext
from sys import argv, exit


def say_hello():
    user = environ['USER']
    print('-' * (len(user)+8))
    print('--> ' + user + ' <--')
    print('-' * (len(user)+8))


def show_help():
    say_hello()
    print('Description: This program creates a gnuplot file from any number of fractional occupation data files')
    print('Usage: frac-gen.py functional.dat * N file.plt')
    print('Author: Barry Moore (moore0557@gmail.com)')
    exit()
    

#Program Main Function
if __name__ == '__main__':
    #Check for Help
    if len(argv) == 1 or argv[1] == '-h' or argv[1] == '--help':
        show_help()
    #Quick redirect file check 
    elif splitext(argv[-1])[1] != '.plt':
        print("ERROR: You must end with a '.plt' file")
        show_help()    
    #Now we can do something
    else:   
        files = argv[1:len(argv)-1]
        pline = []
        pline.append('set term postscript eps color solid enhanced "Helvetica" 20\n')
        var = [1, 2, 3]
        #First print fitting headers
        for i, fi in enumerate(files):
            func, ext = splitext(fi)
            pline.append('#' * 5 + ' Fitting: ' + func.upper() + ' ' + '#' * 5)
            pline.append('f' + str(i) + 'ipr(x) = a' + str(var[0]) + ' + a' + str(var[1]) + ' * x + a' + str(var[2]) + ' * x**2') 
            pline.append('fit [1:0] f' + str(i) + "ipr(x) '" + fi + "' using 1:2 via a" + str(var[0]) + ', a' + str(var[1]) + ', a' + str(var[2]))
            pline.append('f' + str(i) + 'ip(x) = (x>0) ? f' + str(i) + 'ipr(x) : (1/0)\n')
            var = [x+3 for x in var]
            pline.append('f' + str(i) + 'ear(x) = a' + str(var[0]) + ' + a' + str(var[1]) + ' * x + a' + str(var[2]) + ' * x**2') 
            pline.append('fit [0:-1] f' + str(i) + "ear(x) '" + fi + "' using 1:2 via a" + str(var[0]) + ', a' + str(var[1]) + ', a' + str(var[2]))
            pline.append('f' + str(i) + 'ea(x) = (x<0) ? f' + str(i) + 'ear(x) : (1/0)\n')
            var = [x+3 for x in var]
        
        #Print some gnuplot lines
        pline.append("set out '" + splitext(argv[-1])[0] + ".eps'")
        pline.append('set key top right')
        pline.append('set size ratio 1.0')
        pline.append('set pointsize 1.25')
        pline.append('set xzeroaxis')
        pline.append('set xrange [-1:1] noreverse')
        pline.append('#set yrange [-0.5:9.5]')
        pline.append('set xtics 0.2')
        pline.append('set ytics 1')
        pline.append('set xlabel "{/Symbol D}N"')
        pline.append('set ylabel "[E({/Symbol D}N)-E(0)] / eV"')

        #Print the plot lines
        var = [3, 6]
        mod = [0, 1]
        for i, fi in enumerate(files):
            func, ext = splitext(fi)
            line = "'" + fi + "' u 1:2 t sprintf('%s (%4.2f,%5.2f)','" + func.upper() + "',a" + str(var[0]) + ',a' + str(var[1]) + \
                ') w p lt ' + str(mod[0]) + ' lw 2 pt ' + str(mod[1]) + ', f' + str(i) + 'ip(x) w l lt ' + str(mod[0]) + \
                ' lw 2 notitle, f' + str(i) + 'ea(x) w l lt ' + str(mod[0]) + ' lw 2 notitle'
            if i == 0:
                pline.append('plot ' + line + ', \\')
            elif i == len(files)-1:
                pline.append(' ' * 5 + line)
            else:
                pline.append(' ' * 5 + line + ', \\')
            var = [x+6 for x in var]
            mod = [x+1 for x in mod]

        #Finally write the output
        with open(argv[-1], 'w') as f:
            f.write('\n'.join(pline))
