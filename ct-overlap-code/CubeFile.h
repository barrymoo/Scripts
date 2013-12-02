#include <string>
#include <vector>
#include <array>
#include <ios>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

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
        friend CubeFile scalar_add(const CubeFile &A, const double &scalar);
        friend CubeFile scalar_subtract(const CubeFile &A, const double &scalar);
        friend CubeFile scalar_multiply(const CubeFile &A, const double &scalar);
        friend CubeFile scalar_divide(const CubeFile &A, const double &scalar);
        friend CubeFile cube_add(const CubeFile &A, const CubeFile &B);
        friend CubeFile cube_subtract(const CubeFile &A, const CubeFile &B);
        friend CubeFile cube_multiply(const CubeFile &A, const CubeFile &B);
        friend CubeFile cube_abs(const CubeFile &A);
};

//Prototypes for friend function definitions
double return_overlap(const CubeFile &A, const CubeFile &B);
CubeFile scalar_add(const CubeFile &A, const double &scalar);
CubeFile scalar_subtract(const CubeFile &A, const double &scalar);
CubeFile scalar_multiply(const CubeFile &A, const double &scalar);
CubeFile scalar_divide(const CubeFile &A, const double &scalar);
CubeFile cube_add(const CubeFile &A, const CubeFile &B);
CubeFile cube_subtract(const CubeFile &A, const CubeFile &B);
CubeFile cube_multiply(const CubeFile &A, const CubeFile &B);
CubeFile cube_abs(const CubeFile &A);
