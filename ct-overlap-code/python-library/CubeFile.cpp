#include "CubeFile.h"

/*
 * CubeFile Class written by Barry Moore
 * No Warranty Implied
 * Things to update:
 * 1) Stringstreams can become fstreams
*/

using namespace std;

//Misc. Functions
void set_ss(string &str, ifstream &f, stringstream &ss){
    ss.clear();
    getline(f, str);
    ss.str(str);
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
    stringstream ss;
    string line;
    getline(f, line); //Title -- Unused
    getline(f, line); //Field Title -- Unused
    set_ss(line, f, ss); //nAtoms followed by startGridPt vector
    ss >> nAtoms >> startGridPt[0] >> startGridPt[1] >> startGridPt[2];
    //for loop reads in 3 lines containing numGridPts and gridVec matrix
    for(int i=0; i<3; i++){
        set_ss(line, f, ss);
        ss >> numGridPts[i] >> gridVec[i][0] >> gridVec[i][1] >> gridVec[i][2];
    } 
    intStep = gridVec[0][0] * gridVec[1][1] * gridVec[2][2];
    //Now we can read the geometry
    geom.resize(nAtoms);
    for(int i=0; i<nAtoms; i++){
        set_ss(line, f, ss);
        ss >> geom[i].atomicNum >> geom[i].atomicMass >> geom[i].xyzVec[0]
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
    #pragma omp parallel for
    for(size_t i=0; i<cubeVals.size(); i++) cubeVals[i] += scalar;
}

void CubeFile::scalar_subtract(const double &scalar){
    double val=-scalar;
    CubeFile::scalar_add(val);
}

void CubeFile::scalar_multiply(const double &scalar){
    #pragma omp parallel for
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
    #pragma omp parallel for
    for(size_t i=0; i<cubeVals.size(); i++){
        cubeVals[i] += A.cubeVals[i];
    } 
}

void CubeFile::cube_subtract(const CubeFile &A){
    #pragma omp parallel for
    for(size_t i=0; i<cubeVals.size(); i++){
        cubeVals[i] -= A.cubeVals[i];
    } 
}

void CubeFile::cube_multiply(const CubeFile &A){
    #pragma omp parallel for
    for(size_t i=0; i<cubeVals.size(); i++){
        cubeVals[i] *= A.cubeVals[i];
    } 
}

void CubeFile::zero_cubeVals(void){
    #pragma omp parallel for
    for(size_t i=0; i<cubeVals.size(); i++){
        cubeVals[i] = 0.0;
    }
}

CubeFile CubeFile::absolute_value(void){
    #pragma omp parallel for
    for(size_t i=0; i<cubeVals.size(); i++) cubeVals[i] = abs(cubeVals[i]);
    return *this;
}

double CubeFile::integrate(void) const {
    double sum = 0.0;
    for(auto i: cubeVals) sum += i;
    return sum * intStep;
}

void CubeFile::print(void) const {
    cout << "nAtoms: " << nAtoms << "\n";
    cout << "startGridPt: ";
    for(auto i: startGridPt) cout << i << " ";
    cout << '\n';
    cout << "numGridPts: ";
    for(auto i: numGridPts) cout << i << " ";
    cout << '\n';
    cout << "gridVec:\n";
    for(size_t i=0; i<gridVec.size(); i++){
        for(size_t j=0; j<gridVec[i].size(); j++){
            cout << gridVec[i][j] << " ";
        }
        cout << '\n';
    }
    cout << "geom:\n";
    for(int i=0; i<nAtoms; i++){
        cout << geom[i].atomicNum << ' ' << geom[i].atomicMass << ' ';
        for(auto j: geom[i].xyzVec) cout << j << ' ';
        cout << '\n';
    }
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
    C.absolute_value();
    D.absolute_value();
    CubeFile E(C);
    E *= D;
    return E.integrate();
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
