#include <iostream>
#include <stdexcept>
#include <string>
#include <fstream>
#include <Eigen/Dense>

using namespace std;

void print_help(){
    cout << "--> Transition Analyze Helper <--" << endl;
    cout << "This program allows one" <<
            " to analyze NWChem transitions" <<
            " using civecs and nbodata.47 files" << endl;
    cout << "--> Options <--" << endl;
    cout << "-h/--help: Shows this help screen" << endl;
    cout << "-c/--civecs: NWChem CI vectors file" << endl;
    cout << "-r/--rpa: CI vectors file is full TDDFT (TDA assumed by default)" << endl; 
    cout << "-n/--nbo: NBO data file (MOsinNLMO nbodata.47)" << endl;
}

int main(int argc, char *argv[]){
    try{
        //Some necessary variables
        string civecsFile="", nboFile="";
        bool civ=false, nbo=false, rpa=false;

        // Make sure user knows what they are doing
        if(argc == 1){
            throw invalid_argument("Invalid usage of script: try -h/--help");
        }

        //Go through command line arguments
        string argvstr;
        for (int i = 1; i < argc; i++) {
            argvstr = string(argv[i]);
            if(argvstr == "-h" || argvstr == "--help"){
                print_help();
                throw invalid_argument("Help detected: exiting gracefully");
                return 1;
            }
            else if(argvstr == "-c" || argvstr == "--civecs"){
                i+=1;
                if (i == argc){
                    throw invalid_argument("No CI vector file specified, try '-h/--help'");
                }
                else{
                    civecsFile=argv[i];
                    ifstream iciv(civecsFile.c_str());
                    if (!iciv.good()) throw invalid_argument(string(civecsFile)+": file doesn't exist, try '-h/--help'");
                    civ=true;
                }
            }
            else if(argvstr == "-n" || argvstr == "--nbo"){
                i+=1;
                if (i == argc){
                    throw invalid_argument("No NBO file specified, try '-h/--help'");
                }
                else{
                    nboFile=argv[i];
                    ifstream inbo(nboFile.c_str());
                    if (!inbo.good()) throw invalid_argument(string(nboFile)+": file doesn't exist, try '-h/--help'");
                    nbo=true;
                }
            }
            else if(argvstr == "-r" || argvstr == "--rpa") rpa=true;
            else throw invalid_argument("Invalid Argument: " + argvstr + ", try --help");
        }

        //Let's make sure we have enough information to proceed
        if (!civ) throw invalid_argument("You must specify a civecs file, '-c/--civecs' option");
        
        //Now we can process the civecs file
    }
    catch(std::exception &ex){
        cout << ex.what() << endl;
        return 1;
    } 
    return 0;
}
