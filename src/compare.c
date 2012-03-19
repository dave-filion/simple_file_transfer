#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "constants.h"

int main(int argc, char* argv[]) {
   if (argc != 3) {
      printf("Usage: compare <before> <after>\n");
      exit(-1);
   } else {         
      FILE* in = fopen(argv[1], "r");
      FILE* out = fopen(argv[2], "r");

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
}
