#include <string>
#include <vector>
#include <array>
#include <ios>
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <cmath>
#include <Python.h>
#include <boost/python.hpp>

using namespace std;
namespace bp = boost::python;

//Necessary typedefs and structures
typedef array <double, 3> Vector3d;
typedef array <unsigned int, 3> Vector3i;
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
        //Constructors
        CubeFile(void);
        CubeFile(const string &filename);
        CubeFile(const CubeFile &A);
        //Member Functions
        void read(const string &filename);
        void write(const string &filename) const;
        void copy(const CubeFile &A);
        int check(const CubeFile &A) const;
        void scalar_add(const double &scalar);
        void scalar_subtract(const double &scalar);
        void scalar_multiply(const double &scalar);
        void scalar_divide(const double &scalar);
        void cube_add(const CubeFile &A);
        void cube_subtract(const CubeFile &A);
        void cube_multiply(const CubeFile &A);
        void zero_cubeVals(void);
        CubeFile absolute_value(void);
        CubeFile square_root(void);
        double integrate(void) const;
        double iso_volume(const double &iso) const;
        bp::list centroid(void) const;
        void print(void) const;     
        //Operator Overloads
        CubeFile& operator=(const CubeFile &A);
        CubeFile& operator+=(const CubeFile &A);
        CubeFile& operator+=(const double &scalar);
        CubeFile& operator-=(const CubeFile &A);
        CubeFile& operator-=(const double &scalar);
        CubeFile& operator*=(const CubeFile &A);
        CubeFile& operator*=(const double &scalar);
        CubeFile& operator/=(const double &scalar);
        CubeFile operator+(const CubeFile &A) const;
        CubeFile operator+(const double &scalar) const;
        CubeFile operator-(const CubeFile &A) const;
        CubeFile operator-(const double &scalar) const;
        CubeFile operator*(const CubeFile &A) const;
        CubeFile operator*(const double &scalar) const;
        CubeFile operator/(const double &scalar) const;
        //Friend Functions
        friend double g_return_overlap(const CubeFile &A, const CubeFile &B);
};

//Prototypes for friend and global functions
double g_return_overlap(const CubeFile &A, const CubeFile &B);
double g_return_modulo_overlap(const CubeFile &A, const CubeFile &B);
CubeFile g_scalar_add(const CubeFile &A, const double &scalar);
CubeFile g_scalar_subtract(const CubeFile &A, const double &scalar);
CubeFile g_scalar_multiply(const CubeFile &A, const double &scalar);
CubeFile g_scalar_divide(const CubeFile &A, const double &scalar);
CubeFile g_cube_add(const CubeFile &A, const CubeFile &B);
CubeFile g_cube_subtract(const CubeFile &A, const CubeFile &B);
CubeFile g_cube_multiply(const CubeFile &A, const CubeFile &B);
CubeFile g_cube_absolute_value(const CubeFile &A);
CubeFile g_cube_square_root(const CubeFile &A);
CubeFile operator+(const double &scalar, const CubeFile &A);
CubeFile operator-(const double &scalar, const CubeFile &A);
CubeFile operator*(const double &scalar, const CubeFile &A);
CubeFile operator/(const double &scalar, const CubeFile &A);
CubeFile operator^(const double &scalar, const CubeFile &A);
