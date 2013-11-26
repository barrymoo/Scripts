#include "CubeFile.h"

using namespace Eigen;
using namespace std;

void set_ss(string &str, ifstream &f, stringstream &ss){
    ss.clear();
    getline(f, str);
    ss.str(str);
} 

void CubeFile::set(string filename){
    ifstream f(filename);
    stringstream ss;
    string line;
    getline(f, line); //Title -- Unused
    getline(f, line); //Field Title -- Unused
    set_ss(line, f, ss); //nAtoms followed by startGridPt vector
    ss >> nAtoms >> startGridPt(0) >> startGridPt(1) >> startGridPt(2);
    //for loop reads in 3 lines containing numGridPts and gridVec matrix
    for(int i=0; i<3; i++){
        set_ss(line, f, ss);
        ss >> numGridPts(i) >> gridVec(i, 0) >> gridVec(i, 1) >> gridVec(i, 2);
    } 
    intStep = gridVec(0, 0) * gridVec(1, 1) * gridVec(2, 2);
    //Now we can read the geometry
    atomicNum = ArrayXi::Zero(nAtoms);
    atomicMass = ArrayXd::Zero(nAtoms);
    geometry = ArrayXXd::Zero(nAtoms, 3);
    for(int i=0; i<nAtoms; i++){
        set_ss(line, f, ss);
        ss >> atomicNum(i) >> atomicMass(i) >> geometry(i, 0)
           >> geometry(i, 1) >> geometry(i, 2);
    }
    //Now we can read the grid points
    cube = ArrayXd::Zero(numGridPts(0)*numGridPts(1)*numGridPts(2));
    ss.clear();
    ss << f.rdbuf();
    for(int i=0; i<cube.size(); i++){
        ss >> cube(i);
    }
    f.close();        
}

void CubeFile::print(){
    cout << "nAtoms: " << nAtoms << '\n';
    cout << "startGridPt:\n" << startGridPt << '\n';
    cout << "numGridPts:\n" << numGridPts << '\n';
    cout << "gridVec:\n" << gridVec << '\n';
    cout << "atomicNum:\n" << atomicNum << '\n';
    cout << "atomicMass:\n" << atomicMass << '\n';
    cout << "geometry:\n" << geometry << '\n';
    cout << "cube:\n" << cube << '\n';
}

double CubeFile::integrate(){
    double sum = cube.sum() * intStep;
    return sum;
}

double get_overlap(const CubeFile &A, const CubeFile &B){
    CubeFile overlap;
    //Allocate Necessary Class Variables
    overlap.cube = ArrayXd::Zero(A.cube.size());
    overlap.cube = A.cube * B.cube;
    return overlap.cube.sum() * A.intStep;
}
