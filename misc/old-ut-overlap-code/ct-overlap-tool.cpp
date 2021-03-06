#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <random>
#include <ctime>
#include <unsupported/Eigen/MatrixFunctions>
#include <omp.h>
#include "CubeFile.h"

using namespace Eigen;
using namespace std;

void initialize_random_number_generator(void){
    timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    srand(t.tv_nsec);
}

//Something wrong in this function
void unitary_transformation(const int n, const MatrixXd &U, const vector <CubeFile> &ab, vector <CubeFile> &abprime){
    size_t N = n;
    CubeFile A(ab[0]);
    if(U.cols() != n || U.rows() != n) throw runtime_error("U is not the correct dimensions?");
    else if(ab.size() != N) throw runtime_error("Input Vector is not the correct dimension?");
    else if(abprime.size() != N) throw runtime_error("Output Vector is not the correct dimension?");
    else{
        //Set the abprime vector cubeVals to zero
        for(size_t i=0; i<N; i++) abprime[i].zero_cubeVals();
        A.zero_cubeVals();

        //Now generate the new cube files
        for(size_t i=0; i<N; i++){ //Goes over the N vector CubeFile Array
            for(size_t j=0; j<N; j++){ //Goes over the j index of U
                A = ab[j] * U(i, j);
                abprime[i] += A;
            }
        }
    }
}

void print_lower_triangle(const int n, const MatrixXd &A){
    if(A.cols() != n || A.rows() != n) throw runtime_error("print_lower_triangle error: MatrixXd is not the correct dimensions?");
    cout.setf(ios::fixed);
    for(int i=0; i<n; i++){
        for(int j=0; j<i; j++){
            cout.width(9);
            cout.precision(6);
            cout << A(i, j) << " ";
        }
    }
    cout << "\n";
}

int main(int argc, char *argv[]){
    try{
        if(argc < 3){
            cout << "Error: Must list at least two cubes, see README\n";
            return 1;
        }
        //Get all the cube files on the command line and store to cubeFiles string vector
        int n = argc-1;
        vector <string> cubeFiles(n);
        for(int i=0; i < n; i++){
            cubeFiles[i] = argv[i+1];
        }
        //For each cube, store in a CubeFile object vector where they will be stored for unitary transformation
        vector <CubeFile> cubeObjs(n);
        for(int i=0; i < n; i++){
            cubeObjs[i].read(cubeFiles[i]);
        }
        //Now lets create a (U)nitary transformation matrix from a random antisymmetric matrix W
        initialize_random_number_generator();
        MatrixXd W = MatrixXd::Random(n, n);
        MatrixXd U = MatrixXd::Zero(n, n);
        //Make Random Matrix Antisymmetric
        for(int i=0; i<n; i++) W(i, i) = 0;
        for(int i=0; i<n; i++){
            for(int j=0; j<i; j++){
                W(j, i) = -W(i, j);
            }
        }
        //Generate Unitary Transformation Matrix
        U = W.exp();
        //Print out W and U for user!
        cout << "W      : ";
        print_lower_triangle(n, W);
        cout << "U      : ";
        print_lower_triangle(n, U);

        //Let's form the symmetric overlap matrix
        MatrixXd Oab = MatrixXd::Zero(n, n);
        for(int i=0; i<n; i++){
            for(int j=0; j<=i; j++){
                Oab(i, j) = g_return_modulo_overlap(cubeObjs[i], cubeObjs[j]);
                Oab(j, i) = Oab(i, j);
            }
        }
        cout << "O_{ab} : ";
        print_lower_triangle(n, Oab);

        //Let's apply the unitary transformation matrix
        vector <CubeFile> transformed_cubeObjs(n);
        for(size_t i=0; i<cubeObjs.size(); i++){
            transformed_cubeObjs[i] = cubeObjs[i];
        }
        unitary_transformation(n, U, cubeObjs, transformed_cubeObjs);
        //Let's now form the symmetric overlap matrix for the transformed cubes
        MatrixXd Oabprime = MatrixXd::Zero(n, n);
        for(int i=0; i<n; i++){
            for(int j=0; j<=i; j++){
                Oabprime(i, j) = g_return_modulo_overlap(transformed_cubeObjs[i], transformed_cubeObjs[j]);
                Oabprime(j, i) = Oabprime(i, j);
            }
        }
        cout << "O_{ab}': ";
        print_lower_triangle(n, Oabprime);
    }
    catch (exception &ex)
    {
        cout << ex.what() << "\n";
        return 1;
    }
    return 0;
}
