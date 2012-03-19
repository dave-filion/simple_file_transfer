#include<stdio.h>
#include"constants.h"

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
