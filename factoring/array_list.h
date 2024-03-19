
#ifndef ARRAY_LIST
#include <stdint.h>

#define ARRAY_LIST
#ifndef AL_TYPE
#define AL_TYPE uint64_t
#endif
#ifndef AL_COMPARE
#define AL_COMPARE(x,y) ((int64_t)(x)-(int64_t)(y))
#endif

struct array_list { uint64_t capacity; uint64_t elements; uint64_t rear; uint64_t head; AL_TYPE * list; };

#define foreach_al(i,al) for(uint64_t j_l=al->head,i=al->list[j_l];j_l<al->rear;j_l++,i=al->list[j_l])

void init_al(uint64_t, struct array_list *);

void free_al(struct array_list *);

void expand_al(struct array_list *);

void unshift_al(struct array_list *);

void maintenance_al(struct array_list *);

void add_al(AL_TYPE, struct array_list *);

void add_front_al(AL_TYPE, struct array_list *);

uint8_t find_al(AL_TYPE, struct array_list *);

uint64_t find_multiple_al(AL_TYPE *, uint8_t, struct array_list *);

AL_TYPE rem_front_al(struct array_list *);

AL_TYPE rem_back_al(struct array_list *);

AL_TYPE rem_element_al(AL_TYPE, struct array_list *);

AL_TYPE rem_all_element_al(AL_TYPE, struct array_list *);

void fast_rem_all_element_al(AL_TYPE, struct array_list *);

void fast_rem_all_above_element_al(AL_TYPE, struct array_list *);

void fast_rem_all_below_element_al(AL_TYPE, struct array_list *);

void replace_all_element_al(AL_TYPE, AL_TYPE, struct array_list *);

void insertion_sort_al(uint64_t, uint64_t, AL_TYPE *);

uint64_t partition_al(uint64_t, uint64_t, AL_TYPE *);

void quicksort_al(uint64_t, uint64_t, AL_TYPE *);

void sort_al(struct array_list *);

#endif
