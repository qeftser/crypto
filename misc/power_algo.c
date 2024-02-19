
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

int main(int argc, char **argv) {
   if (argc < 4) {
      printf("usage:\nmodpow <int a> <int k> <int n>\n");
      exit(1);
   }

   int ret = power_algo(atoi(argv[1]),atoi(argv[2]),atoi(argv[3]));
   printf("%d\n",ret);

   return 0;
}
