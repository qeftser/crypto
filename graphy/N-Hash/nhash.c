
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

#define swap(a,b)  b ^= a; a ^= b; b^= a
#define Ajk(j,k) 4*(j-1)+k

uint8_t s0_NHash(uint8_t x1, uint8_t x2) {
   uint16_t cx1 = (uint16_t) x1;
   uint8_t Y = (uint8_t) ((cx1+x2) % 256);
   Y = (Y << 2) | (Y >> 6);
   return Y;
}

uint8_t s1_NHash(uint8_t x1, uint8_t x2) {
   uint16_t cx1 = (uint16_t) x1;
   uint8_t Y = (uint8_t) ((cx1+x2+1) % 256);
   Y = (Y << 2) | (Y >> 6);
   return Y;
}

void generate_V_NHash(uint64_t * V, uint8_t j) {
   memset(V,0,16);
   V[0] |= Ajk(j,1);
   V[0] <<= 32;
   V[0] |= Ajk(j,2);
   V[1] |= Ajk(j,3);
   V[1] <<= 32;
   V[1] |= Ajk(j,4);
}

uint32_t f_NHash(uint32_t I) {
   uint8_t interm[2];

   interm[0] = (I >> 24) ^ ((I >> 16) & 0xff);
   interm[1] = ((I >> 8) & 0xff) ^ (I & 0xff);
   interm[0] = s1_NHash(interm[0],interm[1]);
   interm[1] = s0_NHash(interm[0],interm[1]);

   return ((s0_NHash(I >> 24,interm[0]) << 24) | (interm[0] << 16) | (interm[1] << 8) | (s1_NHash(interm[1],I & 0xff))); /* xD */
}

void PS_NHash(uint32_t * Y, uint32_t * X, uint32_t * P) {
   uint32_t interm;

   interm = X[0] ^ f_NHash(P[1] ^ (X[1] ^ f_NHash(X[0] ^ P[0])));
   Y[2] = X[2] ^ interm; Y[3] = X[3] ^ interm;
   interm = Y[2] ^ f_NHash(P[3] ^ (Y[3] ^ f_NHash(Y[2] ^ P[2])));
   Y[0] = X[0] ^ interm; Y[1] = X[1] ^ interm;
}

void N_Hash(uint64_t * out, uint8_t * msg, uint64_t msg_len) {
   
   uint64_t Hprev[2] = { 0x0000000000000000, 0x0000000000000000 }; /* Initial value */
   uint64_t Mcurr[2] = { 0x0,                0x0                };
   uint64_t Xproc[2] = { 0x0,                0x0                };
   uint64_t V[2];
   uint64_t Y[2];

   static uint64_t scramble[2] = { 0xaaaaaaaaaaaaaaaa, 0x5555555555555555 };

   while (msg_len >= 16) {
      memcpy(Xproc,Hprev,16);

      swap(Xproc[0],Xproc[1]);
      Xproc[0] ^= scramble[0]; Xproc[1] ^= scramble[1];

      memcpy(Mcurr,msg,16);
      msg+=16; msg_len-=16;

      Xproc[0] ^= Mcurr[0]; Xproc[1] ^= Mcurr[1];
      
      for (int j = 1; j < 16; j++) { /* fifteen rounds as recommended in 1996 lol */
         generate_V_NHash(V,j);
         V[0] ^= Hprev[0]; V[1] ^= Hprev[1];
         PS_NHash((uint32_t *)Y,(uint32_t *)Xproc,(uint32_t *)V);
         Xproc[0] = Y[0]; Xproc[1] = Y[1];
      }

      Hprev[0] = Xproc[0] ^ Hprev[0]; Hprev[1] = Xproc[1] ^ Hprev[1];
   }

   if (msg_len == 0) {
      memcpy(out,Hprev,16);
   }
   else {
      uint8_t padded[16];
      uint32_t i;
      for (i = 0; i < msg_len; i++) {
         padded[i] = msg[i];
      }
      for (; i < 16; i++) {
         padded[i] = 0x0;
      }
      memcpy(Xproc,Hprev,16);

      swap(Xproc[0],Xproc[1]);
      Xproc[0] ^= scramble[0]; Xproc[1] ^= scramble[1];

      memcpy(Mcurr,padded,16);

      Xproc[0] ^= Mcurr[0]; Xproc[1] ^= Mcurr[1];
      
      for (int j = 1; j < 16; j++) { /* fifteen rounds as recommended in 1996 lol */
         generate_V_NHash(V,j);
         V[0] ^= Hprev[0]; V[1] ^= Hprev[1];
         PS_NHash((uint32_t *)Y,(uint32_t *)Xproc,(uint32_t *)V);
         Xproc[0] = Y[0]; Xproc[1] = Y[1];
      }

      Hprev[0] = Xproc[0] ^ Hprev[0]; Hprev[1] = Xproc[1] ^ Hprev[1];
      memcpy(out,Hprev,16);
   }
}

int main(void) {

   int size = INT_MAX;


   char * msg;

   msg = malloc(sizeof(char)*size);

   for (long int i = 0; i < size; i++) {
      msg[i] = rand() % 255;
   }

   uint64_t hash[2];

   N_Hash(hash,(uint8_t *)msg,size);
   
   printf("hash: %lx %lx\n",hash[0],hash[1]);

}


