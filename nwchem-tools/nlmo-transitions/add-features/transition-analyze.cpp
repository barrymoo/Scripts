#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>
#include <fstream>
#include <Eigen/Core>
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
    cout << "-n/--nbo: NBO data file (MOsinNLMO nbodata.47)" << endl;
    cout << "-d/--debug: Print some debugging info" << endl;
}

int main(int argc, char *argv[]){
    try{
        cout << "--> Greetings from Transition Analyze <--" << endl;
        cout << "--> contact barrymoo@buffalo.edu with problems/questions <--" << endl;
        cout << endl;
        //Some necessary variables
        string civecsFile="", nboFile="";
        bool civ=false, nbo=false, debug=false;

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
            else if(argvstr == "-d" || argvstr == "--debug"){
                debug = true;
            }
            else throw invalid_argument("Invalid Argument: " + argvstr + ", try --help");
        }

        //Let's make sure we have enough information to proceed
        if (!civ) throw invalid_argument("You must specify a civecs file, '-c/--civecs' option");
        
        //Now we can process the civecs file
        ifstream iciv(civecsFile.c_str());
        int ipol, nroots;
        Eigen::Vector2i nocc, nmo, nfc, nfv, nov, nvir;
        char tda;
        double exen;
        iciv >> tda; //T if TDA, F is RPA
        iciv >> ipol; //1 if closed, 2 if open
        iciv >> nroots; //Number of Roots
        iciv >> nocc(0) >> nocc(1);
        iciv >> nmo(0) >> nmo(1);
        iciv >> nfc(0) >> nfc(1);
        iciv >> nfv(0) >> nfv(1);
        iciv >> nov(0) >> nov(1);

        //Debugging
        if(debug){
            cout << "== DEBUG BLOCK ==" << endl;
            cout << "TDA (T) or RPA (F): " << tda << endl;
            cout << "Closed (1) or Open (2) Shell: " << ipol << endl;
            cout << "Number of Roots: " << nroots << endl;
            for(int i=0; i<ipol; i++){
                cout << i << endl;
            }
            cout << "== END DEBUG BLOCK ==" << endl;
            cout << endl;
        }
        
        //std::vector of Matrices to handle ipol=2
        vector <Eigen::MatrixXd> xpy(ipol), xmy(ipol);
        for(int i=0; i<ipol; i++){
            xpy[i].resize(nroots, nov(i));
            xmy[i].resize(nroots, nov(i));
            nvir(i) = nmo(i) - nocc(i);
        }
        for(int i=0; i<ipol; i++){
            for(int j=0; j<nroots; j++){
                iciv >> exen;
                for(int k=0; k<nov(i); k++){
                    iciv >> xpy[i](j, k);
                }
                if(tda == 'F'){
                    for(int k=0; k<nov(i); k++){
                        iciv >> xmy[i](j, k);
                    }
                }
            }
        }
        iciv.close();

        //Let's first analyze the civecs data
        double ctot=0.0, contrib=0.0; //ctol = total contrib which should be 1!
        int offset=0;
        cout << "== CI Vector Analysis ==" << endl;
        for(int i=0; i<ipol; i++){
            if(i==1){
                cout << "-- Beta Spin --" << endl;
            }
            else{
                cout << "-- Alpha Spin --" << endl;
            }
            for(int j=0; j<nroots; j++){
                cout << "\t-- Root " << j+1 << " --" << endl;
                if(tda == 'F'){ //RPA BLOCK
                    for(int k=0; k<nocc(i); k++){
                        for(int l=0; l<nvir(i); l++){
                            offset=k*nvir(0)+l;
                            contrib=xpy[i](j, offset)*xmy[i](j, offset);
                            ctot+=contrib;
                            if(contrib >= 0.05){
                                cout << "\t\tocc " << k+1 << " -> vir " << nocc(i)+l+1 << " : " << contrib*100 << "%" << endl;
                            }
                        }
                    }
                }
                else{ //TDA BLOCK
                    for(int k=0; k<nocc(i); k++){
                        for(int l=0; l<nvir(i); l++){
                            offset=k*nvir(0)+l;
                            contrib=xpy[i](j, offset)*xpy[i](j, offset);
                            ctot+=contrib;
                            if(contrib >= 0.05){
                                cout << "\t\tocc " << k+1 << " -> vir " << nocc(i)+l+1 << " : " << contrib*100 << "%" << endl;
                            }
                        }
                    }
                }
                cout << "\t-- End Root " << j+1 << ", <X+Y|X-Y> = " << ctot << " --" << endl;
                ctot=0.0;
            }
        }
        cout << "== END CI Vector Analysis ==" << endl;
        cout << endl;

        //Now we can additionally analyze NBO data
        if(nbo){
            ifstream inbo(nboFile.c_str());
            Eigen::MatrixXd nbomat(nmo(0),nmo(0));
            getline(inbo, argvstr); // Skipping three lines
            getline(inbo, argvstr); // Store in string no longer used
            getline(inbo, argvstr); 
            for(int i=0; i<nmo(0); i++){
                for(int j=0; j<nmo(0); j++){
                    inbo >> nbomat(i, j);
                }
            }
            if(debug){
                cout << "== DEBUG BLOCK ==" << endl;
                nbomat = nbomat * nbomat.transpose();
                for(int i=0; i<nmo(0); i++){
                    cout << "Element " << i << " " << i << ": " << nbomat(i, i) << endl;
                }
                cout << "== END DEBUG BLOCK ==" << endl;
                cout << endl;
            }
            inbo.close();
            //First turn the linear array into a matrix
            Eigen::MatrixXd xpynbo(nvir(0), nocc(0)), xmynbo(nvir(0), nocc(0));
            cout << "== NBO Decomposition Analysis ==" << endl;
            for(int k=0; k<nroots; k++){
                cout << "\t-- Root " << k+1 << " --" << endl;
                for(int i=0; i<nocc(0); i++){
                    for(int j=0; j<nvir(0); j++){
                        offset=i*nvir(0)+j;
                        xpynbo(j, i) = xpy[0](k, offset);
                        xmynbo(j, i) = xmy[0](k, offset);
                    }
                }
                //Now do the block matrix multiplication
                xpynbo = nbomat.block(nocc(0), nocc(0), nvir(0), nvir(0)) * xpynbo * nbomat.block(0, 0, nocc(0), nocc(0));
                xmynbo = nbomat.block(nocc(0), nocc(0), nvir(0), nvir(0)) * xmynbo * nbomat.block(0, 0, nocc(0), nocc(0));
                for(int i=0; i<nvir(0); i++){
                    for(int j=0; j<nocc(0); j++){
                        contrib = xpynbo(i, j) * xmynbo(i, j);
                        if(contrib >= 0.05){
                            cout << "\t\tocc " << j+1 << " -> vir " << nocc(0)+i+1 << " : " << contrib*100 << "%" << endl;
                        }
                        ctot += contrib;
                    }
                }
                cout << "\t-- End Root " << k+1 << ", <X+Y|X-Y> = " << ctot << " --" << endl;
                ctot = 0.0;
            }
            cout << "== End NBO Decomposition Analysis ==" << endl;
        }
        cout << nvir(0) << endl;
    }
    catch(std::exception &ex){
        cout << ex.what() << endl;
        return 1;
    } 
    return 0;
}
