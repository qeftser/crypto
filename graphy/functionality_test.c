
#include "cryptography.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define ROL1(a) a = (a << 1) | (a >> 63)

const unsigned long int cycles = 1;

int main(void) {


   uint64_t res0, num0 = 0x0123456789abcdef;
   uint32_t res1[4], num1[4] = { 0x0123, 0x4567, 0x89ab, 0xcdef };
   uint32_t resA, resB, numA, numB;

   uint32_t gen[4];

   uint64_t key0 = 0x123456789abcdef;
   uint32_t key1[8] = { 0x1234, 0x5678, 0x9abc, 0xdef0, 0x0123, 0x4567, 0x89ab, 0xcdef };
   uint32_t key2[4] = { 0x1234, 0x5678, 0x9abc, 0xdef0 };
   uint64_t key3[2] = { 0x0123456789abcdef, 0x012345689abcdef };
   uint32_t key4[60];
   uint64_t key5[3];
   uint32_t key6[60];
   uint32_t key7[60];

   srand(clock());


   /* testing DES */
   for (unsigned long int i = 0; i < cycles; i++) {
      gen[0] = rand(); gen[1] = rand();
      memcpy(&key0,gen,8);
      gen[0] = rand(); gen[1] = rand();
      memcpy(&num0,gen,8);
      res0 = encrypt_DES(num0,key0);
      res0 = decrypt_DES(res0,key0);
      
      //printf("num: %lu <> res: %lu\n",num0,res0);
      printf("num = res? %d\n",num0 == res0);

   }

   /* testing FEAL */
   for (unsigned long int i = 0; i < cycles; i++) {
      gen[0] = rand(); gen[1] = rand();
      memcpy(&key0,gen,8);
      gen[0] = rand(); gen[1] = rand();
      memcpy(&num0,gen,8);
      res0 = encrypt_FEAL(num0,key0);
      res0 = decrypt_FEAL(res0,key0);

      //printf("num: %lu <> res: %lu\n",num0,res0);
      printf("num = res? %d\n",num0 == res0);
   }

   /* testing GOST */
   for (unsigned long int i = 0; i < cycles; i++) {
      gen[0] = rand(); gen[1] = rand();
      memcpy(&num0,gen,8);
      for (int i = 0; i < 8; i++) {
         key1[i] = rand();
      }
      res0 = num0;
      encrypt_GOST(&res0,key1);
      decrypt_GOST(&res0,key1);

      //printf("num: %lu <> res: %lu\n",num0,res0);
      printf("num = res? %d\n",num0 == res0);
   }

   /* testing LOKI */
   for (unsigned long int i = 0; i < cycles; i++) {
      gen[0] = rand(); gen[1] = rand();
      memcpy(&key0,gen,8);
      gen[0] = rand(); gen[1] = rand();
      memcpy(&num0,gen,8);
      res0 = encrypt_LOKI(num0,key0);
      res0 = decrypt_LOKI(res0,key0);

      //printf("num: %lu <> res: %lu\n",num0,res0);
      printf("num = res? %d\n",num0 == res0);
   }
   
   /* testing MMB */
   for (unsigned long int i = 0; i < cycles; i++) {
      for (int i = 0; i < 4; i++) res1[i] = num1[i] = rand();
      for (int i = 0; i < 4; i++) key2[i] = rand();
      encrypt_MMB(res1,key2);
      decrypt_MMB(res1,key2);

      /*
      printf("num: ");
      for (int i = 0; i < 4; i++) {
         printf("%d ",num1[i]);
      }
      printf("<> res: ");
      for (int i = 0; i < 4; i++) {
         printf("%d ",res1[i]);
      }
      printf("\n");
      */
      printf("num = res? %d\n",num1[0] == res1[0] && num1[1] == res1[1] &&
                               num1[2] == res1[2] && num1[3] == res1[3]);
   }

   /* testing IDEA */
   for (unsigned long int i = 0; i < cycles; i++) {
      gen[0] = rand(); gen[1] = rand();
      gen[2] = rand(); gen[3] = rand();
      memcpy(key3,gen,16);
      gen[0] = rand(); gen[1] = rand();
      memcpy(&num0,gen,8);
      res0 = num0;
      encrypt_IDEA(&res0,key3);
      decrypt_IDEA(&res0,key3);
      //printf("num: %lu <> res: %lu\n",num0,res0);
      printf("num = res? %d\n",num0 == res0);
   }

   struct Geffe geffe;

   /* testing Geffe */
   for (unsigned long int i = 0; i < cycles; i++) {
      for (int i = 0; i < 3; i++) key5[i] = rand()*rand();
      gen[0] = rand(); gen[1] = rand();
      memcpy(&num0,gen,8);
      res0 = num0;
      init_Geffe(key5[0],key5[1],key5[2],0,&geffe);
      for (int i = 0; i < 64; i++) {
         res0 ^= shift_Geffe(&geffe);
         ROL1(res0);
      }
      init_Geffe(key5[0],key5[1],key5[2],0,&geffe);
      for (int i = 0; i < 64; i++) {
         res0 ^= shift_Geffe(&geffe);
         ROL1(res0);
      }

      //printf("num: %lu <> res: %lu\n",num0,res0);
      printf("num = res? %d\n",num0 == res0);
   }

   struct BPSaG bpsag;

   /* testing BPSaG */
   /* BROKEN */
   for (unsigned long int i = 0; i < cycles; i++) {
      for (int i = 0; i < 3; i++) key5[i] = rand()*rand();
      gen[0] = rand(); gen[1] = rand();
      memcpy(&num0,gen,8);
      res0 = num0;
      init_BPSaG(key5[0],key5[1],key5[2],0,&bpsag);
      for (int i = 0; i < 64; i++) {
         res0 ^= shift_BPSaG(&bpsag);
         ROL1(res0);
      }
      init_BPSaG(key5[0],key5[1],key5[2],0,&bpsag);
      for (int i = 0; i < 64; i++) {
         res0 ^= shift_BPSaG(&bpsag);
         ROL1(res0);
      }

      //printf("num: %lu <> res: %lu\n",num0,res0);
      printf("num = res? %d\n",num0 == res0);
   }

   struct BSaG bsag;
   init_BSaG(key0,key0,0,&bsag);

   /* testing BSaG */
   /* BROKEN */
   for (unsigned long int i = 0; i < cycles; i++) {
      for (int i = 0; i < 2; i++) key5[i] = rand()*rand();
      gen[0] = rand(); gen[1] = rand();
      memcpy(&num0,gen,8);
      res0 = num0;
      init_BSaG(key5[0],key5[1],0,&bsag);
      for (int i = 0; i < 64; i++) {
         res0 ^= shift_BSaG(&bsag);
         ROL1(res0);
      }
      init_BSaG(key5[0],key5[1],0,&bsag);
      for (int i = 0; i < 64; i++) {
         res0 ^= shift_BSaG(&bsag);
         ROL1(res0);
      }

      //printf("num: %lu <> res: %lu\n",num0,res0);
      printf("num = res? %d\n",num0 == res0);
   }

   struct ASaG asag;

   /* testing ASaG */
   /* BROKEN */
   for (unsigned long int i = 0; i < cycles; i++) {
      for (int i = 0; i < 3; i++) key5[i] = rand()*rand();
      gen[0] = rand(); gen[1] = rand();
      memcpy(&num0,gen,8);
      res0 = num0;
      init_ASaG(key5[0],key5[1],key5[2],0,&asag);
      for (int i = 0; i < 64; i++) {
         res0 ^= shift_ASaG(&asag);
         ROL1(res0);
      }
      init_ASaG(key5[0],key5[1],key5[2],0,&asag);
      for (int i = 0; i < 64; i++) {
         res0 ^= shift_ASaG(&asag);
         ROL1(res0);
      }

      //printf("num: %lu <> res: %lu\n",num0,res0);
      printf("num = res? %d\n",num0 == res0);
   }

   struct Fish fish;

   /* testing Fish */
   /* BROKEN */
   for (unsigned long int i = 0; i < cycles; i++) {
      for (int i = 0; i < 60; i++) { key4[i] = rand(); key6[i] = rand(); }
      resA = numA = rand(); resB = numB = rand();
      init_Fish(key4,key6,&fish);
      shift_Fish(&resA,&resB,&fish);
      init_Fish(key4,key6,&fish);
      shift_Fish(&resA,&resB,&fish);

      //printf("num: %u:%u <> res: %u:%u\n",numA,numB,resA,resB);
      printf("num = res? %d\n",numA == resA && numB == resB);
   }

   struct Pike pike;
   init_Pike(key4,key4,key4,&pike);

   /* testing Pike */
   for (unsigned long int i = 0; i < cycles; i++) {
      for (int i = 0; i < 60; i++) { key4[i] = rand(); key6[i] = rand(); key7[i] = rand(); }
      resA = numA = rand();
      init_Pike(key4,key6,key7,&pike);
      resA ^= shift_Pike(&pike);
      init_Pike(key4,key6,key7,&pike);
      resA ^= shift_Pike(&pike);

      //printf("num: %u <> res: %u\n",numA,resA);
      printf("num = res? %d\n",numA == resA);
   }

   struct Mush mush;
   init_Mush(key4,key4,&mush);

   /* testing Mush */
   for (unsigned long int i = 0; i < cycles; i++) {
      for (int i = 0; i < 60; i++) { key4[i] = rand(); key6[i] = rand(); }
      resA = numA = rand();
      init_Mush(key4,key6,&mush);
      resA ^= shift_Mush(&mush);
      init_Mush(key4,key6,&mush);
      resA ^= shift_Mush(&mush);

      //printf("num: %u <> res: %u\n",numA,resA);
      printf("num = res? %d\n",numA == resA);
   }

   return 0;
}
