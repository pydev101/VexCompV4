#include <stdlib.h>

class File{
public:
    std::string fileName;
    vex::brain* vexbrain;

    File() {
    }

    File(std::string filename, vex::brain* b) {
        fileName = filename;
        vexbrain = b;
    }

    void write(std::string data) {
      int size = data.size();
      vexbrain->SDcard.savefile(fileName.c_str(), reinterpret_cast<unsigned char*>(&data[0]), size);
    }

    void append(std::string data){
      int size = data.size();
      vexbrain->SDcard.appendfile(fileName.c_str(), reinterpret_cast<unsigned char*>(&data[0]), size);
    }

    void clear(){
      write("");
    }

    std::string load() {
      return "";
    }
};