#include<stdio.h>
#include"constants.h"

#define NUM 1000
#define BEGIN 48
#define END 122

int main(){

  FILE *f = fopen(INFILE, "w");

  int i;  
  int j = BEGIN;
  for (i = 0; i < NUM; i++) {
    if (j == END) j = BEGIN;
    fputc((char) j, f);
    j++;
  }

  fclose(f);

  return 0;
}
