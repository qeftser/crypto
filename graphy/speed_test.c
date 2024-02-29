
#include "cryptography.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MEGABYTE 1024 * 1024 * 8
#define ENCRYPTION_SIZE 1024
#define TOTAL_CIPHERS 13

#define PRINT_RES(a) printf("%-10s|%11ld|   % 14.6f| % 18.6f|\n", \
                     names[a],cycles/encryption_size[a],times[a],((double)cycles/(8*1024))/times[a])

const unsigned long int cycles = (long)MEGABYTE * ENCRYPTION_SIZE;

int main(void) {

   double times[TOTAL_CIPHERS];
   char *names[TOTAL_CIPHERS] = { "DES", "FEAL", "GOST", "LOKI", "MMB", "IDEA", 
                                  "Geffe", "BPSaG", "BSaG", "ASaG", "Fish", "Pike", "Mush" };
   uint16_t encryption_size[TOTAL_CIPHERS] = { 64, 64, 64, 64, 128, 64, 1, 1, 1, 1, 64, 32, 32 };

   uint64_t num = 0x0123456789abcdef;
   uint32_t num1[4] = { 0x0123, 0x4567, 0x89ab, 0xcdef };
   uint32_t numA, numB;

   uint64_t key0 = 0x123456789abcdef;
   uint32_t key1[8] = { 0x1234, 0x5678, 0x9abc, 0xdef0, 0x0123, 0x4567, 0x89ab, 0xcdef };
   uint32_t key2[4] = { 0x1234, 0x5678, 0x9abc, 0xdef0 };
   uint64_t key3[2] = { 0x0123456789abcdef, 0x012345689abcdef };
   uint32_t key4[60] = { 1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7,8,9,1,2,3,
                         4,5,6,7,8,9,1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7,8,9,1,2,3,4,5,6};


   clock_t sClock;

   int res;

   res = system("date");
   if (res == -1) printf("error getting date\n");
   printf("BENCHMARKING ALL CIPHERS OVER %d MEGABYTES OF DATA (%0.2f GB)\n",
          ENCRYPTION_SIZE,(double)ENCRYPTION_SIZE/1024);

   printf("Cipher    |    Cycles |            Time |            KB/Sec |\n");
   printf("-------------------------------------------------------------\n");

   /* testing DES */
   sClock = clock();
   for (unsigned long int i = 0; i < cycles / encryption_size[0]; i++) {
      num = encrypt_DES(num,key0);
   }
   times[0] = (double)(clock() - sClock) / CLOCKS_PER_SEC;
   PRINT_RES(0);

   /* testing FEAL */
   sClock = clock();
   for (unsigned long int i = 0; i < cycles / encryption_size[1]; i++) {
      num = encrypt_FEAL(num,key0);
   }
   times[1] = (double)(clock() - sClock) / CLOCKS_PER_SEC;
   PRINT_RES(1);

   /* testing GOST */
   sClock = clock();
   for (unsigned long int i = 0; i < cycles / encryption_size[2]; i++) {
      encrypt_GOST(&num,key1);
   }
   times[2] = (double)(clock() - sClock) / CLOCKS_PER_SEC;
   PRINT_RES(2);

   /* testing LOKI */
   sClock = clock();
   for (unsigned long int i = 0; i < cycles / encryption_size[3]; i++) {
      num = encrypt_LOKI(num,key0);
   }
   times[3] = (double)(clock() - sClock) / CLOCKS_PER_SEC;
   PRINT_RES(3);
   
   /* testing MMB */
   sClock = clock();
   for (unsigned long int i = 0; i < cycles / encryption_size[4]; i++) {
      encrypt_MMB(num1,key2);
   }
   times[4] = (double)(clock() - sClock) / CLOCKS_PER_SEC;
   PRINT_RES(4);

   /* testing IDEA */
   sClock = clock();
   for (unsigned long int i = 0; i < cycles / encryption_size[5]; i++) {
      encrypt_IDEA(&num,key3);
   }
   times[5] = (double)(clock() - sClock) / CLOCKS_PER_SEC;
   PRINT_RES(5);

   struct Geffe geffe;
   init_Geffe(key0,key0,key0,0,&geffe);

   /* testing Geffe */
   sClock = clock();
   for (unsigned long int i = 0; i < cycles / encryption_size[6]; i++) {
      num ^= shift_Geffe(&geffe);
   }
   times[6] = (double)(clock() - sClock) / CLOCKS_PER_SEC;
   PRINT_RES(6);

   struct BPSaG bpsag;
   init_BPSaG(key0,key0,key0,0,&bpsag);

   /* testing BPSaG */
   sClock = clock();
   for (unsigned long int i = 0; i < cycles / encryption_size[7]; i++) {
      num ^= shift_BPSaG(&bpsag);
   }
   times[7] = (double)(clock() - sClock) / CLOCKS_PER_SEC;
   PRINT_RES(7);

   struct BSaG bsag;
   init_BSaG(key0,key0,0,&bsag);

   /* testing BSaG */
   sClock = clock();
   for (unsigned long int i = 0; i < cycles / encryption_size[8]; i++) {
      num ^= shift_BSaG(&bsag);
   }
   times[8] = (double)(clock() - sClock) / CLOCKS_PER_SEC;
   PRINT_RES(8);

   struct ASaG asag;
   init_ASaG(key0,key0,key0,0,&asag);

   /* testing ASaG */
   sClock = clock();
   for (unsigned long int i = 0; i < cycles / encryption_size[9]; i++) {
      num ^= shift_ASaG(&asag);
   }
   times[9] = (double)(clock() - sClock) / CLOCKS_PER_SEC;
   PRINT_RES(9);

   struct Fish fish;
   init_Fish(key4,key4,&fish);

   /* testing Fish */
   sClock = clock();
   for (unsigned long int i = 0; i < cycles / encryption_size[10]; i++) {
      shift_Fish(&numA,&numB,&fish);
      num ^= numA ^ numB;
   }
   times[10] = (double)(clock() - sClock) / CLOCKS_PER_SEC;
   PRINT_RES(10);

   struct Pike pike;
   init_Pike(key4,key4,key4,&pike);

   /* testing Pike */
   sClock = clock();
   for (unsigned long int i = 0; i < cycles / encryption_size[11]; i++) {
      num ^= shift_Pike(&pike);
   }
   times[11] = (double)(clock() - sClock) / CLOCKS_PER_SEC;
   PRINT_RES(11);

   struct Mush mush;
   init_Mush(key4,key4,&mush);

   /* testing Mush */
   sClock = clock();
   for (unsigned long int i = 0; i < cycles / encryption_size[12]; i++) {
      num ^= shift_Mush(&mush);
   }
   times[12] = (double)(clock() - sClock) / CLOCKS_PER_SEC;
   PRINT_RES(12);

   num ^= num1[0] ^ num1[1] ^ num1[2] ^ num1[3];
   printf("\n\nGARBAGE: %lu\n",num);

   return 0;
}
