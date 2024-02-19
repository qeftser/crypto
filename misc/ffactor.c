
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int ffactor(int n, int *X, int *Y) {
   int i_y, i_x, nd2 = (n + 1) / 2;
   double y,x;

   if (n % 2 == 0) return 3;
   x = sqrtf((double)n);
   i_x = (int) x;
   if (x - (double) i_x < 0.00001) {
      *X = i_x;
      return 0;
   }
   x++;

   while (i_x < nd2) {
      y = sqrtf((double)(i_x * i_x) - n);
      i_y = (int) y;
      if (y - (double) i_y < 0.00001) {
         *X = i_x;
         *Y = i_y;
         return 1;
      }
      i_x++;
   }

   return 2;
}

int main(int argc, char **argv) {
   if (argc < 2) {
      printf("usage:\nffactor <int n>\n");
   }

   int X, Y;
   int ret = ffactor(atoi(argv[1]),&X,&Y);
   if (!ret) {
      printf("%d\n",X);
   }
   else if (ret == 1) {
      printf("%10d\t%10d\n",(X + Y), (X - Y));
   }
   else if (ret == 2) {
      printf("prime\n");
   }
   else {
      printf("even\n");
   }
   return 1;
}
