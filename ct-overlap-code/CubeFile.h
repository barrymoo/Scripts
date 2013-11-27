#include <string>
#include <vector>
#include <array>
#include <ios>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

//Necessary typedefs and structures
typedef array <double, 3> Vector3d;
typedef array <int, 3> Vector3i;
typedef array <Vector3d, 3> Matrix3d;
struct Geometry{
    int atomicNum;
    double atomicMass;
    Vector3d xyzVec;
};

//Class for CubeFiles
class CubeFile{
    private:
        int nAtoms;
        Vector3d startGridPt;
        Vector3i numGridPts;
        Matrix3d gridVec;
        double intStep;
        vector <Geometry> geom;
        vector <double> cubeVals;
    public:
        CubeFile();
        CubeFile(const string &filename);
        CubeFile(const CubeFile &A);
        void read(const string &filename);
        void write(const string &filename);
        void copy(const CubeFile &A);
        void initialize(const CubeFile &A);
        double integrate();
        void print();        
        friend double return_overlap(const CubeFile &A, const CubeFile &B);
};

//Prototypes
double return_overlap(const CubeFile &A, const CubeFile &B);
