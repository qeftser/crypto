
#define _GNU_SOURCE
#include <stdint.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sched.h>
#include "../../graphy/cryptography.h"


struct GOST_block { uint32_t K[8]; uint64_t plain_text; uint64_t cipher_text; };
static void *GOST_thread(void *arg) {
   struct GOST_block *block = arg;
   uint64_t plain_text_copy;
   for (uint32_t i = 0; i < UINT_MAX; i++) {
      if (i % 0x00ffffff == 0) {
         printf("Key: %08x %08x %08x %08x %08x %08x %08x %08x\n",
                block->K[0],block->K[1],block->K[2],block->K[3],block->K[4],block->K[5],block->K[6],block->K[7]);
      }
      block->K[7] = i;
      plain_text_copy = block->plain_text;
      encrypt_GOST(&plain_text_copy,block->K);
      if (plain_text_copy == block->cipher_text) {
         printf("Found Key: %08x %08x %08x %08x %08x %08x %08x %08x\n",
                block->K[0],block->K[1],block->K[2],block->K[3],block->K[4],block->K[5],block->K[6],block->K[7]);
         exit(0);
      }
   }
   return (void *)-1;
}

struct DES_block { uint64_t Ks; uint64_t Ke; uint64_t P; uint64_t C; };
static void *DES_thread(void *arg) {
   struct DES_block *block = arg;
   uint64_t cc;
   for (uint64_t i = block->Ks; i < block->Ke; i++) {
      cc = encrypt_DES(block->P,i);
      if (cc == block->C) {
         printf("Found Key: %016lx\n",i);
         exit(0);
      }
   }
   return (void *)-1;
}

/* GOST */
/* find GOST key with known plaintext */
void crack_GOST(uint64_t plain_text, uint64_t cipher_text) {

   const int threads = 8;
   const int cpu_count = 8;

   struct GOST_block blocks[threads];
   pthread_t GOST_threads[threads];
   pthread_attr_t attr;
   cpu_set_t cpu_set;
   void * res = 0;

   /* thread setup */
   pthread_attr_init(&attr);
   pthread_attr_setstacksize(&attr,128);
   pthread_attr_setguardsize(&attr,0);

   for (int i = 0; i < threads; i++) {
      blocks[i].plain_text = plain_text;
      blocks[i].cipher_text = cipher_text;
   }

   for (uint32_t i0 = 0; i0 < UINT_MAX; i0++) { printf("I0: %u\n",i0);
   for (int j = 0; j < threads; j++) { blocks[j].K[0] = i0; }
   for (uint32_t i1 = 0; i1 < UINT_MAX; i1++) { printf("I1: %u\n",i1);
   for (int j = 0; j < threads; j++) { blocks[j].K[1] = i1; }
   for (uint32_t i2 = 0; i2 < UINT_MAX; i2++) { printf("I2: %u\n",i2);
   for (int j = 0; j < threads; j++) { blocks[j].K[2] = i2; }
   for (uint32_t i3 = 0; i3 < UINT_MAX; i3++) { printf("I3: %u\n",i3);
   for (int j = 0; j < threads; j++) { blocks[j].K[3] = i3; }
   for (uint32_t i4 = 0; i4 < UINT_MAX; i4++) { printf("I4: %u\n",i4);
   for (int j = 0; j < threads; j++) { blocks[j].K[4] = i4; }
   for (uint32_t i5 = 0; i5 < UINT_MAX; i5++) { printf("I5: %u\n",i5);
   for (int j = 0; j < threads; j++) { blocks[j].K[5] = i5; }
   for (uint32_t i6 = 0; i6+(threads-1) < UINT_MAX; i6+=threads) { printf("I6: %u\n",i6);

      for (int i7 = 0; i7 < threads; i7++) {
         blocks[i7].K[6] = i6+i7;
         CPU_ZERO(&cpu_set);
         CPU_SET(i7%cpu_count,&cpu_set);
         pthread_attr_setaffinity_np(&attr,sizeof(cpu_set_t),&cpu_set);
         pthread_create(&GOST_threads[i7],&attr,GOST_thread,&blocks[i7]);
      }

      for (int i7 = 0; i7 < threads; i7++) {
         pthread_join(GOST_threads[i7],res);
         free(res);
      }

   }
   }
   }
   }
   }
   }
   }
}

/* DES */
/* find DES key with single known plaintext */
void crack_DES(uint64_t plain_text, uint64_t cipher_text) {

   const int threads = 8;
   const int cpu_count = 8;
   const uint64_t step = 2097152;

   struct DES_block blocks[threads];
   pthread_t DES_threads[threads];
   pthread_attr_t attr;
   cpu_set_t cpu_set;
   void * res = 0;

   /* thread setup */
   pthread_attr_init(&attr);
   pthread_attr_setstacksize(&attr,256);
   pthread_attr_setguardsize(&attr,0);

   for (int i = 0; i < threads; i++) {
      blocks[i].P = plain_text;
      blocks[i].C = cipher_text;
   }

   for (uint64_t D = 0; D+step-1 < 36028797018963968; D+=step) {
      for (int T = 0; T < threads; T++) {
         printf("[ %016lx - %016lx ]\n",D+(step*T),D+(step*(T+1)));
         blocks[T].Ks = D+(step*T);
         blocks[T].Ke = D+(step*(T+1));
         CPU_ZERO(&cpu_set);
         CPU_SET(T%cpu_count,&cpu_set);
         pthread_attr_setaffinity_np(&attr,sizeof(cpu_set_t),&cpu_set);
         pthread_create(&DES_threads[T],&attr,DES_thread,&blocks[T]);
      }

      for (int i = 0; i < threads; i++) {
         pthread_join(DES_threads[i],res);
         free(res);
      }
   }
}
