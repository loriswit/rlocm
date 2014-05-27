#include "seed.h"

/****************************************/
/// class seed
/****************************************/

using namespace std;

seed::seed(const string& _str)
{
    // convert a hexadecimal string to 4 bytes
    string str = _str;
    for(unsigned int i=0; i<str.size(); i++)
        if(str[i] == ' ') str.erase(i,1);

    for(int i=0; i<4; i++){
        stringstream sstr;
        int byte;

        sstr << str[i*2] << str[i*2+1];
        sstr >> hex >> byte;
        set(i,byte);
    }
}

int seed::operator[](size_t pos)
{
    switch(pos){
        case 0: return a;
        case 1: return b;
        case 2: return c;
        case 3: return d;
        default: return 0;
    }
}

void seed::operator=(seed s)
{
    a = s[0];
    b = s[1];
    c = s[2];
    d = s[3];
}

void seed::set(size_t pos, int i)
{
    switch(pos){
        case 0: a = i; break;
        case 1: b = i; break;
        case 2: c = i; break;
        case 3: d = i; break;
    }
}

string seed::to_str(void)
{
    ostringstream sstr;
    for(int i=0; i<4; i++){
        sstr << hex << uppercase << setw(2) << setfill('0') << operator[](i);
        if(i<3) sstr << " ";
    }
    return sstr.str();
}
