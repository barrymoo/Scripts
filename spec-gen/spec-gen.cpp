#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <algorithm>

using namespace std;

string toLower(string &input)
{
    transform(input.begin(), input.end(), input.begin(), ::tolower);
    return input;
}

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
    cout << "   '-d','--griddensity' : Number of grid points (Default 100)\n";
    cout << "   '-p','--padding'     : Additional padding to emin/emax as"
         << " multiple of broaden parameter (Default 5)\n";
}

int main(int argc, char **argv)
{
    try{
        // Variables Needed
        string infile = "", argvstr = "";
        bool gaussian = false, lorentzian = false; 
        int gridDensity = 100;
        double broaden = 0.0, padding = 5.0;

        //Check for command line input
        if (argc == 1){
            throw runtime_error("Error: Not Enough Arguments, try '--help'/'-h' arguments");
        }

        //Parse Input
        for(int i=1; i < argc; i++){
            argvstr = argv[i];
            if (argvstr == "--help" || argvstr == "-h"){
                printHelp();
                return 1;
            }
            else if (argvstr == "-i" || argvstr == "--infile"){
                infile = GetNextArg(i, argc, argv);
            }
            else if (argvstr == "-g" || argvstr == "--gaussian"){
                gaussian = true;
            }
            else if (argvstr == "-l" || argvstr == "--lorentzian"){
                lorentzian = true;
            }
            else if (argvstr == "-b" || argvstr == "--broaden"){
                stringstream ss(GetNextArg(i, argc, argv));
                ss >> broaden;
                if(ss.fail() || ss.bad() || !ss.eof())
                    throw runtime_error("Error parsing input: " + argvstr + " must be followed by float");
            }
            else if (argvstr == "-d" || argvstr == "--griddensity"){
                stringstream ss(GetNextArg(i, argc, argv));
                ss >> gridDensity;
                if(ss.fail() || ss.bad() || !ss.eof())
                    throw runtime_error("Error parsing input: " + argvstr + " must be followed by integer");
            }
            else if (argvstr == "-p" || argvstr == "--padding"){
                stringstream ss(GetNextArg(i, argc, argv));
                ss >> padding;
                if(ss.fail() || ss.bad() || !ss.eof())
                    throw runtime_error("Error parsing input: " + argvstr + " must be followed by float");
            }
            else{
                throw runtime_error("Error parsing input: " + argvstr + " is not an option? try '--help'/'-h' argument!");
            }
        }

        // ***Check for command line validity ***
        // 1) Input File
        ifstream inputFile(infile);
        if (infile == "") throw runtime_error("Error: Input file option not set, see README");
        if (!inputFile) throw runtime_error("Error: " + infile + " is not a valid file");
        // 2) Gaussian or Lorentzian
        if ( (!gaussian && !lorentzian) || (gaussian && lorentzian) )
            throw runtime_error("Error: Must specify either gaussian or lorentzian broadening");
        // 3) Broading Parameter
        if (broaden == 0.0) throw runtime_error("Error: Must specify valid broadening parameter");

        //Now we can parse input file
        int nexc = 0;
        string line;
        stringstream ss;

        //First line gives us the number of excitations
        getline(inputFile, line);
        ss << line;
        ss >> nexc;
        ss.clear();

        //vector<double> x(10, 0);
    }
    catch (exception &ex)
    {
        cout << ex.what() << "\n";
        return 1;
    }
    return 0;
}
