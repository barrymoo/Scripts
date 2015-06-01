Python Libraries
================

I am placing Python classes or scripts I find generally useful here. The directory is in its infancy.

* simpletemplate.py -- A set of functions I use to make basic templated input files.

Here is a template for Time-Dependent Hartree-Fock calculations in [NWChem](http://www.nwchem-sw.org/index.php/Main_Page)
which I used recently to generate and submit many Quantum Chemistry Jobs to a large computer cluster. The `{{ }}`
indicate template items for substitution.

```
echo
start {{name}}-tzvp-{{abbrev}}-tda
title {{name}}-tzvp-{{abbrev}}-tda

geometry noautoz units angstrom nocenter
symmetry c1
{{geom}}
end

## def2tzvp
basis spherical
* library def2-tzvp
end

charge {{charge}}

dft
grid xfine
direct
{{func}}
convergence energy 1d-8
iterations 200
end

tddft
cis
nroots 5
thresh 1e-05
singlet
triplet
end

set dft:job_grid_acc 1d-20
set cphf:acc 1.0d-10

task tddft energy
```

Note this is extremely simple, but the script is language agnostic which is what I needed.
