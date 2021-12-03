#include <iostream>
#include <fstream>
#include <typeinfo>

typedef struct Storage {
    int x;
    double z;
};

void out(Storage z) {
    std::cout << z.x << ", " << z.z << std::endl;
}

class FileManager {
public:
    std::string fileName;

    FileManager(std::string filename) {
        fileName = filename;
    }

    void Dump(void* data, int size) {
        std::ofstream f(fileName, std::ios::binary);
        f.write((char*)data, size);
        f.close();
    }

    char* Load(int size) {
        std::ifstream f(fileName, std::ios::binary);
        char* b = (char*)malloc(size);
        f.read(b, size);
        f.close();
        return b;
    }
};

int main(){
    Storage test = { 28, 36.565 };
    out(test);

    FileManager bob = FileManager("dump.blm");
    bob.Dump(&test, sizeof(test));


    Storage* s = (Storage*)bob.Load(sizeof(test));
    Storage hm = *s;
    free(s);
    out(hm);

    return 0;
}