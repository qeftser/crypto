
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

uint64_t gcd(uint64_t x, uint64_t y) {
   uint64_t g = y;

   while (x > 0) {
      g = x;
      x = y % x;
      y = g;
   }

   return g;
}

#define g(x,n) ((x * x) + 1) % n

int64_t pollard_monte_carlo(uint64_t n) {
   int64_t x = 2;
   int64_t y = x;
   uint64_t d = 1;

   while (d == 1) {
      x = g(x,n);
      y = g(y,n);
      y = g(y,n);
      d = gcd(labs(x - y), n);
   }
   if (d == n) return 0;
   else return d;
}

int main(void) {

   uint64_t n = 10403;

   long int factor = pollard_monte_carlo(n);

   printf("%lu factored: %ld\n",n,factor);


   return 0;
}
