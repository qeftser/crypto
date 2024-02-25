
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "bits.h"

#define ROL12(x) x = ((x << 12) | (x >> 20))
#define ROR12(x) x = ((x >> 12) | (x << 20))
#define ROL13(x) x = ((x << 13) | (x >> 19))
#define ROR13(x) x = ((x >> 13) | (x << 19))
#define swap(a,b) temp = a; a = b; b = temp

uint32_t O(uint32_t r, uint32_t c, uint16_t P) {
   uint32_t val = ((c + ((r * 17) ^ 0xff)) & 0xff);
   uint32_t accum = val;
   for (int i = 0; i < 5; i++) {
      accum = (accum * accum) % P;
   }
   return accum/val;
}

void binary_print(uint32_t in) {
   for (int i = 31; i >= 0; i--) {
      if (in & BIT[i]) putchar('1'); else putchar('0');
   }
}

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

   binary_print(L);
   printf("    ");
   binary_print(R);
   printf("\n");

   /* split key */
   memcpy(&KL,&key,4);
   memcpy(&KR,((char *)&key)+4,4);

   //binary_print(KL);
   //putchar('\n');

   for (int i = 0; i < 16; i++) {
      /* XOR */
      RX = R ^ KL;
      binary_print(R);
      printf(" XOR ");
      binary_print(KL);
      printf(" --> ");
      binary_print(RX);
      printf("\n");
      if (i % 2) {
         ROL13(KL);
         swap(KL,KR);
      }
      else {
         ROL12(KL);
      }
      //binary_print(KL);
      //putchar('\n');

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

      /*
      binary_print(L);
      putchar(' ');
      binary_print(R);
      putchar('\n');
      */
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
      binary_print(R);
      printf(" XOR ");
      binary_print(KL);
      printf(" --> ");
      binary_print(RX);
      printf("\n");

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

   printf("\n");
   binary_print(L);
   printf("    ");
   binary_print(R);
   printf("\n");

   /* copy results in */
   swap(R,L);
   memcpy(&block,&L,4);
   memcpy(((char *)&block)+4,&R,4);

   return block;
}


int main(void) {
   
   uint64_t num = 0x1123456789abcdef;
   uint64_t key = 0x0100000000000010;

   printf("original:  %lu\n",num);

   num = encrypt_LOKI(num,key);

   printf("encrypted: %lu\n",num);

   num = decrypt_LOKI(num,key);

   printf("decrypted: %lu\n",num);

   return 0;
}


