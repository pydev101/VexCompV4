#include <stdio.h>
#include <string.h>

//Data file format
/*
DATA\n
int 56863\n
double 53.33\n
string 6 Hello\0\n      //Length/Size of String ; Reads into buffer until it hits null
bool 1\n                //1 for true, 0 for false
*/

//Log file format
/*
HEADERA,HEADERB,HEADERC\n
DATA1A,DATA1B,DATA1C\n
DATA2A,DATA3B,DATA2C\n
DATA3A,DATA2B,DATA3C\n
*/

class FileManager{
  char* buffer;
  int buffSize;
  FILE *f;
  int formatType; //0 if DATA, 1 if CSV

  FileManager(char *fileName){
    
  }

  bool status(){
    if (f == NULL) {
      return false;
    }else{
      return true;
    }
  }

  int clear(){

  }

  int write(){

  }
};

int main() {
   int i, x = 4;
   char s[20];
   FILE *f = fopen("new.txt", "w");

   for (i=0; i<x; i++) {
      puts("Enter text");
      gets(s);
      fprintf(f,"%d.%s\n", i, s);
   }
   fclose(f);
   return 0;
}