#include<stdio.h>
#include<stdlib.h>

#include "constants.h"

int main() {
  FILE* in = fopen(INFILE, "r");
  FILE* out = fopen(OUTFILE, "r");

  char a, b;
  int r = 0;
  while ((a = fgetc(in) != EOF) && (b = fgetc(out) != EOF)) {
    if (a == EOF && b != EOF) {
      r++;
    } else if (a != EOF && b == EOF) {
      r++;
    } else {
      if (a != b) {
	printf("DIFFERENCE! %c VS %c\n", a, b);
	r++;
      }
    }
  }
  
  fclose(in);
  fclose(out);

  printf("TOTAL DIFFERENCES : %d\n", r);

  exit(0);
}
