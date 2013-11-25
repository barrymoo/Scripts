#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <unsupported/Eigen/MatrixFunctions>
#include "CubeFile.cpp"

using namespace Eigen;
using namespace std;

void initialize_random_number_generator(void){
    timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    srand(t.tv_nsec);
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
            cubeObjs[i].set(cubeFiles[i]);
        }
        //Now lets create a (U)nitary transformation matrix from a random antisymmetric matrix W
        initialize_random_number_generator();
        MatrixXd W = MatrixXd::Random(n, n);
        MatrixXd U = MatrixXd::Zero(n, n);
        //Make Random Matrix Antisymmetric
        for(int i=0; i < n; i++){
            for(int j=0; j < i; j++){
                W(j, i) = -W(i, j);
            }
        }
        //Set Diagonal Components to Zero
        for(int j=0; j < n; j++){
            W(j, j) = 0;
        }
        U = W.exp();
        cout << U * U.transpose() << '\n';
    }
    catch (exception &ex)
    {
        cout << ex.what() << "\n";
        return 1;
    }
    return 0;
}
