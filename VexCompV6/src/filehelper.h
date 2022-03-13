#include <fstream>
#include <stdlib.h>

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
//free(point*), realloc(point*, newSize)