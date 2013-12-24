Misc
===

SLURM
==
For SLURM I have written some squeue wrappers to make a more readable output.
* bqueue.sh -> Runs squeue for USER environment variable
* jqueue.sh -> Runs squeue for a specific partition, in my case jochena
* scrubber.py, gen-scrubber.py -> Temporary scripts needed for error in SLURM inputs originally
* sgen.py -> Wraps ADF (.adf), NWChem (.nw), Gaussian (.com), and QChem (.qc) inputs with SLURM scripts.
You must define some environment variables to set up environment and it should be self explanatory what
those variables are when you run the script, however ask me for more details if needed.

LaTeX
==
The runlatex bash script runs latex and bibtex (an appropriate number of times) on a tex file, while
halting on errors. Note I use dvi2pdf at the end to create a pdf image, I prefer to use EPS images and
therefore I don't run pdflatex. Obviously, that is easily changed.

tensor-tools
==
This directory contains some tools I have used to do passive axis transformations on xyz based geometries. First script
is center-on-atom.py see --help option. The pas-rotate tool is designed to rotate an xyz based geometry by a 3 x 3 tensor.
To run the tool <code> ./pas-rotate XYZGeom Tensor <\code>. Where XYZGeom is a xyz file and Tensor is a text file containing
a 3 x 3 symmetric tensor (see efg-tensor as example). Additionally, one can specify 'inverse' for Tensor instead of a file to
invert the geometry (I use this for stereochemical inversions). 

ut-overlap-code
==
Ignore old-ut-overlap-code as this is an old non-parallelized version. This directory contains a tool to do a 4 x 4 complete space
unitary transformation on 4 orthonormal cube files. I will add a citation for this code at a later date.
