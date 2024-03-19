
#include "factor.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <pthread.h>
#include <string.h>

const int THREADS = 8;
struct thread_info { uint64_t start; uint64_t end; char id[20]; };

const unsigned long int SIZE = 0x000000ffffffffff;

static void * thread_process(void * arg) {
   struct thread_info * data = (struct thread_info *)arg;

   FILE * out;

   out = fopen(data->id,"w+");
   if (out == NULL) {
      printf("failed creating file\n");
      exit(1);
   }

   printf("created file %s\n",data->id);

   for (uint64_t i = data->start; i < data->end; i++) {
   //   printf("%016lx\n",i);
      if (i % 2 == 0 ||
          pollard_monte_carlo(2,i) ||
          pollard_monte_carlo(3,i) ||
          pollard_monte_carlo(4,i) ||
          pollard_monte_carlo(5,i) ||
          pollard_monte_carlo(6,i) ||
          pollard_monte_carlo(7,i) ||
          pollard_monte_carlo(8,i) ||
          pollard_monte_carlo(9,i)) {
      }
      else fprintf(out,"%lu\n",i);
   }

   if (EOF == fclose(out)) {
      printf("failed closing file\n");
      exit(1);
   }

   printf("closed file %s\n",data->id);

   return data;
}


int main(void) {

   uint64_t * a = malloc(sizeof(uint64_t)*SIZE);
   pthread_t threads[THREADS];
   pthread_attr_t attr;

   char names[20] = "factor_list_x_.txt";

   /* thread setup */
   pthread_attr_init(&attr);
   pthread_attr_setstacksize(&attr,128);
   pthread_attr_setguardsize(&attr,0);

   void * res = 0;

   uint64_t interval = SIZE / THREADS;
   struct thread_info data[THREADS];
   for (int i = 0; i < THREADS; i++) {
      data[i].start = interval*i;
      data[i].end = interval*(i+1);
      names[12] = '0' + i;
      strcpy(data[i].id,names);
   }
   data[THREADS-1].end = SIZE;
   data[0].start = 2;

   printf("total numbers to process: %lx\n",SIZE);


   for (int i = 0; i < THREADS; i++) {
      pthread_create(&threads[i],&attr,thread_process,&data[i]);
   }

   for (int i = 0; i < THREADS; i++) {
      pthread_join(threads[i],res);
      free(res);
   }

   printf("threads complete - joining\n");

   system("cat factor_list_0_.txt factor_list_1_.txt"
          "    factor_list_2_.txt factor_list_3_.txt"
          "    factor_list_4_.txt factor_list_5_.txt"
          "    factor_list_6_.txt factor_list_7_.txt > factor_list.txt");
   system("rm factor_list_[0-8]_.txt");

   return 0;
}

