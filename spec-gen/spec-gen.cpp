#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <vector>

using namespace std;

string GetNextArg(int &i, int argc, char **argv)
{
    i++;
    if(i >= argc){
        throw runtime_error("Argument: " + string(argv[i-1]) + " is the final argument?");
    }
    return argv[i];
}

void printHelp()
{
    cout << "spec-gen [options]\n";
    cout << "Options Include:\n";
    cout << "   '-h','--help'        : shows this menu\n";
    cout << "   '-i','--infile'      : sets name spectrum/cdspectrum file\n";
    cout << "   '-g','--gaussian'    : Gaussian line broadening\n";
    cout << "   '-l','--lorentzian'  : Lorentzian line broadening\n";
    cout << "   '-b','--broaden'     : Broadening factor sigma"
         << " (gaussian) or gamma (lorentzian)\n";
    cout << "   '-d','--griddensity' : Number of grid points\n";
    cout << "   '-p','--padding'     : Additional padding to emin/emax as"
         << " multiple of broaden parameter\n";
}

int main(int argc, char **argv)
{
    try{
        //Parse Input
        vector<double> xvec(10, 0);
        xvec[0] = 10.0;
        for (auto i: xvec) {
            cout << i << "\n";
        }
    }
    catch (exception &ex)
    {
        cout << ex.what() << "\n";
        return 1;
    }
    return 0;
}
