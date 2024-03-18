
#include "factor.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv) {
   if (argc < 2) {
      printf("Usage:\nfactor_base <B - max>\n");
      exit(0);
   }
   int B = atoi(argv[1]);

   FILE *out;

   out = fopen("factor_list.txt","w+");
   if (out == NULL) {
      printf("failed creating file\n");
      exit(1);
   }

   uint64_t i = 2;
   while (B) {
      if (pollard_monte_carlo(i) == 0) {
         fprintf(out,"%ld ",i);
         B--;
      }
      i++;
   }
   if (EOF == fclose(out)) {
      printf("failed closing file\n");
      exit(1);
   }

   return 0;
}

