#include "CubeFile.h"

/*
 * CubeFile Class written by Barry Moore
 * No Warranty Implied
 * Things to update:
*/

using namespace std;
namespace bp = boost::python;

//Function to wrap Vector3d to Boost::Python::List
bp::list wrap_Vector3d(const Vector3d &A){
    bp::list myList;
    for(auto i: A) myList.append(i);
    return myList;
}

//Default Contructor
CubeFile::CubeFile(void){
    //Do Nothing
}

//File Construtor
CubeFile::CubeFile(const string &filename){
    CubeFile::read(filename);
}

//Copy Constructor
CubeFile::CubeFile(const CubeFile &A){
    CubeFile::copy(A);
}

//Read from file
void CubeFile::read(const string &filename){
    ifstream f(filename);
    string line;
    getline(f, line); //Title -- Unused
    getline(f, line); //Field Title -- Unused
    f >> nAtoms >> startGridPt[0] >> startGridPt[1] >> startGridPt[2];
    //for loop reads in 3 lines containing numGridPts and gridVec matrix
    for(int i=0; i<3; i++){
        f >> numGridPts[i] >> gridVec[i][0] >> gridVec[i][1] >> gridVec[i][2];
    } 
    intStep = gridVec[0][0] * gridVec[1][1] * gridVec[2][2];
    //Now we can read the geometry
    geom.resize(nAtoms);
    for(int i=0; i<nAtoms; i++){
        f >> geom[i].atomicNum >> geom[i].atomicMass >> geom[i].xyzVec[0]
          >> geom[i].xyzVec[1] >> geom[i].xyzVec[2];
    }
    //Now we can read the grid points
    cubeVals.resize(numGridPts[0]*numGridPts[1]*numGridPts[2]);
    for(size_t i=0; i<cubeVals.size(); i++){
        f >> cubeVals[i];
    }
    f.close();        
}

//Write to File
void CubeFile::write(const string &filename) const {
    ofstream f(filename);
    f << "Generated Cube File\n";
    f << "Generic Field Title\n";
    f << nAtoms << ' ';
    for(auto i: startGridPt) f << i << ' ';
    f << '\n';
    for(int i=0; i<3; i++){
        f << numGridPts[i] << ' ';
        for(auto j: gridVec[i]){
            f << j << ' ';
        }
        f << '\n';
    }
    for(int i=0; i<nAtoms; i++){
        f << geom[i].atomicNum << ' ' << geom[i].atomicMass << ' ';
        for(auto j: geom[i].xyzVec) f << j << ' ';
        f << '\n';
    }
    f.setf(ios::scientific);
    //f.width(13);
    //f.precision(5);
    int counter = 0;
    for(size_t i=0; i<cubeVals.size(); i++){
        if(counter == 6){
            f << '\n';
            counter = 0;
        }
        f << cubeVals[i] << ' ';
        counter += 1;
        if( i%numGridPts[2] == numGridPts[2]-1){
            f << '\n';
            counter = 0;
        }
    }
    f.close();
}

//Copy contents of cube file to current
void CubeFile::copy(const CubeFile &A){
    nAtoms = A.nAtoms;
    startGridPt = A.startGridPt;
    numGridPts = A.numGridPts;
    gridVec = A.gridVec;
    intStep = A.intStep;
    geom = A.geom;
    cubeVals = A.cubeVals;
}

//Check Function, return 0 if sizes are correct, 1 otherwise
int CubeFile::check(const CubeFile &A) const {
    return 0;
}

void CubeFile::scalar_add(const double &scalar){
    for(size_t i=0; i<cubeVals.size(); i++) cubeVals[i] += scalar;
}

void CubeFile::scalar_subtract(const double &scalar){
    double val=-scalar;
    CubeFile::scalar_add(val);
}

void CubeFile::scalar_multiply(const double &scalar){
    for(size_t i=0; i<cubeVals.size(); i++) cubeVals[i] *= scalar;
}

void CubeFile::scalar_divide(const double &scalar){
    if(scalar == 0.0){
        throw runtime_error("Attempting to use scalar_divide with zero");
    } 
    double val=1/scalar;
    CubeFile::scalar_multiply(val);
}

void CubeFile::cube_add(const CubeFile &A){
    for(size_t i=0; i<cubeVals.size(); i++){
        cubeVals[i] += A.cubeVals[i];
    } 
}

void CubeFile::cube_subtract(const CubeFile &A){
    for(size_t i=0; i<cubeVals.size(); i++){
        cubeVals[i] -= A.cubeVals[i];
    } 
}

void CubeFile::cube_multiply(const CubeFile &A){
    for(size_t i=0; i<cubeVals.size(); i++){
        cubeVals[i] *= A.cubeVals[i];
    } 
}

void CubeFile::zero_cubeVals(void){
    for(size_t i=0; i<cubeVals.size(); i++){
        cubeVals[i] = 0.0;
    }
}

CubeFile CubeFile::absolute_value(void){
    for(size_t i=0; i<cubeVals.size(); i++) cubeVals[i] = abs(cubeVals[i]);
    return *this;
}

CubeFile CubeFile::square_root(void){
    for(size_t i=0; i<cubeVals.size(); i++) cubeVals[i] = sqrt(cubeVals[i]);
    return *this;
}

double CubeFile::integrate(void) const {
    double sum = 0.0;
    for(auto i: cubeVals) sum += i;
    return sum * intStep;
}

double CubeFile::iso_integrate(const double &iso) const {
    double sum = 0.0;
    for(size_t i=0; i<cubeVals.size(); i++){
        if(cubeVals[i] >= iso){
            sum += cubeVals[i];
        }
    }
    return sum * intStep;
}

bp::list CubeFile::centroid(void) const {
    //x, y, z starting points
    double xp = startGridPt[0];
    double yp = startGridPt[1];
    double zp = startGridPt[2];
    //Define and initialize centroid vector
    Vector3d centroid;
    for(size_t i=0; i<3; i++) centroid[i] = 0;
    //Traverse cubeVals and generate centroid vector
    // cubeVals in a 1d vector, use an outside counter
    int counter = 0;
    for(size_t i=0; i<numGridPts[0]; i++){
        for(size_t j=0; j<numGridPts[1]; j++){
            for(size_t k=0; k<numGridPts[2]; k++){
                /* \int \phi_i^2 \colvec{x, y, z} dV */
                centroid[0] += pow(cubeVals[counter],2) * xp;
                centroid[1] += pow(cubeVals[counter],2) * yp;
                centroid[2] += pow(cubeVals[counter],2) * zp;
                //Increment z point by dz
                zp += gridVec[2][2];
                //Increment counter every loop iteration
                counter += 1;
            }
            //Reset z point to starting point
            zp = startGridPt[2];
            //Increment y by dy
            yp += gridVec[1][1];
        }
        //Reset z and y to starting point
        zp = startGridPt[2];
        yp = startGridPt[1];
        //Increment x by dz
        xp += gridVec[0][0];
    }
    //Multiply centroid vector components by dV
    for(size_t i=0; i<3; i++) centroid[i] *= intStep;
    //Return orbital centroid
    return wrap_Vector3d(centroid);
}

void CubeFile::print(void) const {
    cout << "\ncubeVals:\n";
    for(auto i: cubeVals){
        cout << i << ' ';
    }
    cout << '\n';
}

/*
 *
 * Operator Overloads
 *
*/
CubeFile& CubeFile::operator=(const CubeFile &A){
    CubeFile::copy(A);
    return *this;
}

CubeFile& CubeFile::operator+=(const CubeFile &A){
    CubeFile::cube_add(A);
    return *this;
}

CubeFile& CubeFile::operator+=(const double &scalar){
    CubeFile::scalar_add(scalar);
    return *this;
}

CubeFile& CubeFile::operator-=(const CubeFile &A){
    CubeFile::cube_subtract(A);
    return *this;
}

CubeFile& CubeFile::operator-=(const double &scalar){
    CubeFile::scalar_subtract(scalar);
    return *this;
}

CubeFile& CubeFile::operator*=(const CubeFile &A){
    CubeFile::cube_multiply(A);
    return *this;
}

CubeFile& CubeFile::operator*=(const double &scalar){
    CubeFile::scalar_multiply(scalar);
    return *this;
}

CubeFile& CubeFile::operator/=(const double &scalar){
    CubeFile::scalar_divide(scalar);
    return *this;
}

CubeFile CubeFile::operator+(const CubeFile &A) const{
    return g_cube_add(*this, A);
}

CubeFile CubeFile::operator+(const double &scalar) const{
    return g_scalar_add(*this, scalar);
}

CubeFile CubeFile::operator-(const CubeFile &A) const{
    return g_cube_subtract(*this, A);
}

CubeFile CubeFile::operator-(const double &scalar) const{
    return g_scalar_subtract(*this, scalar);
}

CubeFile CubeFile::operator*(const CubeFile &A) const {
    return g_cube_multiply(*this, A);
}

CubeFile CubeFile::operator*(const double &scalar) const{
    return g_scalar_multiply(*this, scalar);
}

CubeFile CubeFile::operator/(const double &scalar) const{
    return g_scalar_divide(*this, scalar);
}

/*
 *
 * Global Function Definitions
 *
*/
double g_return_overlap(const CubeFile &A, const CubeFile &B){
    CubeFile C(A);
    C *= B;
    return C.integrate();
}

double g_return_modulo_overlap(const CubeFile &A, const CubeFile &B){
    CubeFile C(A);
    CubeFile D(B);
    return (C.absolute_value() * D.absolute_value()).integrate();
}

CubeFile g_scalar_add(const CubeFile &A, const double &scalar){
    CubeFile B(A);
    B += scalar;
    return B;
}

CubeFile g_scalar_subtract(const CubeFile &A, const double &scalar){
    CubeFile B(A);
    B -= scalar;
    return B;
}

CubeFile g_scalar_multiply(const CubeFile &A, const double &scalar){
    CubeFile B(A);
    B *= scalar;
    return B;    
}

CubeFile g_scalar_divide(const CubeFile &A, const double &scalar){
    CubeFile B(A);
    B /= scalar;
    return B;
}

CubeFile g_cube_add(const CubeFile &A, const CubeFile &B){
    CubeFile C(A);
    C += B;
    return C;
}

CubeFile g_cube_subtract(const CubeFile &A, const CubeFile &B){
    CubeFile C(A);
    C -= B;
    return C;
}

CubeFile g_cube_multiply(const CubeFile &A, const CubeFile &B){
    CubeFile C(A);
    C *= B;
    return C;
}

CubeFile g_cube_absolute_value(const CubeFile &A){
    CubeFile B(A);
    return B.absolute_value();
}

CubeFile g_cube_square_root(const CubeFile &A){
    CubeFile B(A);
    return B.square_root();
}

CubeFile operator+(const double &scalar, const CubeFile &A){
    return A + scalar;
}

CubeFile operator-(const double &scalar, const CubeFile &A){
    return -1.0 * A + scalar;
}

CubeFile operator*(const double &scalar, const CubeFile &A){
    return A * scalar;
}

CubeFile operator/(const double &scalar, const CubeFile &A){
    return A / scalar;
}
