#include <stdlib.h>
#include <sstream>
#include <iostream>
using namespace std;


class FileHelper : public stringstream{
    void save() {
        string a = str();
        cout << a << endl;
    }
    void load() {
        for (int i = 0, i < 6; i++) {
            this << i << endl;
        }
    }
};

int main(){
    

    return 0;
}