
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void f(uint32_t *x) {
   uint32_t C  = 0x2aaaaaaa;
   uint64_t c[4];
   c[0] = 0x025f1cdb;
   c[1] = 0x04be39b6; 
   c[2] = 0x12f8e6d8;
   c[3] = 0x2f8e6d81;

   for (int i = 0; i < 4; i++) {
      x[i] = ((uint64_t)(c[i] * (uint64_t)x[i])) % 0xffffffff;
   }
   for (int i = 0; i < 4; i++) {
      x[i] = x[(i+3)%4] ^ x[i] ^ x[(i+1)%4];
   }
   if (x[0] & 1) x[0] ^= C;
   if (x[3] & 1) x[3] ^= C;
}

void fi(uint32_t *x) {
   uint32_t C  = 0x2aaaaaaa;
   uint64_t c[4];
   c[0] = 0x0dad4694;
   c[1] = 0x06d6a34a;
   c[2] = 0x81b5a8d2;
   c[3] = 0x281b5a8d;

   if (x[0] & 1) x[0] ^= C;
   if (x[3] & 1) x[3] ^= C;
   for (int i = 3; i >= 0; i--) {
      x[i] = x[(i+3)%4] ^ x[i] ^ x[(i+1)%4];
   }
   for (int i = 0; i < 4; i++) {
      x[i] = ((uint64_t)(c[i] * (uint64_t)x[i])) % 0xffffffff;
   }
}

void encrypt_MMB(uint32_t *x, uint32_t *k) {
   uint8_t p[6] = { 0, 1, 2, 0, 1, 2};
   for (int i = 0; i < 6; i++) {
      for (int j = 0; j < 4; j++) {
         x[j] ^= k[(j+p[i])%4];
      }
      f(x);
   }
}

void decrypt_MMB(uint32_t *x, uint32_t *k) {
   uint8_t p[6] = { 2, 1, 0, 2, 1, 0};
   for (int i = 0; i < 6; i++) {
      fi(x);
      for (int j = 0; j < 4; j++) {
         x[j] ^= k[(j+p[i])%4];
      }
   }
}

int main(void) {

   srand(clock());

   uint32_t num[4] = { rand(), rand(), rand(), rand()}; 
   uint32_t key[4] = { 1, 2, 3, 4};

   printf("before:  ");
   for (int i = 3; i >= 0; i--) {
      printf("%08x",num[i]);
   }
   printf("\n");

   encrypt_MMB(num,key);

   printf("encrypt: ");
   for (int i = 3; i >= 0; i--) {
      printf("%08x",num[i]);
   }
   printf("\n");

   decrypt_MMB(num,key);

   printf("decrypt: ");
   for (int i = 3; i >= 0; i--) {
      printf("%08x",num[i]);
   }
   printf("\n");

   return 0;
}
