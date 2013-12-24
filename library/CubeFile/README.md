CubeFile Tool
===

This tool is designed to provide the user with access to CubeFile manipulation from C++ and the python
terminal. To use this class in C++ simply include the header file, you will need the Eigen Libraries.
To compile the python shared library you will, additionally, need the Boost Python Library. You should be
able to update the Makefile and compile the code yourself. After the shared object is created
you must add the directory that contains the shared object to your PYTHONPATH environment variable.
Now you can call the CubeFile library from the python terminal! I prefer using the ipython terminal
as it handles tab autocompletion for functions, and is generally more awesome.

Let's look at some classic examples for orthonormal cube files i.e. the molecular orbitals
created from a quantum chemical calculation. We must check the integrals &lt; i | j &gt; = 0 and &lt; i | i &gt; = 1.
Note these cube files have a small grid value and therefore the integration is not exact.
Follow this code snippet in the python terminal to test the integration.

    import CubeFile
    c1 = CubeFile.CubeFile("1.cube")
    c2 = CubeFile.CubeFile("2.cube")
    print((c1*c1).integrate(), "Equals 1?")
    print((c2*c2).integrate(), "Equals 1?")
    print((c1*c2).integrate(), "Equals 0?")

Let's say we need to do some math on the cubes file and then write it to a new file. Try this:

    import CubeFile
    c1 = CubeFile.CubeFile("1.cube")
    c2 = CubeFile.CubeFile("2.cube")
    new = 2 * c1 - c2 / 2
    new.write("math.cube")
