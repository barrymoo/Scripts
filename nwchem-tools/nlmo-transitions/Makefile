# uses Intel fortran

### Fortran compiler
FC     = gfortran
FC90   = gfortran

### Disable/Enable OpenMP support
OMP = 
#OMP = --nparallel --openmp -O
#OMP = --parallel --ocl --threads 2

### Compilation/Linking options
FOPT = -O2 -mcmodel=medium -g -C

### Extra libraries
#FLIB =  -lblasmt -L/usr/local/lf9560/lib
#FLIB =  -L/usr/local/atlas/lib/Linux_ATHLONSSE1_2 -llapack -lf77blas -lcblas -latlas
#FLIB = -lport -L/crystal1/che9992/PORT/port3
#FLIB = -L$(HOME)/fortran -llapack -lblas
FLIB=

FFLAGS = $(FOPT) $(OMP)

.SUFFIXES: .f .F .F90 .f90

BIN = nlmo-transitions

#OBJ = snafu.o rho.o quadrature_new.o ryspol.o ovrlap.o kinetic.o b3lypxc.o \
      pbexc.o fockmatrix.o xalpha.o vwn5.o b88.o

###OBJ90 = vartypes.o write-matrices.o csgeti.o  contns.o csbegn.o nlmo-transitions.o 
OBJ90 = vartypes.o nlmo-transitions.o 

.F.o:
	$(FC) $(FOPT) -c $< -o $@
.F90.o: 
	$(FC90) $(FOPT) -c $< -o $@

.f90.o:
	$(FC90) $(FOPT) -c $< -o $@


all: $(BIN)

$(BIN): $(OBJ) $(OBJ90)
	$(FC) -o $(BIN) $(OBJ) $(OBJ90) $(FFLAGS) $(FLIB)

clean:
	rm -f $(BIN) $(OBJ) $(OBJ90) *.mod

