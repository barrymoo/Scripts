#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <unsupported/Eigen/MatrixFunctions>
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
    if(U.cols() != n || U.rows() != n) throw runtime_error("U is not the correct dimensions?");
    else if(ab.size() != N) throw runtime_error("Input Vector is not the correct dimension?");
    else if(abprime.size() != N) throw runtime_error("Output Vector is not the correct dimension?");
    else{
        //Set the abprime vector cubeVals to zero
        for(size_t i=0; i<N; i++) abprime[i].zero_cubeVals();    

        //Now generate the new cube files
        for(size_t i=0; i<N; i++){ //Goes over the N vector CubeFile Array
            for(size_t j=0; j<N; j++){ //Goes over the j index of U
                abprime[i] += ab[j] * U(i, j);
            }
        }
    }
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
        U = W.transpose();
        W -= U;
        //Generate Unitary Transformation Matrix
        U = W.exp();
        //Print out W and U for user!
        cout << "==========> W <==========\n" << W << "\n";
        cout << "==========> U <==========\n" << U << "\n";
        //Let's form the symmetric overlap matrix
        MatrixXd Oab = MatrixXd::Zero(n, n);
        for(int i=0; i<n; i++){
            for(int j=0; j<=i; j++){
                Oab(i, j) = g_return_modulo_overlap(cubeObjs[i], cubeObjs[j]);
                Oab(j, i) = Oab(i, j);
            }
        }
        cout << "==========> O_{ab} <==========\n" << Oab << "\n";
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
        cout << "==========> O_{ab}' <==========\n" << Oabprime << "\n";
    }
    catch (exception &ex)
    {
        cout << ex.what() << "\n";
        return 1;
    }
    return 0;
}
