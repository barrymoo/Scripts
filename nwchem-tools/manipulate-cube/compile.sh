#!/bin/bash

gfortran -Wall -Wextra -O2 -c string.f08 cubefile.f08
gfortran -Wall -Wextra -O2 -o manipulate-cube manipulate-cube.f08 *.o -I./
