
#include "array_list.h"
#include <stdlib.h>
#include <stdio.h>

struct array_list * al; 
uint64_t test_set[20] = { 1,2,3,4,5,6,7,8,9,0,9,8,7,6,5,4,3,2,1,0 }; 
uint64_t i;

#define RESET() free_al(al);al=malloc(sizeof(struct array_list)); \
                init_al(20,al);for(i=0;i<20;i++)add_al(test_set[i],al)

int main(void) {

   al = malloc(sizeof(struct array_list));

   printf("TESTING add_al = ");
   init_al(20,al);
   for (i=0; i < 20; i++) {
      add_al(test_set[i],al);
   }
   printf("COMPLETE\n");

   RESET();
   printf("TESTING add_al past capacity = ");
   for (i=0;i<20;i++) {
      add_al(test_set[i],al);
   }
   printf("COMPLETE\n");

   RESET();
   printf("TESTING rem_front_al = ");
   i = rem_front_al(al);
   if (i != 1) printf("fail<1> ");
   i = rem_front_al(al);
   if (i != 2) printf("fail<2> ");
   i = rem_front_al(al);
   if (i != 3) printf("fail<3> ");
   printf("COMPLETE\n");

   RESET();
   printf("TESTING rem_back_al = ");
   i = rem_back_al(al);
   if (i != 0) printf("fail<0> ");
   i = rem_back_al(al);
   if (i != 1) printf("fail<1> ");
   i = rem_back_al(al);
   if (i != 2) printf("fail<2> ");
   printf("COMPLETE\n");

   RESET();
   printf("TESTING rem_front_al then unshift_al trigger = ");
   for (i=0;i<19;i++) {
      add_al(test_set[i],al);
      rem_front_al(al);
   }
   add_al(200,al); 
   add_al(100,al); /* trigger shift */
   if (al->capacity == 80) printf("fail ");
   printf("COMPLETE\n");

   RESET();
   printf("TESTING add_front_al = ");
   for (i=0;i<20;i++) {
      add_front_al(i+100,al);
   }
   i = rem_front_al(al);
   if (i != 119) printf("fail<119> ");
   i = rem_front_al(al);
   if (i != 118) printf("fail<118> ");
   i = rem_front_al(al);
   if (i != 117) printf("fail<117> ");
   printf("COMPLETE\n");

   RESET();
   printf("TESTING find_al = ");
   if (!find_al(0,al)) printf("fail<0> ");
   if (!find_al(9,al)) printf("fail<1> ");
   printf("COMPLETE\n");

   RESET();
   printf("TESTING find_al with altered head = ");
   for (i=0;i<5;i++) rem_back_al(al);
   if (!find_al(0,al)) printf("fail<0> ");
   if (!find_al(9,al)) printf("fail<1> ");
   printf("COMPLETE\n");

   uint64_t options1[2] = { 1,2 };
   uint64_t options2[8] = { 1,2,3,4,5,6,7,8 };
   uint64_t options3[8] = { 1,200,3,400,5,600,7,800 };
   RESET();
   printf("TESTING find_multiple_al = ");
   if (find_multiple_al(options1,2,al) != 3) printf("fail<1,2> ");
   if (find_multiple_al(options2,8,al) != 0xff) printf("fail<1...8> ");
   if (find_multiple_al(options3,8,al) != 0x55) printf("fail<1...8/h>" );
   printf("COMPLETE\n");

   RESET();
   printf("TESTING rem_element_al = ");
   for (i=0;i<5;i++) rem_back_al(al);
   rem_element_al(1,al);
   if (find_al(1,al)) printf("fail<1> ");
   rem_element_al(3,al);
   if (find_al(3,al)) printf("fail<3> ");
   printf("COMPLETE\n");

   RESET();
   printf("TESTING rem_all_element_al = ");
   rem_all_element_al(1,al);
   if (find_al(1,al)) printf("fail<1> ");
   rem_all_element_al(3,al);
   if (find_al(3,al)) printf("fail<3> ");
   printf("COMPLETE\n");

   RESET();
   printf("TESTING fast_rem_all_element_al = ");
   fast_rem_all_element_al(1,al);
   if (find_al(1,al)) printf("fail<1> ");
   rem_all_element_al(3,al);
   if (find_al(3,al)) printf("fail<3> ");
   printf("COMPLETE\n");

   RESET();
   printf("TESTING fast_rem_all_above_element_al = ");
   fast_rem_all_above_element_al(0,al);
   if (al->elements != 2) printf("fail<0> ");
   printf("COMPLETE\n");

   RESET();
   printf("TESTING fast_rem_all_below_element_al = ");
   fast_rem_all_below_element_al(10,al);
   if (al->elements) printf("fail<10> ");
   printf("COMPLETE\n");

   RESET();
   printf("TESTING replace_all_element_al = ");
   replace_all_element_al(9,0,al);
   if (find_al(9,al)) printf("fail<9> ");
   rem_all_element_al(0,al);
   if (al->elements != 16) printf("fail<0> ");
   printf("COMPLETE\n");

   uint64_t expected[20] = {0,0,1,1,2,2,3,3,4,4,
                            5,5,6,6,7,7,8,8,9,9};
   RESET();
   printf("TESTING sort_al = ");
   sort_al(al);
   for (int i = 0; i < 20; i++) {
      if (expected[i] != al->list[i]) printf("fail<%d> ",i);
   }
   printf("COMPLETE\n");


   return 0;
}
