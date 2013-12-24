#include <iostream>
#include <string>
#include <vector>
#include "ThrowStream.h"
#include <fstream>
#include <algorithm>

using namespace std;

string getExt(string pathName)
{
    size_t period = pathName.find_last_of(".");
    if ( period != string::npos )
    {
        string ext = pathName.substr(period + 1);
        return ext;
    }
    else if (period == string::npos )
    {
        return pathName;
    }
    else
    {
        THROWSTREAM << "Error: " << pathName << " expecting file extension";
    }
}

string getPref_toUpper(string pathName)
{
    size_t period = pathName.find_last_of(".");
    if ( period != string::npos )
    {
        string ext = pathName.substr(0,period);
        transform(ext.begin(),ext.end(),ext.begin(),::toupper);
        return ext;
    }
    else
    {
        THROWSTREAM << "Error: " << pathName << " expecting file extension";
    }
}


struct fitting_data{
    string filename;
    int cationcoeff;
    int anioncoeff;
};

int main(int argc, char ** argv)
{
    try
    {
        vector<fitting_data> data;
        vector<int> datfilenum;
        
        if ( argc == 1 )
        { 
          cout << "==============================" << endl;
          cout << "Stop: Usage -> ./frac-gen *dat" << endl;
          THROWSTREAM << "No command line arguments given";
        }

        for (int i=1; i < argc ; i++)
        {
            if ( getExt(argv[i]) == "dat" )
            {
                datfilenum.push_back(i);
            }
            else
            {
                cout << "Ignoring files without *.dat extension: " << argv[i] << endl;
            }
        }

        data.resize(datfilenum.size());

        for (int i=0; i < datfilenum.size() ; i++)
        {
            data[i].filename = argv[datfilenum[i]];
        }

        ofstream ofs;
        ofs.open("fractional.plt");
        if (!ofs.is_open())
        {
            THROWSTREAM << "Cannot open fractional.plt for writing";
        }

        ofs << "set term postscript eps color solid enhanced \"Helvetica\" 16" << endl << endl;

        int coeff=0;

        for (int i=0; i < data.size(); i++)
        {
            ofs << "############ Fitting " << data[i].filename << " ###########" << endl;
            ofs << "f" << i << "ipr(x) = a" << coeff;
            coeff+=1;
            ofs << " + a" << coeff;
            coeff+=1;
            ofs << " * x + a" << coeff;
            coeff+=1;
            ofs << " * x**2" << endl;
            ofs << "fit [0:-1] f" << i << "ipr(x) '" << data[i].filename << "' using 1:2 via a" << coeff-3 << ", a" << coeff-2 << ", a" << coeff-1 << endl;
            ofs << "f" << i << "ip(x) = (x<0) ? f" << i << "ipr(x) : (1/0)" << endl;
            ofs << endl;

            ofs << "f" << i << "ear(x) = a" << coeff;
            coeff+=1;
            ofs << " + a" << coeff;
            coeff+=1;
            ofs << " * x + a" << coeff;
            coeff+=1;
            ofs << " * x**2" << endl;
            ofs << "fit [1:0] f" << i << "ear(x) '" << data[i].filename << "' using 1:2 via a" << coeff-3 << ", a" << coeff-2 << ", a" << coeff-1 << endl;
            ofs << "f" << i << "ea(x) = (x>0) ? f" << i << "ear(x) : (1/0)" << endl;
            ofs << "##########################################" << endl;
            ofs << endl;
            data[i].cationcoeff = coeff-4;
            data[i].anioncoeff = coeff-1;
        }

        ofs << "set out 'fractional.eps'" << endl;
        ofs << "set key top right" << endl;
        ofs << "set pointsize 1.1" << endl;
        ofs << "#set yrange [0:1]" << endl;
        ofs << "set size ratio 1.5" << endl;
        ofs << "set xzeroaxis" << endl;
        ofs << "set ytics 1" << endl;
        ofs << "set xlabel \"{/Symbol \\104}N\"" << endl;
        ofs << "set ylabel \"[E({/Symbol \\104}N)-E(0)] / eV\"" << endl;
        ofs << "set xrange [-1:1] noreverse" << endl;

        ofs << "plot ";

        ofs << "'" << data[0].filename << "' u 1:2 t sprintf('%s(%4.2f,%4.2f)',\"" << getPref_toUpper(data[0].filename) << "\"";
        ofs << ",a" << data[0].cationcoeff << ",a" << data[0].anioncoeff << ") w p lt -1 lw 2 pt 1";
        ofs << ", f0ip(x) w l lt -1 lw 2 notitle, f0ea(x) w l lt -1 lw 2 notitle";

        if (data.size() > 1)
        {
            ofs << ", ";
            for (int i=1; i < data.size(); i++)
            {
                ofs << "'" << data[i].filename << "' u 1:2 t sprintf('%s(%4.2f,%4.2f)',\"" << getPref_toUpper(data[i].filename) << "\"";
                ofs << ",a" << data[i].cationcoeff << ",a" << data[i].anioncoeff << ") w p lt " << i << " lw 2 pt " << i;
                ofs << ", f" << i << "ip(x) w l lt " << i << " lw 2 notitle, f" << i << "ea(x) w l lt " << i << " lw 2 notitle";
                if ( i == data.size()-1 )
                {
                    ofs << endl;
                }
                else
                {
                    ofs << ", ";
                }
            }
        }
        else
        {
            ofs << endl;
        }

        ofs.close();

        return 0;
    }
    catch(std::exception & ex)
    {
        cout << "\nEXCEPTION!\n";
        cout << ex.what();
        cout << "\n\n";
        return 1;
    }
}

