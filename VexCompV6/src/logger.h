#include <stdio.h>
#include <stdlib.h>

class Logger{
  char* output = (char*)malloc(sizeof(char)*11);

  ~Logger(){
    free(output);
  }

};