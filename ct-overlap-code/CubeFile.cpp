#include "CubeFile.h"

using namespace std;

//Misc. Functions
void set_ss(string &str, ifstream &f, stringstream &ss){
    ss.clear();
    getline(f, str);
    ss.str(str);
} 

//Default Contructor
CubeFile::CubeFile(){
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
    for(int i=0; i<cubeVals.size(); i++){
        f >> cubeVals[i];
    }
    f.close();        
}

//Write to File
void CubeFile::write(const string &filename){
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
    for(int i=0; i<cubeVals.size(); i++){
        if(counter == 6){
            f << '\n';
            counter = 0;
        }
        f << cubeVals[i] << ' ';
        counter += 1;
        if(i%numGridPts[2] == numGridPts[2]-1){
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

//Initialize function is the nearly the same as copy
// except only resize the cubeVals for later processing
void CubeFile::initialize(const CubeFile &A){
    nAtoms = A.nAtoms;
    startGridPt = A.startGridPt;
    numGridPts = A.numGridPts;
    gridVec = A.gridVec;
    intStep = A.intStep;
    geom = A.geom;
    cubeVals.resize(A.cubeVals.size());
}

double CubeFile::integrate(){
    double sum = 0.0;
    for(auto i: cubeVals) sum += i;
    return sum * intStep;
}

void CubeFile::print(){
    cout << "\ncubeVals:\n";
    for(auto i: cubeVals){
        cout << i << ' ';
    }
    cout << '\n';
}

double return_overlap(const CubeFile &A, const CubeFile &B){
    //Do Something
    return 1.0;
}
