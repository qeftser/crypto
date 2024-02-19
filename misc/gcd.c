
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int gcd(int a, int b) {
   int a_i, b_i, r;
   if (a < b) {
      a_i = b; b_i = a; r = b_i;
   }
   else {
      a_i = a; b_i = b; r = b_i;
   }
   r = a_i % b_i;
   while (r) {
      a_i = b_i; b_i = r;
      r = a_i % b_i;
   } 
   return b_i;
}

int gcd_extended(int a, int b, int *A, int *B) {
   int a_i, b_i, r, q, x0, x1, y0, y1, temp;
   x1 = 1; y1 = 0; x0 = 0; y0 = 1;
   if (a < b) {
      a_i = b; b_i = a; r = b_i;
   }
   else {
      a_i = a; b_i = b; r = b_i;
   }
   r = a_i % b_i;
   q = a_i / b_i;
   while (r) {
      temp = x0;
      x0 = x1 - (q * x0);
      x1 = temp;

      temp = y0;
      y0 = y1 - (q * y0);
      y1 = temp;

      a_i = b_i; b_i = r;
      r = a_i % b_i;
      q = a_i / b_i;
   } 
   *A = x0; *B = y0;
   return b_i;
}

int main(int argc, char **argv) {
   if (argc < 3) {
      printf("usage:\ngcd <num 1> <num 2> [-e]\n");
      exit(0);
   }
   int ret, A, B;
   if (argc > 3 && strcmp("-e",argv[3]) == 0) {
      ret = gcd_extended(atoi(argv[1]),atoi(argv[2]), &A, &B);
      printf("%10d\t%10d\t%10d\n",ret,A,B);
   }
   else {
      ret = gcd(atoi(argv[1]),atoi(argv[2]));
      printf("%d\n",ret);
   }
}
