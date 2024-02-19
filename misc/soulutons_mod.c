
#include <stdio.h>
#include <stdlib.h>

int power_algo(int a, int e, int n) {
   int A, E, P = 1;
   A = a; E = e;
   while (1) {
      if (!E) return P;
      if (E % 2 == 1) {
         P = (A * P) % n;
         E = (E - 1) / 2;
      }
      else {
         E /= 2;
      }
      A = (A * A) % n;
   }
}

int function(int a, int p) {
   int k = (p - 3) / 4;
   int B = power_algo(a,k+1,p);
   if ((B * B) % p == a % p) return B;
   return -1;
}

int main(int argc, char **argv) {
   if (argc < 3) {
      printf("usage:\nakmop <int a> <int p>\n");
      exit(1);
   }

   int ret = function(atoi(argv[1]),atoi(argv[2]));
   if (ret == -1) 
      printf("no solution\n");
   else
      printf("%d\n",ret);


   return 0;
}
