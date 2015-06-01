frac-gen.py
===========

Here is the `frac-gen.py` utility. This script is used in conjunction with 
`fon-gen.py`. It takes the output from `fon-gen.py` and generates a
[Gnuplot](http://www.gnuplot.info/) input file for plotting. The script isn't
horribly written, but improvements could be made. I haven't used the script 
in awhile, but if your interested please let me know.

To generate a `*.plt` file from `a.dat` and `b.dat` just run:

`frac-gen.py *.dat test.plt`

then, to compile the `*.eps` file:

`gnuplot test.plt`
