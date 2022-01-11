#include <stdlib.h>
#include <sstream>

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
      if(!(vexbrain->SDcard.exists(fileName.c_str()))){
        clear();
      }
      vexbrain->SDcard.appendfile(fileName.c_str(), reinterpret_cast<unsigned char*>(&data[0]), size);
    }

    void clear(){
      write("");
    }

    std::string load() {
      std::string out = "";
      if(vexbrain->SDcard.exists(fileName.c_str())){
        int size = vexbrain->SDcard.size(fileName.c_str());
        int length = size/sizeof(uint8_t);
        uint8_t* rawFileData = (uint8_t*)malloc(size);

        vexbrain->SDcard.loadfile(fileName.c_str(), rawFileData, size);

        for(int i=0; i<length; i++){
            out += static_cast<char>(rawFileData[i]);
        }

        free(rawFileData);
      }
      return out;
    }
};