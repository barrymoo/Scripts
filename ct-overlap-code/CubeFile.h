#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <Eigen/Core>

using namespace Eigen;
using namespace std;

//Class for CubeFiles
class CubeFile{
    private:
        int nAtoms;
        Array3d startGridPt;
        Array3d numGridPts;
        ArrayXXd gridVec = ArrayXXd::Zero(3, 3);
        double intStep;
        ArrayXi atomicNum;
        ArrayXd atomicMass;
        ArrayXXd geometry;
        ArrayXd cube;
    public:
        void set(string filename);
        double integrate();
        void print();        
        friend double get_overlap(const CubeFile &A, const CubeFile &B);
};

//Prototypes
double get_overlap(const CubeFile &A, const CubeFile &B);
