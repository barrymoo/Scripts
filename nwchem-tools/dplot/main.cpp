#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include "ThrowStream.h"

// Changes that need some attention
// 1) Read PBS file once while getting geometry

using namespace std;

vector<long> ConvertSelectionString(const string & str, bool allowdupe);

string toLower(string & oldstring)
{
    transform(oldstring.begin(),oldstring.end(),oldstring.begin(),::tolower);
    return oldstring;
}

string GetNextArg(int & i, int argc, char ** argv)
{
    i++;
    if(i >= argc)
        THROWSTREAM << "This is the last argument: I was expecting more...";

    return argv[i];
}

void trimString(string & str)
{
    if ( str.length() == 0 )
    {
        return;
    }
    size_t begin = str.find_first_not_of(" \t");
    size_t end = str.find_last_not_of(" \t");
    if(begin == str.npos || end == str.npos)
    {
        str = "";
    }
    else if(end < begin)
    {
        THROWSTREAM << "Trimming error - begin is " << begin << " and end is " << end;
    }
    else
    {
       str = str.substr(begin,end-begin+1);
    }
}


string getExt(string pathName)
{
    size_t period = pathName.find_last_of(".");
    if ( period != string::npos )
    {
        string ext = pathName.substr(period + 1);
        return ext;
    }
    else
    {
        THROWSTREAM << "Error: " << pathName << " expecting file extension";
    }
}

string getPref(string pathName)
{
    size_t period = pathName.find_last_of(".");
    if ( period != string::npos )
    {
        string ext = pathName.substr(0,period);
        return ext;
    }
    else
    {
        THROWSTREAM << "Error: " << pathName << " expecting file extension";
    }
}

int getLine_counter(ifstream & ifs, string & line, int & counter)
{
    if ( getline(ifs,line) ) 
    {
        counter++;
        return 1;
    }
    else
    {
        return 0;
    }
}

void printDplotHelp()
{
    cout << "dplot [options]" << endl;
    cout << "Options Include:" << endl;
    cout << "   '-h','--help'       : shows this menu" << endl;
    cout << "   '-i','--infile'     : sets name of PBS or NW file" << endl;
    cout << "   '-m','--movecs'     : sets name of movecs file" << endl;
    cout << "   '-c','--civecs'     : sets name of civecs file" << endl;
    cout << "   '-l','--list'       : list of molecular vectors(movecs) or transition densities(civecs)" << endl;
    cout << "   '-g','--grid'       : sets grid parameter (default 50) " << endl;
    cout << "   '-b','--beta'       : sets beta orbitals on (default off)" << endl;
    cout << "   '-ab','--alphabeta' : sets alpha and beta orbitals on (default alpha on, beta off)" << endl;
    cout << "   '-amd','--amd64'    : for a pbs job sets queue to amd64 with 1 node, 4 processors, 00:10:00 walltime" << endl;
    cout << "   '-d','--debug'      : for a pbs job sets queue to debug with 1 node, 4 processors, default (01:00:00) walltime" << endl;
}

int main(int argc, char ** argv)
{
    try
    {
        string infile="", vecfile="", civecfile="", argvstr="";
        int grid = 50; //Default value
        bool beta = false, alphabeta = false, amd = false, debug = false;
        int linecounter = 0;
        vector<long> list;
        if ( argc == 1 )
        {
            printDplotHelp();
            THROWSTREAM << "Error: No command line arguments given. Use '-h' or '--help' option";
        }

        for(int i=1; i < argc; i++)
        {
            argvstr = argv[i];
            if(argvstr=="-h" || argvstr=="--help")
            {
                printDplotHelp();
                return 1;
            }
            else if (argvstr=="-i" || argvstr=="--infile")
            {
                stringstream ss(GetNextArg(i,argc,argv));
                ss >> infile;
                if(ss.fail() || ss.bad() || !ss.eof())
                    THROWSTREAM << "Error Reading File: " << argvstr;
            }
            else if (argvstr=="-m" || argvstr=="--movecs")
            {
                stringstream ss(GetNextArg(i,argc,argv));
                ss >> vecfile;
                if(ss.fail() || ss.bad() || !ss.eof())
                    THROWSTREAM << "Error Reading File: " << argvstr;
            }
            else if (argvstr=="-c" || argvstr=="--civecs")
            {
                stringstream ss(GetNextArg(i,argc,argv));
                ss >> civecfile;
                if(ss.fail() || ss.bad() || !ss.eof())
                    THROWSTREAM << "Error Reading File: " << argvstr;
            }
            else if (argvstr=="-g" || argvstr=="--grid")
            {
                stringstream ss(GetNextArg(i,argc,argv));
                ss >> grid;
                if(ss.fail() || ss.bad() || !ss.eof())
                    THROWSTREAM << "Error parsing grid: " << argvstr << " must be integer";
            }
            else if (argvstr=="-b" || argvstr=="--beta")
            {
                beta=true;
            }
            else if (argvstr=="-ab" || argvstr=="--alphabeta")
            {
                alphabeta=true;
            }
            else if (argvstr=="-l" || argvstr=="--list")
            {
                stringstream ss(GetNextArg(i,argc,argv));
                list = ConvertSelectionString(ss.str(),0);
            }
            else if ( argvstr=="-amd" || argvstr=="--amd64")
            {
                amd=true;
            }
            else if (argvstr=="-d" || argvstr=="--debug")
            {
                debug=true;
            }
            else
            {
                THROWSTREAM << "Error: Not a command line argument -> " << argvstr 
                << " <- be careful with regular expressions such as *pbs or *movecs";
            }
        }
        //Done Parsing lets do some checks
        string inExt = "";
        string vecExt = "";
        string civecExt = "";
        if (infile == "")
        {
            printDplotHelp();
            THROWSTREAM << "Error: No input pbs/nw file (-i option)";
        }
        else
        {
            inExt=getExt(infile);
        }
        if (vecfile == "")
        {
            printDplotHelp();
            THROWSTREAM << "Error: No movecs file (-m option)";
        }
        else
        {
            vecExt=getExt(vecfile);
        }
        if ( inExt != "nw" && inExt != "pbs")
        {
            THROWSTREAM << "Error: Not a valid input file (-i option expects .nw or .pbs)";
        }
        if ( vecExt != "movecs")
        {
            THROWSTREAM << "Error: Not a valid movecs file (-m option expects .movecs)";
        }
        if ( civecfile != "" )
        {
            civecExt=getExt(civecfile);
        }
        if ( civecfile != "" && civecExt != "civecs_singlet" && civecExt != "civecs_triplet" )
        {
            THROWSTREAM << "Error: Not a valid civecs file (-c option expects .civecs_singlet or .civecs_triplet";
        }

        //Lets get XYZ Min/Max XYZ Coords from nw or pbs file
        ifstream ifs;
        string line, linecopy, dummy;
        double x, y, z;
        double xmax,xmin,ymax,ymin,zmax,zmin;
        vector <double> xcoords;
        vector <double> ycoords;
        vector <double> zcoords;

        ifs.open(infile.c_str(),ifstream::in);
        if ( ! ifs.is_open() )
        {
            THROWSTREAM << "Error: Cannot Open File: " << infile;
        }

        while ( getLine_counter(ifs,line,linecounter) )
        {
            toLower(line);
            if ( line.find("geometry") != string::npos )
            {
                while ( getLine_counter(ifs,line,linecounter) )
                {
                    linecopy = line;
                    toLower(line);
                    if ( line.find("symmetry") != string::npos )
                    {
                        //Skip this line it is not needed
                    }
                    else if ( line.find("end") != string::npos)
                    {
                        break;
                    }
                    else
                    {
                        trimString(linecopy);
                        if ( linecopy.size() == 0 )
                        {
                            //Do Nothing
                        }
                        else
                        {
                            stringstream ss(line);
                            ss >> dummy >> x >> y >> z;
                            if(ss.fail() || ss.bad() || !ss.eof())
                                THROWSTREAM << "Error parsing xyz coordinates: Line Number -> " << linecounter << " Line -> " << line;
                            xcoords.push_back(x);
                            ycoords.push_back(y);
                            zcoords.push_back(z);
                        }
                    }
                }
                break;
            }
        }

        //Clear and reset infile to beggining, also get xmin to zmax
        ifs.clear();
        ifs.seekg(0,ios::beg);
        if ( !xcoords.empty() && !ycoords.empty() && !zcoords.empty())
        {
            xmin=*min_element(xcoords.begin(),xcoords.end())-2;
            xmax=*max_element(xcoords.begin(),xcoords.end())+2;
            ymin=*min_element(ycoords.begin(),ycoords.end())-2;
            ymax=*max_element(ycoords.begin(),ycoords.end())+2;
            zmin=*min_element(zcoords.begin(),zcoords.end())-2;
            zmax=*max_element(zcoords.begin(),zcoords.end())+2;
        }
        else
        {
            THROWSTREAM << "Could not find an XYZ block in: " << infile;
        }


        //Done with debugging options for user, time to read/write dplot blocks
        ofstream ofs;

        if (inExt == "nw")
        {
            ofs.open("dplot.nw",ofstream::out);
            if (!ofs.is_open())
            {
                THROWSTREAM << "Error: Cannot open output file: dplot.nw";
            }
            while ( getLine_counter(ifs,line,linecounter) )
            {
                linecopy=line;
                toLower(linecopy);
                if (linecopy.find("task") != string::npos)
                {
                    ofs << "#" << line << endl;
                }
                else
                {
                    ofs << line << endl;
                }
            }
            ofs << endl;
            ifs.close();
        }
        else if (inExt == "pbs")
        {
            ofs.open("dplot.pbs",ofstream::out);
            if (!ofs.is_open())
            {
                THROWSTREAM << "Error: Cannot open file: dplot.pbs";
            }
            while (getLine_counter(ifs,line,linecounter))
            {
                linecopy=line;
                toLower(linecopy);
                if ( linecopy.find("task") != string::npos )
                {
                    ofs << "#" << line << endl;
                }
                else if ( linecopy.find("eor") != string::npos && line.find("cat") == string::npos )
                {
                    ofs << endl;
                    break;
                }
                else if (linecopy.find("#pbs -") != string::npos)
                {
                    //First find out what is written
                    if ( linecopy.find("nodes") != string::npos )
                    {
                        if ( amd )
                        {
                            ofs << "#PBS -l nodes=1:ppn=4" << endl;
                            ofs << "#PBS -l walltime=01:00:00" << endl;
                            ofs << "#PBS -q amd64" << endl;
                        }
                        else if ( debug )
                        {
                            ofs << "#PBS -l nodes=1:ppn=4" << endl;
                            ofs << "#PBS -q debug" << endl;
                        }
                        else
                        {
                            ofs << line << endl;
                        }
                    }
                    else if ( linecopy.find("walltime") != string::npos )
                    {
                        if ( amd || debug )
                        {
                            //Do nothing
                        }
                        else
                        {
                            ofs << line << endl;
                        }
                    }
                    else if ( linecopy.find("pbs -q") != string::npos )
                    {
                        if ( amd || debug )
                        {
                            //Do nothing
                        }
                        else
                        {
                            ofs << line << endl;
                        }
                    }
                    else if ( linecopy.find("pbs -o") != string::npos)
                    {
                        ofs << "#PBS -o dplot.out" << endl;
                    }
                    else
                    {
                        ofs << line << endl;
                    }
                }
                else
                {
                    ofs << line << endl;
                }
            }
        }
        else
        {
            THROWSTREAM << "Error: Unrecognized File Extension of " << infile << " expects '.pbs' or '.nw'";
        }

        //Now we should be in the position to input the dplot blocks for civecs movecs!!
        if ( !list.empty() )
        {
            string vecPref=getPref(vecfile);

            for ( int i = 0; i < list.size(); i++ )
            {
                if ( vecExt == "movecs" && alphabeta == 1 && beta == 0)
                {
                    vector <string> abvec;
                    abvec.push_back("alpha");
                    abvec.push_back("beta");
                    for (int j=0;j<abvec.size();j++)
                    {
                        ofs << "dplot" << endl;
                        ofs << "TITLE " << list[i] << endl;
                        ofs << "vectors " << vecfile << endl;
                        ofs << "LimitXYZ" << endl;
                        ofs << xmin <<  " " << xmax << " " << grid << endl;
                        ofs << ymin <<  " " << ymax << " " << grid << endl;
                        ofs << zmin <<  " " << zmax << " " << grid << endl;
                        ofs << "spin " << abvec[j] << endl;
                        ofs << "gaussian" << endl;                                             
                        ofs << "orbitals view;1;" << list[i] << endl;
                        ofs << "output " << vecPref << "-" << list[i] << "-" << abvec[j] << ".cube" << endl;
                        ofs << "end" << endl;
                        ofs << "task dplot" << endl;
                        ofs << endl;
                    }

                }
                else
                {
                    ofs << "dplot" << endl;
                    if ( civecExt == "civecs_singlet" || civecExt =="civecs_triplet" )
                    {
                        ofs << "civecs " << civecfile << endl;
                        ofs << "vectors " << vecfile << endl;
                        ofs << "root " << list[i] << endl;
                        ofs << "LimitXYZ" << endl;
                        ofs << xmin <<  " " << xmax << " " << grid << endl;
                        ofs << ymin <<  " " << ymax << " " << grid << endl;
                        ofs << zmin <<  " " << zmax << " " << grid << endl;
                        ofs << "gaussian" << endl;
                        ofs << "output " << vecPref << "-root-" << list[i] << ".cube " << endl;
                    }
                    else if ( vecExt == "movecs" && alphabeta == 0 )
                    {                        
                        ofs << "TITLE " << list[i] << endl;
                        ofs << "vectors " << vecfile << endl;
                        ofs << "LimitXYZ" << endl;
                        ofs << xmin <<  " " << xmax << " " << grid << endl;
                        ofs << ymin <<  " " << ymax << " " << grid << endl;
                        ofs << zmin <<  " " << zmax << " " << grid << endl;
                        if ( beta == 1)
                        {
                            ofs << "spin beta" << endl;
                            ofs << "gaussian" << endl;
                            ofs << "orbitals view;1;" << list[i] << endl;
                            ofs << "output " << vecPref << "-beta-" << list[i] << ".cube" << endl;
                        }
                        else
                        {
                            ofs << "spin alpha" << endl;
                            ofs << "gaussian" << endl;
                            ofs << "orbitals view;1;" << list[i] << endl;
                            ofs << "output " << vecPref << "-" << list[i] << ".cube" << endl;
                        }                       
                    }
                    ofs << "end" << endl;
                    ofs << "task dplot" << endl;
                }
                ofs << endl;
            }
        }
        else
        {
            THROWSTREAM << "No list of molecular vectors (movecs) or transition densities (civecs) to plot (-l option)";
        }

        //Finally we need to finish the ifs lines for pbs file
        if ( inExt == "pbs")
        {
            ofs << "eor" << endl;
            ofs << endl;
            ofs << "cp $PBS_O_WORKDIR/*vecs* $PBSTMPDIR/" << endl;

            int addCubeLine = 0;

            while ( getLine_counter(ifs,line,linecounter) )
            {
                linecopy=line;
                toLower(linecopy);
                if ( linecopy.find("cd $pbstmpdir") != string::npos )
                {
                    if ( addCubeLine == 0 )
                    {
                        ofs << line << endl;
                        ofs << "cp $PBSTMPDIR/*.cube $PBS_O_WORKDIR/" << endl;
                        addCubeLine = 1;
                    }
                    else
                    {
                        ofs << line << endl;
                    }
                }
                else
                {
                    ofs << line << endl;
                }
            }
        }
        ifs.close();
        ofs.close();
    }
    catch(std::exception & ex)
    {
        cout << "\nEXCEPTION!\n";
        cout << ex.what();
        cout << "\n\n";
        return 1;
    }
    return 0;
}

