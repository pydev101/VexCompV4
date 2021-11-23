#include <stdio.h>

class FileManager{
  char* buffer;
  int buffSize;
  FILE *f;


};

#include<stdio.h>
int main() {
   int i, x = 4;
   char s[20];
   FILE *f = fopen("new.txt", "w");
   if (f == NULL) {
      printf("Could not open file");
      return 0;
   }
   for (i=0; i<x; i++) {
      puts("Enter text");
      gets(s);
      fprintf(f,"%d.%s\n", i, s);
   }
   fclose(f);
   return 0;
}