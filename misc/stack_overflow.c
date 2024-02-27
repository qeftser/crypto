
#include <stdio.h>

int stack_overflow(int prev) {
   printf("current iteration: %d\n",prev);
   return stack_overflow(prev+1);
}

int main(void) {

   stack_overflow(0);

   return 0;
}


