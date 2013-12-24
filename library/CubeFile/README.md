CubeFile Tool
===

This tool is designed to provide the user with access to CubeFile manipulation from the python
terminal. To compile this code you will need the Eigen and Boost Python Libraries. I prefer using
the ipython terminal as it handles tab autocompletion for function calls etc. Note that you can use
the code in c++ code by including the header file. After the shared object is created you must 
add this directory to your PYTHONPATH environment variable and you are ready to use the
python terminal.

Let's look at some classic examples for orthonormal cube files i.e. the molecular orbitals
created from a quantum chemical calculation. We must check the integrals &lt;i|j&gt;=0 and &lt;i|i&gt;=1.
Note these cube files have a small grid value and therefore the integration is not exact.
Follow this code snippet in the python terminal:

    import CubeFile
    c1 = CubeFile.CubeFile("1.cube")
    c2 = CubeFile.CubeFile("2.cube")
    print((c1*c1).integrate(), "Equals 1?")
    print((c2*c2).integrate(), "Equals 1?")
    print((c1*c2).integrate(), "Equals 0?")
