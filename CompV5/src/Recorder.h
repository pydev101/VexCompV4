#include "vex.h"
#include <fstream>

using namespace vex;

char mode = 'w';
//FIL* x = vexFileOpen("Hello", &mode); test if other doesn't work
std::ofstream myfile;
void test(){
  myfile.open("/usd/example.txt", std::ios::out);
  myfile << "Hello World" << std::endl;
  myfile.close();
}