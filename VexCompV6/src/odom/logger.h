#include <iostream>
#include <sstream>
#include "robotmath.h"
#include <fstream>

/*
Name: logger.h
Written By: Carson Easterling

Custom logging libary written for use with my python graphing program. Maps vectors and points onto a grid, and PID control values on to graphs can be exported in file or terminal format.

*/

class Log : public std::ostream{
protected:
  const char* fileName;
  std::stringbuf contents;
public:

  Log(const char* logName){
    fileName = logName;
    rdbuf(&contents);
    contents.str("");
  }

  void clear(){
    contents.str("");
  }

  void print(){
    std::cout << contents.str() << std::flush;
  }

  void save(){
    std::ofstream file(fileName);
    if(file.is_open()){
      file.clear();
      file << contents.str() << std::flush;
      file.close();
    }
  }

  void append(bool clearBuf=true){
    std::ofstream file(fileName, std::ios_base::app);
    if(file.is_open()){
      file << contents.str() << std::flush;
      file.close();

      if(clearBuf){
        clear();
      }
    }
  }
};

class PythonProgramLogger : protected Log{
  PythonProgramLogger(const char* logName) : Log(logName){
    save();
  }

  void markEnd(){
    contents << "END:END" << std::endl;
  }

  PythonProgramLogger operator << (Point &p){
      
  }

  PythonProgramLogger operator << (Vector &p){
      
  }
};

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
    std::ofstream file;

    Graph(const char* fileName){
      file.open(fileName, std::fstream::out | std::fstream::app);
    }

    ~Graph() {
        free(points);
        free(vectorlist);
        free(linearPIDData);
        free(rotPIDData);
        file.close();
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
};