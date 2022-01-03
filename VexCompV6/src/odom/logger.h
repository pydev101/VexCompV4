#include <iostream>
#include <sstream>
#include "robotmath.h"
#include "filehelper.h"

typedef struct {
    Point p;
    const char* color;
} PointPair;

typedef struct{
    Point p;
    Vector v;
    const char* color;
} VectorPair;

typedef struct{
  double error;
  PIDOutput pidOutput;
  double realTargetVel;
  double realVel;
} PIDData;

class Graph{
public:
    PointPair* points = (PointPair*)malloc(0);
    int numOfPoints = 0;
    VectorPair* vectorlist = (VectorPair*)malloc(0);
    int numOfVectors = 0;
    PIDData* linearPIDData = (PIDData*)malloc(0);
    int numOfLinPIDData = 0;
    PIDData* rotPIDData = (PIDData*)malloc(0);
    int numOfRotPIDData = 0;
    File f = File();

    Graph(const char* fileName, vex::brain* vexbrain){
      f = File(fileName, vexbrain);
    }

    ~Graph() {
        free(points);
        free(vectorlist);
        free(linearPIDData);
        free(rotPIDData);
    }

    void clear() {
        free(points);
        points = (PointPair*)malloc(0);
        numOfPoints = 0;

        free(vectorlist);
        vectorlist = (VectorPair*)malloc(0);
        numOfVectors = 0;

        free(linearPIDData);
        linearPIDData = (PIDData*)malloc(0);
        numOfLinPIDData = 0;

        free(rotPIDData);
        rotPIDData = (PIDData*)malloc(0);
        numOfRotPIDData = 0;
    }

    void addVector(VectorPair x) {
        numOfVectors++;
        vectorlist = (VectorPair*)realloc(vectorlist, sizeof(VectorPair) * numOfVectors);
        vectorlist[numOfVectors - 1] = x;
    }

    void addPoint(PointPair x) {
        numOfPoints++;
        points = (PointPair*)realloc(points, sizeof(PointPair) * numOfPoints);
        points[numOfPoints - 1] = x;
    }

    void addPID(PIDData data, bool linearPID) {
      if(linearPID){
        numOfLinPIDData++;
        linearPIDData = (PIDData*)realloc(linearPIDData, sizeof(PIDData) * numOfLinPIDData);
        linearPIDData[numOfLinPIDData - 1] = data;
      }else{
        numOfRotPIDData++;
        rotPIDData = (PIDData*)realloc(rotPIDData, sizeof(PIDData) * numOfRotPIDData);
        rotPIDData[numOfRotPIDData - 1] = data;
      }
    }

    std::string getString() {
        std::ostringstream strs;
        for (int i = 0; i < numOfVectors; i++) {
            VectorPair v = vectorlist[i];
            strs << "V:" << v.p.x << "," << v.p.y << "," << v.v.getX() << "," << v.v.getY() << "," << v.color << std::endl;
        }
        for (int i = 0; i < numOfPoints; i++) {
            PointPair p = points[i];
            strs << "P:" << p.p.x << "," << p.p.y << "," << p.color << std::endl;
        }
        for (int i = 0; i < numOfLinPIDData; i++) {
            PIDData d = linearPIDData[i];
            strs << "LPID:" << d.error << "," << d.pidOutput.output << "," << d.pidOutput.reset << "," << d.realTargetVel << "," << d.realVel << std::endl;
        }
        for (int i = 0; i < numOfRotPIDData; i++) {
            PIDData d = rotPIDData[i];
            strs << "RPID:" << d.error << "," << d.pidOutput.output << "," << d.pidOutput.reset << "," << d.realTargetVel << "," << d.realVel << std::endl;
        }
        strs << "END:END" << std::endl;
        return strs.str();
    }


      /*
      void writeCommand(double cmd, double arg, bool isStart=false){
          if(isStart){
            sprintf(buffer, "%g,%g;", cmd, arg);
            return;
          }
          sprintf(buffer, "%s%g,%g;", buffer, cmd, arg);
      }
        int size = sizeof(char)*(strlen(buffer)+1);
        char* charToPrint = (char*)malloc(size);
        strcpy (charToPrint, buffer);
        Brain.SDcard.savefile("recorded.csv", reinterpret_cast<unsigned char*>(charToPrint), size);
        free(charToPrint);*/
};


/*
  int size = Brain.SDcard.size("recorded.csv");
  int length = size/sizeof(uint8_t);
  uint8_t* rawFileData = (uint8_t*)malloc(size);
  char* charList = (char*)malloc(sizeof(char)*length);

  Brain.SDcard.loadfile("recorded.csv", rawFileData, size);

  for(int i=0; i<length; i++){
      charList[i] = static_cast<char>(rawFileData[i]);
  }
  free(rawFileData);*/

/*
class PathLogger{
public:
  std::string fileName;
  smartPointPointer path;
  PathLogger(std::string name){
    fileName = name;
  }
  void read(){
    std::ifstream file("my_file");
    std::string temp;

    while(std::getline(file, temp)) {
      int i = 0;
      for ( ; i < temp.length(); i++ ){ if ( temp[i] == ',' ) break; }
      if(i > 0){
        double x = atof(temp.substr(0, i-1 ).c_str());
        double y = atof(temp.substr(i+1, temp.length()-1 ).c_str());
        path.append(Point(x, y));
      }
    }
    file.close();
  }
  void save(){
    std::ofstream file;
    file.open(fileName);
    file.clear();
    for(int i=0; i<path.size; i++){
      file << path[i].x << "," << path[i].y << std::endl;
    }
    file.close();
  }
  void output(int format=0){
    if(format==0){
      for(int i=0; i<path.size; i++){
        std::cout << "P:" << path[i].x << "," << path[i].y << ",green" << std::endl;
      }
      std::cout << "END:END" << std::endl;
    }else if (format==1) {
      for(int i=0; i<path.size; i++){
        std::cout << "(" << path[i].x << "," << path[i].y << "),";
      }
      std::cout << std::endl;
    }
  }
};*/