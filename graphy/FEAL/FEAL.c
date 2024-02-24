
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define swap(a,b) temp = b; b = a; a = temp

uint8_t s0(uint8_t x1, uint8_t x2) {
   uint16_t cx1 = (uint16_t) x1;
   uint8_t Y = (uint8_t) ((cx1+x2) % 256);
   Y = (Y << 2) | (Y >> 6);
   return Y;
}

uint8_t s1(uint8_t x1, uint8_t x2) {
   uint16_t cx1 = (uint16_t) x1;
   uint8_t Y = (uint8_t) ((cx1+x2+1) % 256);
   Y = (Y << 2) | (Y >> 6);
   return Y;
}

void fk(uint32_t *A, uint32_t B, uint16_t *K1, uint16_t *K2) {
   uint8_t a[4],b[4],Y[4];
   memcpy(a,A,4);
   memcpy(b,&B,4);

   uint8_t a0xa1 = a[0] ^ a[1];
   uint8_t a2xa3 = a[2] ^ a[3];
   uint8_t b0xa2xa3 = b[0] ^ a2xa3;
   Y[1] = s1(a0xa1,b0xa2xa3);
   uint8_t x2 = b[2] ^ Y[1];
   Y[0] = s0(a[0],x2);
   uint8_t x1 = b[1] ^ Y[1];
   Y[2] = s0(x1,a2xa3);
   uint8_t x3 = Y[2] ^ b[3];
   Y[3] = s1(a[3],x3);
   memcpy(A,Y,4);
   memcpy(K1,Y,2);
   memcpy(K2,((char *)(Y))+2,2);
}

uint32_t f(uint32_t A, uint16_t B) {
   uint8_t a[4],b[2],Y[4];
   memcpy(a,&A,4);
   memcpy(b,&B,2);
   uint8_t a0xa1xb0 = a[0] ^ b[0] ^ a[1];
   uint8_t b1xa2xa3 = a[2] ^ b[1] ^ a[3];
   Y[1] = s1(a0xa1xb0,b1xa2xa3);
   Y[2] = s0(b1xa2xa3,Y[1]);
   Y[0] = s0(Y[1],a[0]);
   Y[3] = s1(Y[2],a[3]);
   
   uint32_t out;
   memcpy(&out,Y,4);
   return out;
}

uint16_t * process_key_FEAL(uint64_t key, uint16_t * keys) {
   uint32_t temp,A,B,D[8];
   memcpy(&A,&key,4);
   memcpy(&B,((char *)&key)+4,4);

   D[0] = A;
   fk(&A,B,&keys[0],&keys[1]);
   swap(A,B);

   for (int i = 1, idx = 2; i < 8; i++,idx+=2) {
      D[i] = A;
      fk(&A,B^D[i-1],&keys[idx],&keys[idx+1]);
      swap(A,B);
   }

   return keys;
}

uint64_t encrypt_FEAL(uint64_t block, uint64_t key) {
   uint16_t K[16];
   uint32_t temp,L,R;
   process_key_FEAL(key,K);

   uint64_t bxor,exor;
   memcpy(&bxor,((char *)&K)+8,4);
   memcpy(&exor,((char *)&K)+12,4);

   block ^= bxor;
   memcpy(&L,((char *)&block),4);
   memcpy(&R,((char *)&block)+4,4);

   R ^= L;
   for (int i = 0; i < 7; i++) {
      L ^= f(R,K[i]);
      swap(R,L);
   }
   L ^= f(R,K[7]);
   R ^= L;

   memcpy(((char *)&block),&L,4);
   memcpy(((char *)&block)+4,&R,4);

   block ^= exor;

   return block;
}

uint64_t decrypt_FEAL(uint64_t block, uint64_t key) {
   uint16_t K[16];
   uint32_t temp,L,R;
   process_key_FEAL(key,K);

   uint64_t bxor,exor;
   memcpy(&bxor,((char *)&K)+8,4);
   memcpy(&exor,((char *)&K)+12,4);

   block ^= exor;
   memcpy(&R,((char *)&block)+4,4);
   memcpy(&L,((char *)&block),4);

   R ^= L;
   for (int i = 0; i < 7; i++) {
      L ^= f(R,K[7-i]);
      swap(R,L);
   }
   L ^= f(R,K[0]);
   R ^= L;

   memcpy(((char *)&block),&L,4);
   memcpy(((char *)&block)+4,&R,4);

   block ^= bxor;

   return block;
}

void FEAL(int n, uint64_t *block, uint64_t key) {
   for (int i = 0; i < n; i++) {
      *block = encrypt_FEAL(*block,key);
   }
}

void d_FEAL(int n, uint64_t *block, uint64_t key) {
   for (int i = 0; i < n; i++) {
      *block = decrypt_FEAL(*block,key);
   }
}

