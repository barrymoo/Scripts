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

        //Now lets create W on a U on a grid
        MatrixXd W = MatrixXd::Zero(n, n);
        MatrixXd U = MatrixXd::Zero(n, n);

        const double pi = boost::math::constants::pi<double>();
        //const double step = 9*pi/180;
        const double step = 90*pi/180;

        //For Loop To Complete Grid for 2 by 2 case
        for(double i=0.0; i<=pi/2; i+=step){
            for(double j=0.0; j<=pi/2; j+=step){
                for(double k=0.0; k<=pi/2; k+=step){
                    for(double l=0.0; l<=pi/2; l+=step){
                        for(double m=0.0; m<=pi/2; m+=step){
                            for(double o=0.0; o<=pi/2; o+=step){
                                //First make W antisymmetric, then get unitary transformation matrix
                                // from matrix exponentiation
                                W(1, 0) = i;
                                W(2, 0) = j;
                                W(2, 1) = k;
                                W(3, 0) = l;
                                W(3, 1) = m;
                                W(3, 2) = o;

                                W(0, 1) = -W(1, 0);
                                W(0, 2) = -W(2, 0);
                                W(1, 2) = -W(2, 1);
                                W(0, 3) = -W(3, 0);
                                W(1, 3) = -W(3, 1);
                                W(2, 3) = -W(3, 2);
                                U = W.exp();

                                //Print out W and U
                                cout << "W      : ";
                                print_lower_triangle(n, W);
                                //cout << W << '\n';
                                cout << "U      : ";
                                print_lower_triangle(n, U);
                                //cout << U << '\n';

                                //Let's form the symmetric overlap matrix
                                MatrixXd Oab = MatrixXd::Zero(n, n);
                                for(int i=0; i<n; i++){
                                    for(int j=0; j<=i; j++){
                                        Oab(i, j) = g_return_modulo_overlap(cubeObjs[i], cubeObjs[j]);
                                        Oab(j, i) = Oab(i, j);
                                    }
                                }
                                //cout << "O_{ab} : \n";
                                //print_lower_triangle(n, Oab);
                                //cout << Oab << '\n';

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
                                //cout << Oabprime << '\n';
                                //
                                //====== MODDED VERSION PRINT =======
                                cout.width(9);
                                cout.precision(6);
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
    catch (exception &ex)
    {
        cout << ex.what() << "\n";
        return 1;
    }
    return 0;
}
