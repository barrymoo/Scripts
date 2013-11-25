#include <string>

using namespace std;

//Class for CubeFiles
class CubeFile{
    private:
        int i;
    public:
        void set(string filename);
        int get();        
};

void CubeFile::set(string filename){
    i=1;
}

int CubeFile::get(){
    return i;
}

