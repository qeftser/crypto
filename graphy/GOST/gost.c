
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#define ROL11(X) X = ((X << 11) | (X >> 21))

inline uint32_t f_GOST(uint32_t R, uint32_t K) {
   const uint8_t SB[8][16] = {{ 0x4, 0xa, 0x9, 0x2, 0xd, 0x8, 0x0, 0xe, 0x6, 0xb, 0x1, 0xc, 0x7, 0xf, 0x5, 0x3 },
                              { 0xe, 0xb, 0x4, 0xc, 0x6, 0xd, 0xf, 0xa, 0x2, 0x3, 0x8, 0x1, 0x0, 0x7, 0x5, 0x9 },
                              { 0x5, 0x8, 0x1, 0xd, 0xa, 0x3, 0x4, 0x2, 0xe, 0xf, 0xc, 0x7, 0x6, 0x0, 0x9, 0xb },
                              { 0x7, 0xd, 0xa, 0x1, 0x0, 0x8, 0x9, 0xf, 0xe, 0x4, 0x6, 0xc, 0xb, 0x2, 0x5, 0x3 },
                              { 0x6, 0xc, 0x7, 0x1, 0x5, 0xf, 0xd, 0x8, 0x4, 0xa, 0x9, 0xe, 0x0, 0x3, 0xb, 0x2 },
                              { 0x4, 0xb, 0xa, 0x0, 0x7, 0x2, 0x1, 0xd, 0x3, 0x6, 0x8, 0x5, 0x9, 0xc, 0xf, 0xe },
                              { 0xd, 0xb, 0x4, 0x1, 0x3, 0xf, 0x5, 0x9, 0x0, 0xa, 0xe, 0x7, 0x6, 0x8, 0x2, 0xc },
                              { 0x1, 0xf, 0xd, 0x0, 0x5, 0x7, 0xa, 0x4, 0x9, 0x2, 0x3, 0xe, 0x6, 0xb, 0x8, 0xc }};

   uint32_t split = R + K;
   uint32_t out = 0;

   out |= (SB[0][split & 0xf]);
   out <<= 4;
   split >>= 4;
   out |= (SB[1][split & 0xf]);
   out <<= 4;
   split >>= 4;
   out |= (SB[2][split & 0xf]);
   out <<= 4;
   split >>= 4;
   out |= (SB[3][split & 0xf]);
   out <<= 4;
   split >>= 4;
   out |= (SB[4][split & 0xf]);
   out <<= 4;
   split >>= 4;
   out |= (SB[5][split & 0xf]);
   out <<= 4;
   split >>= 4;
   out |= (SB[6][split & 0xf]);
   out <<= 4;
   split >>= 4;
   out |= (SB[7][split & 0xf]);
   out <<= 4;
   split >>= 4;

   return out;
}

void encrypt_GOST(uint64_t *block, uint32_t *key) {
   static const uint8_t K[32] = {0,1,2,3,4,5,6,7,0,1,2,3,4,5,6,7,
                                 0,1,2,3,4,5,6,7,7,6,5,4,3,2,1,0};
   uint32_t Rminus,L,R;

   L = *block>>32;
   R = *block&0xffffffff;

   for (int i = 0; i < 32; i++) {
      Rminus = R;
      R = f_GOST(R,key[K[i]]);
      ROL11(R) ^ L;
      L = Rminus;
   }

   *block = ((uint64_t)L << 32) | (R);
}

void decrypt_GOST(uint64_t *block, uint32_t *key) {
   static const uint8_t K[32] = {0,1,2,3,4,5,6,7,7,6,5,4,3,2,1,0,
                                 7,6,5,4,3,2,1,0,7,6,5,4,3,2,1,0};
   uint32_t Rminus,L,R;

   R = *block>>32;
   L = *block&0xffffffff;

   for (int i = 0; i < 32; i++) {
      Rminus = R;
      R = f_GOST(R,key[K[i]]);
      ROL11(R) ^ L;
      L = Rminus;
   }
   
   *block = ((uint64_t)R << 32) | (L);
}

int main(void) {

   uint32_t key[8] = { 0,0,0,0,0,0,0,0 };

   uint64_t plain = 0xffffffffffffffff;

   printf("Key: %08x %08x %08x %08x %08x %08x %08x %08x\n",key[0],key[1],key[2],key[3],key[4],key[5],key[6],key[7]);
   printf("plain:    %016lx\n",plain);

   encrypt_GOST(&plain,key);

   printf("encrpted: %016lx\n",plain);

   decrypt_GOST(&plain,key);

   printf("decrpted: %016lx\n",plain);


   return 0;
}
