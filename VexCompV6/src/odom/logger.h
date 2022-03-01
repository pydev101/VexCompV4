#include <iostream>
#include <sstream>
#include "robotmath.h"
#include <fstream>

/*
Name: logger.h
Written By: Carson Easterling

Custom logging libary written for use with my python graphing program. Maps vectors and points onto a grid, and PID control values on to graphs can be exported in file or terminal format.

*/

class Log : public std::ostringstream{
public:
  const char* fileName;

  Log(const char* logName){
    fileName = logName;
    str("");
  }

  void clear(){
    str("");
  }

  void print(bool clearBuf=true){
    std::cout << str() << std::flush;
    if(clearBuf){
      clear();
    }
  }

  void save(bool clearBuf=true){
    std::ofstream file(fileName);
    if(file.is_open()){
      file.clear();
      file << str() << std::flush;
      file.close();
    }
    if(clearBuf){
      clear();
    }
  }

  void append(bool clearBuf=true){
    std::ofstream file(fileName, std::ios_base::app);
    if(file.is_open()){
      file <<str() << std::flush;
      file.close();

      if(clearBuf){
        clear();
      }
    }
  }
};

class PythonProgramLogger{
  protected:
  const char* graphName;
  Log logger;

  public:
  PythonProgramLogger(const char* logName) : logger(logName) {
    graphName = logName;
  }

  void graph(){
    logger << "END:END" << std::endl;
  }

  void addPoint(Point p, const char* color){
    logger << "P:" << p.x << "," << p.y << "," << color << std::endl;
  }

  void addVector(Point p, Vector v, const char* color){
    logger << "V:" << p.x << "," << p.y << "," << v.getX() << "," << v.getY() << "," << color << std::endl;
  }

  void save(bool clearBuffer=true){
    logger.save(clearBuffer);
  }

  void append(bool clearBuffer=true){
    logger.append(clearBuffer);
  }

  void print(bool clearBuffer=true){
    logger.print(clearBuffer);
  }
};


typedef struct{
  double error;
  PIDOutput pidOutput;
  double realTargetVel;
  double realVel;
} PIDData;

class Graph{
public:
    PIDData* linearPIDData = (PIDData*)malloc(0);
    int numOfLinPIDData = 0;
    PIDData* rotPIDData = (PIDData*)malloc(0);
    int numOfRotPIDData = 0;
    std::ofstream file;

    Graph(const char* fileName){
      file.open(fileName, std::fstream::out | std::fstream::app);
    }

    ~Graph() {
        free(linearPIDData);
        free(rotPIDData);
        file.close();
    }

    void clear() {
        free(linearPIDData);
        linearPIDData = (PIDData*)malloc(0);
        numOfLinPIDData = 0;

        free(rotPIDData);
        rotPIDData = (PIDData*)malloc(0);
        numOfRotPIDData = 0;
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