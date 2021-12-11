#include <iostream>
#include <string>
#include <sstream>
#include "command.h"
//#include "blocklist.h"
using namespace std;
int main() {
    int num;
    string str;
    getline(cin,str);
    stringstream ss;
    ss.clear();
    ss.str(str);
    ss>>num;
    for(int i=0;i<num;++i){
        string command;
        command="\n";
        getline(cin,command);
        CarryOut(command + "\n");
    }
    return 0;
}
