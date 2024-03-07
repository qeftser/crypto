
#include "cryptography.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define ROL1(a) a = (a << 1) | (a >> 63)
#define ROL8(a) a = (a << 8) | (a >> 56)

const unsigned long int cycles = 1000000;

void wrapper(uint64_t  * B, uint32_t * K) {
   encrypt_GOST(B,K);
}

int main(void) {


   uint64_t res0, num0 = 0x0123456789abcdef;
   uint32_t res1[4], num1[4] = { 0x0123, 0x4567, 0x89ab, 0xcdef };
   uint32_t resA, resB, numA, numB, intrmA, intrmB;

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
      
      if (num0 != res0) {
         printf("DES FAILED\n");
         printf("On key: %016lx\n",key0);
         printf("On val: %016lx\n",num0);
         break;
      }
   }
   printf("%20s COMPLETE\n","DES");

   /* testing FEAL */
   for (unsigned long int i = 0; i < cycles; i++) {
      gen[0] = rand(); gen[1] = rand();
      memcpy(&key0,gen,8);
      gen[0] = rand(); gen[1] = rand();
      memcpy(&num0,gen,8);
      res0 = encrypt_FEAL(num0,key0);
      res0 = decrypt_FEAL(res0,key0);

      if (num0 != res0) {
         printf("FEAL FAILED\n");
         printf("On key: %016lx\n",key0);
         printf("On val: %016lx\n",num0);
         break;
      }
   }
   printf("%20s COMPLETE\n","FEAL");


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

      if (num0 != res0) {
         printf("GOST FAILED\n");
         printf("On key: %016lx\n",key0);
         printf("On val: %016lx\n",num0);
         break;
      }
   }
   printf("%20s COMPLETE\n","GOST");

   /* testing LOKI */
   for (unsigned long int i = 0; i < cycles; i++) {
      gen[0] = rand(); gen[1] = rand();
      memcpy(&key0,gen,8);
      gen[0] = rand(); gen[1] = rand();
      memcpy(&num0,gen,8);
      res0 = encrypt_LOKI(num0,key0);
      res0 = decrypt_LOKI(res0,key0);

      if (num0 != res0) {
         printf("LOKI FAILED\n");
         printf("On key: %016lx\n",key0);
         printf("On val: %016lx\n",num0);
         break;
      }
   }
   printf("%20s COMPLETE\n","LOKI");
   
   /* testing MMB */
   for (unsigned long int i = 0; i < cycles; i++) {
      for (int i = 0; i < 4; i++) res1[i] = num1[i] = rand();
      for (int i = 0; i < 4; i++) key2[i] = rand();
      encrypt_MMB(res1,key2);
      decrypt_MMB(res1,key2);

      if (num1[0] != res1[0] || num1[1] != res1[1] || num1[2] != res1[2] || num1[3] != res1[3]) {
         printf("MMB FAILED\n");
         printf("On key: %08x %08x %08x %08x\n",key1[0],key1[1],key1[2],key1[2]);
         printf("On val: %08x %08x %08x %08x\n",num1[0],num1[1],num1[2],num1[3]);
         break;
      }
   }
   printf("%20s COMPLETE\n","MMB");

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

      if (num0 != res0) {
         printf("IDEA FAILED\n");
         printf("On key: %016lx\n",key0);
         printf("On val: %016lx\n",num0);
         break;
      }
   }
   printf("%20s COMPLETE\n","IDEA");

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

      if (num0 != res0) {
         printf("Geffe FAILED\n");
         printf("On key: %016lx\n",key0);
         printf("On val: %016lx\n",num0);
         break;
      }
   }
   printf("%20s COMPLETE\n","Geffe");

   struct BPSaG bpsag;

   /* testing BPSaG */
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

      if (num0 != res0) {
         printf("BPSaG FAILED\n");
         printf("On key: %016lx\n",key0);
         printf("On val: %016lx\n",num0);
         break;
      }
   }
   printf("%20s COMPLETE\n","BPSaG");

   struct BSaG bsag;
   init_BSaG(key0,key0,0,&bsag);

   /* testing BSaG */
   /* 
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

      if (num0 != res0) {
         printf("BSaG FAILED\n");
         printf("On key: %016lx\n",key0);
         printf("On val: %016lx\n",num0);
         break;
      }
   }
   printf("%20s COMPLETE","BSaG");
   */

   struct ASaG asag;

   /* testing ASaG */
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

      if (num0 != res0) {
         printf("ASaG FAILED\n");
         printf("On key: %016lx\n",key0);
         printf("On val: %016lx\n",num0);
         break;
      }
   }
   printf("%20s COMPLETE\n","ASaG");

   struct Fish fish;

   /* testing Fish */
   for (unsigned long int i = 0; i < cycles; i++) {
      for (int i = 0; i < 60; i++) { key4[i] = rand(); key6[i] = rand(); }
      resA = numA = rand(); resB = numB = rand();
      init_Fish(key4,key6,&fish);
      shift_Fish(&intrmA,&intrmB,&fish);
      resA ^= intrmA; resB ^= intrmB;
      init_Fish(key4,key6,&fish);
      shift_Fish(&intrmA,&intrmB,&fish);
      resA ^= intrmA; resB ^= intrmB;

      if (numA != resA || numB != resB) {
         printf("Fish FAILED\n");
         printf("On val: %08x %08x\n",numA,numB);
         break;
      }
   }
   printf("%20s COMPLETE\n","Fish");

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

      if (numA != resA) {
         printf("Pike FAILED\n");
         printf("On val: %08x\n",numA);
         break;
      }
   }
   printf("%20s COMPLETE\n","Pike");

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

      if (numA != resA) {
         printf("Mush FAILED\n");
         printf("On val: %08x\n",numA);
         break;
      }
   }
   printf("%20s COMPLETE\n","Mush");

   struct ORYX oryx;

   /* testing ORYX */
   for (unsigned long int i = 0; i < cycles; i++) {
      for (int i = 0; i < 3; i++) { key2[i] = rand(); };
      res0 = num0 = rand();
      init_ORYX(key2[0],key2[1],key2[2],0,&oryx);
      for (int i = 0; i < 8; i++) {
         res0 ^= shift_ORYX(&oryx);
         ROL8(res0);
      }
      init_ORYX(key2[0],key2[1],key2[2],0,&oryx);
      for (int i = 0; i < 8; i++) {
         res0 ^= shift_ORYX(&oryx);
         ROL8(res0);
      }
      if (res0 != num0) {
         printf("ORYX FAILED\n");
         printf("On val: %016lx\n",num0);
         break;
      }
   }
   printf("%20s COMPLETE\n","ORYX");

   return 0;
}
