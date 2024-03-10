
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define SBOX(X) (S[X])

uint16_t spn_encrypt(uint16_t P, uint16_t * K) {
   
   uint8_t S[16] = { 0xe, 0x4, 0xd, 0x1, 0x2, 0xf, 0xb, 0x8, 0x3, 0xa, 0x6, 0xc, 0x5, 0x9, 0x0, 0x7 };

   for (int i = 0; i < 4; i++) {
      P ^= K[i];

      P = S[(P&0x000f)>>0] | S[(P&0x00f0)>>4]<<4 | S[(P&0x0f00)>>8]<<8 | S[(P&0xf000)>>12]<<12;

      P = (P&0x0001)<<0 | (P&0x0010)>>3 | (P&0x0100)>>6 | (P&0x1000)>>9 |
          (P&0x0002)<<3 | (P&0x0020)<<0 | (P&0x0200)>>3 | (P&0x2000)>>6 |
          (P&0x0004)<<6 | (P&0x0040)<<3 | (P&0x0400)<<0 | (P&0x4000)>>3 |
          (P&0x0008)<<9 | (P&0x0080)<<6 | (P&0x0800)<<3 | (P&0x8000)<<0 ;

   }
   P ^= K[4];
   return P;
}

uint16_t spn_decrypt(uint16_t P, uint16_t * K) {
   
   uint8_t S[16] = { 0xe, 0x3, 0x4, 0x8, 0x1, 0xc, 0xa, 0xf, 0x7, 0xd, 0x9, 0x6, 0xb, 0x2, 0x0, 0x5 };

   for (int i = 4; i > 0; i--) {
      P ^= K[i];

      P = (P&0x0001)<<0 | (P&0x0010)>>3 | (P&0x0100)>>6 | (P&0x1000)>>9 |
          (P&0x0002)<<3 | (P&0x0020)<<0 | (P&0x0200)>>3 | (P&0x2000)>>6 |
          (P&0x0004)<<6 | (P&0x0040)<<3 | (P&0x0400)<<0 | (P&0x4000)>>3 |
          (P&0x0008)<<9 | (P&0x0080)<<6 | (P&0x0800)<<3 | (P&0x8000)<<0 ;
      
      P = S[(P&0x000f)>>0] | S[(P&0x00f0)>>4]<<4 | S[(P&0x0f00)>>8]<<8 | S[(P&0xf000)>>12]<<12;

   }
   P ^= K[0];
   return P;
}

void calculate_bias_magnitude_spn() {
   printf("   0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f\n");
   uint8_t S[16] = { 0xe, 0x4, 0xd, 0x1, 0x2, 0xf, 0xb, 0x8, 0x3, 0xa, 0x6, 0xc, 0x5, 0x9, 0x0, 0x7 };
   uint8_t affinity,X,Y;
   for (uint8_t i = 0; i < 16; i++) {
      printf("%x ",i);
      for (uint8_t j = 0; j < 16; j++) {
         
         affinity = X = Y =0;

            for (uint8_t k = 0; k < 16; k++) {

               X = (((i&k&0x1))>>0) ^ (((i&k&0x2))>>1) ^ (((i&k&0x4))>>2) ^ (((i&k&0x8))>>3);
               Y = (((j&S[k]&0x1))>>0) ^ (((j&S[k]&0x2))>>1) ^ (((j&S[k]&0x4))>>2) ^ (((j&S[k]&0x8))>>3);

               if (X == Y) affinity++;

            }
         if (affinity-8) printf("\x1B[32m"); else printf("\x1B[31m");
         printf("% 2d\x1B[0m ",affinity-8);
      }
      printf("\n");
   }
}

int main(void) {


   uint16_t block = 0x1248;
   uint16_t key[5] = { 0x1357, 0x248a, 0x91b3, 0xc2e4, 0xdfdf };

   printf("SPN I: %04x\n",block);
   block = spn_encrypt(block,key);
   printf("SPN E: %04x\n",block);
   block = spn_decrypt(block,key);
   printf("SPN D: %04x\n",block);

   calculate_bias_magnitude_spn();


   return 0;
}
