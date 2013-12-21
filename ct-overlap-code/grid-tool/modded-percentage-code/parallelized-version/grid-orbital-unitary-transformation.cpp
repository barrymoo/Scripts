#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <eigen3/unsupported/Eigen/MatrixFunctions>
#include <boost/math/constants/constants.hpp>
#include <omp.h>
#include "CubeFile.h"

using namespace Eigen;
using namespace std;

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
        if(argc != 5){
            cout << "Error: Must list exactly 4 cubes, see README\n";
            return 1;
        }
        //Get all the cube files on the command line and store to cubeFiles string vector
        int n = 4;
        vector <string> cubeFiles(n);
        for(int i=0; i < n; i++) cubeFiles[i] = argv[i+1];

        //For each cube, store in a CubeFile object vector where they will be stored for unitary transformation
        vector <CubeFile> cubeObjs(n);
        for(int i=0; i < n; i++) cubeObjs[i].read(cubeFiles[i]);

        const double pi = boost::math::constants::pi<double>();
        //const double step = 9*pi/180;
        const double step = 90 * pi / 180;
        const int end = pi / (2 * step);

        //For Loop To Complete Grid for 4 by 4 case
        #pragma omp parallel for collapse(6)
        for(int i=0; i<=end; i++){
            for(int j=0; j<=end; j++){
                for(int k=0; k<=end; k++){
                    for(int l=0; l<=end; l++){
                        for(int m=0; m<=end; m++){
                            for(int o=0; o<=end; o++){
                                double ii = i * step;
                                double jj = j * step;
                                double kk = k * step;
                                double ll = l * step;
                                double mm = m * step;
                                double oo = o * step;

                                MatrixXd W = MatrixXd::Zero(n, n);
                                MatrixXd U = MatrixXd::Zero(n, n);

                                W(1, 0) = ii;
                                W(2, 0) = jj;
                                W(2, 1) = kk;
                                W(3, 0) = ll;
                                W(3, 1) = mm;
                                W(3, 2) = oo;

                                W(0, 1) = -W(1, 0);
                                W(0, 2) = -W(2, 0);
                                W(1, 2) = -W(2, 1);
                                W(0, 3) = -W(3, 0);
                                W(1, 3) = -W(3, 1);
                                W(2, 3) = -W(3, 2);
                                U = W.exp();

                                //Let's form the symmetric overlap matrix
                                MatrixXd Oab = MatrixXd::Zero(n, n);
                                for(int i=0; i<n; i++){
                                    for(int j=0; j<=i; j++){
                                        Oab(i, j) = g_return_modulo_overlap(cubeObjs[i], cubeObjs[j]);
                                        Oab(j, i) = Oab(i, j);
                                    }
                                }

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
                                //====== MODDED VERSION PRINT =======
                                #pragma omp critical
                                {
                                    cout.width(9);
                                    cout.precision(6);
                                    cout << "W      : ";
                                    print_lower_triangle(n, W);
                                    cout << "U      : ";
                                    print_lower_triangle(n, U);
                                    //cout << "O_{ab} : ";
                                    //print_lower_triangle(n, Oab);
                                    cout << "O_{ab}': ";
                                    print_lower_triangle(n, Oabprime);
                                    cout << "HL     : " << Oabprime(1,2) << " " << pow(U(1,1), 2) + pow(U(1,2), 2)
                                                    << " " << pow(U(2,1), 2) + pow(U(2,2), 2) << '\n';

                                    cout << "H-1L   : " << Oabprime(0,2) << " " << pow(U(0,0), 2) + pow(U(0,2), 2)
                                                    << " " << pow(U(2,0), 2) + pow(U(2,2), 2) << '\n';

                                    cout << "HL+1   : " << Oabprime(1,3) << " " << pow(U(1,1), 2) + pow(U(1,3), 2)
                                                    << " " << pow(U(3,1), 2) + pow(U(3,3), 2) << '\n';
                                    cout << "H-1H   : " << Oabprime(0,1) << " " << pow(U(0,0), 2) + pow(U(0,1), 2)
                                                    << " " << pow(U(1,0), 2) + pow(U(1,1), 2) << '\n';
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    catch (exception &ex)
    {
        cout << ex.what() << "\n";
        return 1;
    }
    return 0;
}
