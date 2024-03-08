
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define F1SHA(x,y,z) ((x & y) | ((~ x) & z))
#define F2SHA(x,y,z) (x ^ y ^ z)
#define F3SHA(x,y,z) ((x & y) | (x & z) | (y & z))

uint32_t ROL_SHA(uint8_t s, uint32_t a) {
   return ((a << s) | (a >> (32-s)));
}

void cycle_SHA(uint32_t * V, uint32_t W, uint8_t i) {
   uint32_t temp;
   if (i < 20) {
      temp = ROL_SHA(V[5],5) + F1SHA(V[6],V[7],V[8]) + V[9] + W + 0x5a827999;
   }
   else if (i < 40) {
      temp = ROL_SHA(V[5],5) + F2SHA(V[6],V[7],V[8]) + V[9] + W + 0x6ed9eba1;
   }
   else if (i < 60) {
      temp = ROL_SHA(V[5],5) + F3SHA(V[6],V[7],V[8]) + V[9] + W + 0x8f1bbcdc;
   }
   else {
      temp = ROL_SHA(V[5],5) + F2SHA(V[6],V[7],V[8]) + V[9] + W + 0xca62c1d6;
   }
   V[9] = V[8];
   V[8] = V[7];
   V[7] = ROL_SHA(V[6],30);
   V[6] = V[5];
   V[5] = temp;
}

void generate_W_SHA(uint32_t * W, uint32_t * M) {
   uint8_t i;
   for (i = 0; i < 16; i++) {
      W[i] = M[i];
   }
   for (; i < 80; i++) {
      W[i] = ROL_SHA(1,(W[i-3] ^ W[i-8] ^ W[i-14] ^ W[i-16]));
   }
}

void SHA(uint32_t * out, uint8_t * msg, uint64_t msg_len) {
   uint32_t M[16];
   uint32_t W[80];
   uint32_t ABCDE[10]; /* 0: A 1: B 2: C 3: D 4: E 5: a 6: b 7: c 8: d 9: e */
   uint8_t padded[64];

   uint64_t og_len = msg_len;
   ABCDE[0] = 0x67452301;
   ABCDE[1] = 0xefcdab89;
   ABCDE[2] = 0x98badcfe;
   ABCDE[3] = 0x10325476;
   ABCDE[4] = 0xc3d2e1f0;

   while (msg_len >= 64) {
      memcpy(ABCDE+5,ABCDE,20);
      memcpy(M,msg,64);
      generate_W_SHA(W,M);
      msg+=64; msg_len-=64;
      for (int i = 0; i < 80; i++) {
         cycle_SHA(ABCDE,W[i],i);
      }
      ABCDE[0]+=ABCDE[5];ABCDE[1]+=ABCDE[6];ABCDE[2]+=ABCDE[7];
      ABCDE[3]+=ABCDE[8];ABCDE[4]+=ABCDE[9];
   }
   
   if (msg_len > 59) {
      uint64_t i;
      for (i = 0; i < msg_len; i++) {
         padded[i] = msg[i];
      }
      padded[i++] = 0x10;
      for (; i < 64; i++) {
         padded[i] = 0x0;
      }
      memcpy(ABCDE+5,ABCDE,20);
      memcpy(M,padded,64);
      generate_W_SHA(W,M);
      msg+=64; msg_len-=64;
      for (int i = 0; i < 80; i++) {
         cycle_SHA(ABCDE,W[i],i);
      }
      ABCDE[0]+=ABCDE[5];ABCDE[1]+=ABCDE[6];ABCDE[2]+=ABCDE[7];
      ABCDE[3]+=ABCDE[8];ABCDE[4]+=ABCDE[9];
      for (i = 0; i < 60; i++) {
         padded[i] = 0x0;
      }
      memcpy(ABCDE+5,ABCDE,20);
      memcpy(padded+60,&og_len,4);
      memcpy(M,padded,64);
      generate_W_SHA(W,M);
      msg+=64; msg_len-=64;
      for (int i = 0; i < 80; i++) {
         cycle_SHA(ABCDE,W[i],i);
      }
      ABCDE[0]+=ABCDE[5];ABCDE[1]+=ABCDE[6];ABCDE[2]+=ABCDE[7];
      ABCDE[3]+=ABCDE[8];ABCDE[4]+=ABCDE[9];
   }
   else {
      uint64_t i;
      for (i = 0; i < msg_len; i++) {
         padded[i] = msg[i];
      }
      padded[i++] = 0x10;
      for (; i < 60; i++) {
         padded[i] = 0x0;
      }
      memcpy(ABCDE+5,ABCDE,20);
      memcpy(padded+60,&og_len,4);
      memcpy(M,padded,64);
      generate_W_SHA(W,M);
      msg+=64; msg_len-=64;
      for (int i = 0; i < 80; i++) {
         cycle_SHA(ABCDE,W[i],i);
      }
      ABCDE[0]+=ABCDE[5];ABCDE[1]+=ABCDE[6];ABCDE[2]+=ABCDE[7];
      ABCDE[3]+=ABCDE[8];ABCDE[4]+=ABCDE[9];
   }

   memcpy(out,ABCDE,20);
}

/*
int main(void) {

   long int count = INT_MAX;

   char * msg;

   msg = malloc(sizeof(char)*count);
   for (long int i = 0; i < count; i++) {
      msg[i] = rand();
   }

   uint32_t out[5];

   SHA(out,(uint8_t *)msg,count);

   printf("hash: %x %x %x %x %x\n",out[0],out[1],out[2],out[3],out[4]);

   return 0;
}
*/
