
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "array_list.h"

void init_al(uint64_t init_capacity, struct array_list *al) {
   al->list = malloc(sizeof(AL_TYPE)*init_capacity);
   if (al->list == NULL) {
      fprintf(stderr,"ERROR: Unable to initialize array list\n");
      exit(1);
   }
   al->rear = 0;
   al->head = 0;
   al->elements = 0;
   al->capacity = init_capacity;
}

void free_al(struct array_list *al) {
   free(al->list);
   free(al);
}

void expand_al(struct array_list *al) {
   al->list = reallocarray(al->list,sizeof(AL_TYPE)*al->capacity*2,sizeof(AL_TYPE));
   if (al->list == NULL) {
      fprintf(stderr,"ERROR: Unable to realocate data\n");
      perror(0);
      exit(1);
   }
   al->capacity *= 2;
}

void unshift_al(struct array_list *al) {
   for (uint64_t i = 0; i < al->elements; i++) {
      al->list[i] = al->list[i+al->head];
   }
   al->head = 0;
   al->rear = al->elements;
}

void maintenance_al(struct array_list *al) {
   if (al->elements == al->capacity) {
      expand_al(al);
   }
   else if (al->rear == al->capacity) {
      unshift_al(al);
   }
}

void add_al(AL_TYPE element, struct array_list *al) {
   maintenance_al(al);
   al->list[al->rear++] = element;
   al->elements++;
}

void add_front_al(AL_TYPE element, struct array_list *al) {
   maintenance_al(al);
   if (al->head) {
      al->list[--(al->head)] = element;
   }
   else {
      /* this is the slow case */
      al->rear++;
      for (uint64_t i = al->rear; i > al->head; i--) {
         al->list[i] = al->list[i-1];
      }
      al->list[al->head] = element;
   }
   al->elements++;
}

AL_TYPE rem_front_al(struct array_list *al) {
   al->head++;
   al->elements--;
   return al->list[al->head-1];
}

AL_TYPE rem_back_al(struct array_list *al) {
   al->rear--;
   al->elements--;
   return al->list[al->rear];
}

AL_TYPE rem_element_al(AL_TYPE element, struct array_list *al) {
   /* this is a slow one as well */
   for (uint64_t i = al->head; i < al->rear; i++) {
      if (AL_COMPARE(al->list[i],element)==0) {
         for (uint64_t j = i; j < al->rear-1; j++) {
            al->list[j] = al->list[j+1];
         }
         al->rear--;
         al->elements--;
         return element;
      }
   }
   return 0;
}

AL_TYPE rem_all_element_al(AL_TYPE element, struct array_list *al) {
   /* don't use this method if you know you are removing
    * more than two or three elements */
   uint64_t total = 0;
   for (uint64_t i = al->head; i < al->rear; i++) {
      if (AL_COMPARE(al->list[i],element)==0) {
         for (uint64_t j = i; j < al->rear-1; j++) {
            al->list[j] = al->list[j+1];
         }
         al->rear--;
         al->elements--;
         total++;
         i--;
      }
   }
   return total;
}

void fast_rem_all_element_al(AL_TYPE element, struct array_list *al) {
   struct array_list * new_al = malloc(sizeof(struct array_list));
   if (new_al == NULL) {
      fprintf(stderr,"ERROR: Unable to create new array_list in fast rem\n");
      perror(0);
      exit(1);
   }
   init_al(al->capacity,new_al);
   AL_TYPE curr;
   while (al->elements) {
      curr = rem_front_al(al);
      if (AL_COMPARE(curr,element)!=0) {
         add_al(curr,new_al);
      }
   }
   free(al->list);
   al->list = new_al->list;
   al->elements = new_al->elements;
   al->rear = new_al->rear;
   al->head = 0;
   free(new_al);
}

void replace_all_element_al(AL_TYPE old, AL_TYPE new, struct array_list *al) {
   for (uint64_t i = al->head; i < al->rear; i++) {
      if (AL_COMPARE(al->list[i],old)==0) al->list[i] = new;
   }
}

uint8_t find_al(AL_TYPE element, struct array_list *al) {
   for (uint64_t i = al->head; i < al->rear; i++) {
      if (AL_COMPARE(al->list[i],element)==0) {
         return 1;
      }
   }
   return 0;
}

uint64_t find_multiple_al(AL_TYPE * elements, uint8_t elements_len, struct array_list *al) {
   if (elements_len > 64) elements_len = 64;
   uint64_t ret = 0x0000000000000000;
   for (uint64_t i = al->head; i < al->rear; i++) {
      for (uint8_t j = 0; j < elements_len; j++) {
         if (AL_COMPARE(al->list[i],elements[j])==0) {
            /* mildly redundent but avoids (mostly) needless complication */
            ret |= (0x1<<j);
         }
      }
   }
   return ret;
}

void fast_rem_all_above_element_al(AL_TYPE cutoff, struct array_list *al) {
   struct array_list * new_al = malloc(sizeof(struct array_list));
   init_al(al->capacity,new_al);
   if (new_al == NULL) {
      fprintf(stderr,"ERROR: Unable to create new array_list in fast rem\n");
      perror(0);
      exit(1);
   }
   AL_TYPE curr;
   while (al->elements) {
      curr = rem_front_al(al);
      if (AL_COMPARE(curr,cutoff)<=0) {
         add_al(curr,new_al);
      }
   }
   free(al->list);
   al->list = new_al->list;
   al->elements = new_al->elements;
   al->rear = new_al->rear;
   al->head = 0;
   free(new_al);
}

void fast_rem_all_below_element_al(AL_TYPE cutoff, struct array_list *al) {
   struct array_list * new_al = malloc(sizeof(struct array_list));
   init_al(al->capacity,new_al);
   if (new_al == NULL) {
      fprintf(stderr,"ERROR: Unable to create new array_list in fast rem\n");
      perror(0);
      exit(1);
   }
   AL_TYPE curr;
   while (al->elements) {
      curr = rem_front_al(al);
      if (AL_COMPARE(curr,cutoff)>=0) {
         add_al(curr,new_al);
      }
   }
   free(al->list);
   al->list = new_al->list;
   al->elements = new_al->elements;
   al->rear = new_al->rear;
   al->head = 0;
   free(new_al);
}

void insertion_sort_al(uint64_t l, uint64_t h, AL_TYPE * A) {
   uint64_t bi;
   AL_TYPE bv,t;
   for (uint64_t i = l; i < h; i++) {
      bv = A[i]; bi = i;
      for (uint64_t j = i+1; j <= h; j++) {
         if (AL_COMPARE(A[j],bv)<0) {
            bv = A[j]; bi = j;
         }
      }
      t = A[i]; A[i] = A[bi]; A[bi] = t;
   }
}

void quicksort_al(uint64_t l, uint64_t h, AL_TYPE * A) {
   if (l >= h || l < 0) return;
   
   if (h-l < 16) {
      insertion_sort_al(l,h,A);
   }
   else {
      uint64_t pivot = partition_al(l,h,A);

      quicksort_al(l,pivot-1,A);
      quicksort_al(pivot+1,h,A);
   }
}


uint64_t partition_al(uint64_t l, uint64_t h, AL_TYPE * A) {
   AL_TYPE p = A[h],t;
   uint64_t i,j;
   i = l-1;
   for (j = l; j < h; j++) {
      if (AL_COMPARE(A[j],p)<=0) {
         i++;
         t = A[i]; A[i] = A[j]; A[j] = t;
      }
   }
   i++;
   t = A[i]; A[i] = A[h]; A[h] = t;
   return i;
}

void sort_al(struct array_list * al) {
   quicksort_al(al->head,al->rear-1,al->list);
   printf("\n");
}

