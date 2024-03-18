
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "array_list.h"

void init_al(uint64_t init_capacity, struct array_list *al) {
   al->list = malloc(sizeof(uint64_t)*init_capacity);
   if (al->list == NULL) {
      fprintf(stderr,"ERROR: Unable to initialize array list\n");
      exit(1);
   }
   al->capacity = init_capacity;
   al->rear = 0;
   al->head = 0;
   al->capacity = init_capacity;
}

void free_al(struct array_list *al) {
   free(al->list);
   free(al);
}

void expand_al(struct array_list *al) {
   al->list = reallocarray(al->list,sizeof(uint64_t)*al->capacity*2,sizeof(uint64_t));
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

void add_al(uint64_t element, struct array_list *al) {
   maintenance_al(al);
   al->list[al->rear++] = element;
   al->elements++;
}

void add_front_al(uint64_t element, struct array_list *al) {
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

uint64_t rem_front_al(struct array_list *al) {
   al->head++;
   al->elements--;
   return al->list[al->head-1];
}

uint64_t rem_back_al(struct array_list *al) {
   al->rear--;
   al->elements--;
   return al->list[al->rear];
}

uint64_t rem_element_al(uint64_t element, struct array_list *al) {
   /* this is a slow one as well */
   for (uint64_t i = al->head; i < al->rear; i++) {
      if (al->list[i] == element) {
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

uint64_t rem_all_element_al(uint64_t element, struct array_list *al) {
   /* don't use this method if you know you are removing
    * more than two or three elements */
   uint64_t total = 0;
   for (uint64_t i = al->head; i < al->rear; i++) {
      if (al->list[i] == element) {
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

void fast_rem_all_element_al(uint64_t element, struct array_list *al) {
   struct array_list * new_al = malloc(sizeof(struct array_list));
   if (new_al == NULL) {
      fprintf(stderr,"ERROR: Unable to create new array_list in fast rem\n");
      perror(0);
      exit(1);
   }
   init_al(al->capacity,new_al);
   uint64_t curr;
   while (al->elements) {
      curr = rem_front_al(al);
      if (curr != element) {
         add_al(curr,new_al);
      }
   }
   free(al->list);
   al->list = new_al->list;
   free(new_al);
}

void replace_all_element_al(uint64_t old, uint64_t new, struct array_list *al) {
   for (uint64_t i = al->head; i < al->rear; i++) {
      if (al->list[i] == old) al->list[i] = new;
   }
}

uint8_t find_al(uint64_t element, struct array_list *al) {
   for (uint64_t i = al->head; i < al->rear; i++) {
      if (al->list[i] == element) {
         return 1;
      }
   }
   return 0;
}

uint64_t find_multiple_al(uint64_t * elements, uint8_t elements_len, struct array_list *al) {
   if (elements_len > 64) elements_len = 64;
   uint64_t ret = 0x0000000000000000;
   for (uint64_t i = al->head; i < al->rear; i++) {
      for (uint8_t j = 0; j < elements_len; j++) {
         if (al->list[i] == elements[j]) {
            /* mildly redundent but avoids (mostly) needless complication */
            ret |= (0x1<<j);
         }
      }
   }
   return ret;
}

void fast_rem_all_above_element_al(uint64_t cutoff, struct array_list *al) {
   struct array_list * new_al = malloc(sizeof(struct array_list));
   init_al(al->capacity,new_al);
   if (new_al == NULL) {
      fprintf(stderr,"ERROR: Unable to create new array_list in fast rem\n");
      perror(0);
      exit(1);
   }
   uint64_t curr;
   while (al->elements) {
      curr = rem_front_al(al);
      if (curr <= cutoff) {
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

void fast_rem_all_below_element_al(uint64_t cutoff, struct array_list *al) {
   struct array_list * new_al = malloc(sizeof(struct array_list));
   init_al(al->capacity,new_al);
   if (new_al == NULL) {
      fprintf(stderr,"ERROR: Unable to create new array_list in fast rem\n");
      perror(0);
      exit(1);
   }
   uint64_t curr;
   while (al->elements) {
      curr = rem_front_al(al);
      if (curr >= cutoff) {
         add_al(curr,new_al);
      }
   }
   free(al->list);
   al->list = new_al->list;
   free(new_al);
}

