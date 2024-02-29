
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "../bits.h"

const unsigned long int BIT[64] = { b01, b02, b03, b04, b05, b06, b07, b08, 
                                    b09, b10, b11, b12, b13, b14, b15, b16,
                                    b17, b18, b19, b20, b21, b22, b23, b24,
                                    b25, b26, b27, b28, b29, b30, b31, b32, 
                                    b33, b34, b35, b36, b37, b38, b39, b40,
                                    b41, b42, b43, b44, b45, b46, b47, b48,
                                    b49, b50, b51, b52, b53, b54, b55, b56,
                                    b57, b58, b59, b60, b61, b62, b63, b64 };

#define ROL12(x) x = ((x << 12) | (x >> 20))
#define ROR12(x) x = ((x >> 12) | (x << 20))
#define ROL13(x) x = ((x << 13) | (x >> 19))
#define ROR13(x) x = ((x >> 13) | (x << 19))
#define swap(a,b) temp = a; a = b; b = temp

uint32_t O(uint32_t r, uint32_t c, uint16_t P) {
   uint32_t val = ((c + ((r * 17) ^ 0xff)) & 0xff);
   uint32_t accum = val;
   for (int i = 0; i < 4; i++) {
      accum = (accum * accum) % P;
   }
   if (!(accum & val)) return 0;
   return accum/val;
}

/*
void binary_print(uint32_t in) {
   for (int i = 31; i >= 0; i--) {
      if (in & BIT[i]) putchar('1'); else putchar('0');
   }
}
*/

uint32_t s_box(uint64_t R) {
   uint16_t P[16] =    { 375, 379, 391, 395, 397, 415, 419, 425,
                         433, 445, 451, 463, 471, 477, 487, 499 };
   uint16_t X[4] = {0,0,0,0};
   uint8_t out_a[4];

   for (int i = 0; i < 4; i++) {
      X[i] = R & 0x0000000000000fff;
      R >>= 12;
   }
   uint32_t r,c;
   for (int i = 0; i < 4; i++) {
      r = X[i] & 0xc003;
      c = X[i] & 0x3ffc;
      out_a[i] = O(r,c,P[r]);
   }
   uint32_t out;
   memcpy(&out,out_a,4);
   return out;
}

uint64_t encrypt_LOKI(uint64_t block, uint64_t key) {

   uint32_t EXP_p[48] = { b04, b03, b02, b01, b32, b31, b20, b29,
                          b28, b27, b26, b25, b28, b27, b26, b25,
                          b24, b23, b22, b21, b20, b19, b18, b17,
                          b20, b19, b18, b17, b16, b15, b14, b13,
                          b12, b11, b10, b09, b12, b11, b10, b09,
                          b08, b07, b06, b05, b04, b03, b02, b01 };
   uint32_t P_BOX[32] = { b32, b24, b16, b08, b31, b23, b15, b07,
                          b30, b22, b14, b06, b29, b21, b13, b05,
                          b28, b20, b12, b04, b27, b19, b11, b03,
                          b26, b18, b10, b02, b25, b17, b09, b01 };

   uint32_t temp,L,R,KL,KR;
   uint32_t RX,RS,RP;
   uint64_t RE;
   RE = RX = RS = RP = 0;

   /* split block */
   memcpy(&L,&block,4);
   memcpy(&R,((char *)&block)+4,4);

   /* split key */
   memcpy(&KL,&key,4);
   memcpy(&KR,((char *)&key)+4,4);

   //binary_print(KL);
   //putchar('\n');

   for (int i = 0; i < 16; i++) {
      /* XOR */
      RX = R ^ KL;
      if (i % 2) {
         ROL13(KL);
         swap(KL,KR);
      }
      else {
         ROL12(KL);
      }

      /* expansion permutation */
      RE = 0;
      for (int j = 0; j < 48; j++) {
         RE |= (RX & EXP_p[j]) ? BIT[j] : 0;
      }

      /* S-Box operation */
      RS = s_box(RE);

      /* P-Box permutation */
      RP = 0;
      for (int j = 0; j < 32; j++) {
         RP |= (RS & P_BOX[j]) ? BIT[j] : 0;
      }

      L ^= RP;
      swap(L,R);
   }

   /* copy results in */
   memcpy(&block,&L,4);
   memcpy(((char *)&block)+4,&R,4);

   return block;
}

uint64_t decrypt_LOKI(uint64_t block, uint64_t key) {

   uint32_t EXP_p[48] = { b04, b03, b02, b01, b32, b31, b20, b29,
                          b28, b27, b26, b25, b28, b27, b26, b25,
                          b24, b23, b22, b21, b20, b19, b18, b17,
                          b20, b19, b18, b17, b16, b15, b14, b13,
                          b12, b11, b10, b09, b12, b11, b10, b09,
                          b08, b07, b06, b05, b04, b03, b02, b01 };
   uint32_t P_BOX[32] = { b32, b24, b16, b08, b31, b23, b15, b07,
                          b30, b22, b14, b06, b29, b21, b13, b05,
                          b28, b20, b12, b04, b27, b19, b11, b03,
                          b26, b18, b10, b02, b25, b17, b09, b01 };

   uint32_t temp,L,R,KL,KR;
   uint32_t RX,RS,RP;
   uint64_t RE;
   RE = RX = RS = RP = 0;

   /* split block */
   memcpy(&L,&block,4);
   memcpy(&R,((char *)&block)+4,4);
   swap(L,R);

   /* split key */
   memcpy(&KL,&key,4);
   memcpy(&KR,((char *)&key)+4,4);


   KL = (KL << 4) | ( KL >> 28);
   KR = (KR << 4) | ( KR >> 28);

   //binary_print(KL);
   //putchar('\n');

   for (int i = 0; i < 16; i++) {
      if (!(i % 2)) {
         swap(KL,KR);
         ROR13(KL);
      }
      else {
         ROR12(KL);
      }

      /* XOR */
      RX = R ^ KL;

      /* expansion permutation */
      RE = 0;
      for (int j = 0; j < 48; j++) {
         RE |= (RX & EXP_p[j]) ? BIT[j] : 0;
      }

      /* S-Box operation */
      RS = s_box(RE);

      /* P-Box permutation */
      RP = 0;
      for (int j = 0; j < 32; j++) {
         RP |= (RS & P_BOX[j]) ? BIT[j] : 0;
      }


      L ^= RP;
      swap(L,R);
   }

   /* copy results in */
   swap(R,L);
   memcpy(&block,&L,4);
   memcpy(((char *)&block)+4,&R,4);

   return block;
}

