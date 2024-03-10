/* This freaking thing is based and urealisitic I threw my compter into the wall so 
 * many times that the wall broke into pieces and is all over the floor. My computer
 * still won't break just like this stupid cipher. I am convinced at this point
 * that something is written wrong here becuase it is not working when it should
 * I have checked it like twenty times and still nothing it is absurd 
 */


#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define SBOX(X) (S[X])

uint16_t spn_encrypt(uint16_t P, const uint16_t * const K) {
   
   uint8_t S[16] = { 0xe, 0x4, 0xd, 0x1, 0x2, 0xf, 0xb, 0x8, 0x3, 0xa, 0x6, 0xc, 0x5, 0x9, 0x0, 0x7 };

   for (int i = 0; i < 3; i++) {
      P ^= K[i];

      P = S[(P&0x000f)>>0] | S[(P&0x00f0)>>4]<<4 | S[(P&0x0f00)>>8]<<8 | S[(P&0xf000)>>12]<<12;

      P = (P&0x0001)<<0 | (P&0x0010)>>3 | (P&0x0100)>>6 | (P&0x1000)>>9 |
          (P&0x0002)<<3 | (P&0x0020)<<0 | (P&0x0200)>>3 | (P&0x2000)>>6 |
          (P&0x0004)<<6 | (P&0x0040)<<3 | (P&0x0400)<<0 | (P&0x4000)>>3 |
          (P&0x0008)<<9 | (P&0x0080)<<6 | (P&0x0800)<<3 | (P&0x8000)<<0 ;

   }
   P ^= K[3];
   P = S[(P&0x000f)>>0] | S[(P&0x00f0)>>4]<<4 | S[(P&0x0f00)>>8]<<8 | S[(P&0xf000)>>12]<<12;
   P ^= K[4];
   return P;
}

uint16_t spn_decrypt(uint16_t P, uint16_t * K) {
   
   uint8_t S[16] = { 0xe, 0x3, 0x4, 0x8, 0x1, 0xc, 0xa, 0xf, 0x7, 0xd, 0x9, 0x6, 0xb, 0x2, 0x0, 0x5 };

   P ^= K[4];
   P = S[(P&0x000f)>>0] | S[(P&0x00f0)>>4]<<4 | S[(P&0x0f00)>>8]<<8 | S[(P&0xf000)>>12]<<12;
   for (int i = 3; i > 0; i--) {
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

void linear_attack_spn(int rounds, const uint16_t  * const K) {
   static const uint8_t S[16] = { 0xe, 0x3, 0x4, 0x8, 0x1, 0xc, 0xa, 0xf, 0x7, 0xd, 0x9, 0x6, 0xb, 0x2, 0x0, 0x5 }; /* decryption s-boxes */
   /* attempting to recover key bits: k5,k6,k7,k8,k13,k14,k15,k16 */
   uint8_t * text = malloc(sizeof(uint8_t)*rounds);
   for (int i = 0; i < rounds; i++) {
      text[i] = rand();
   }
   double prob;
   uint8_t test_k, want, count;
   uint16_t k_plain, k_cipher;
   for (int i = 0; i < 256; i++) { /* for all possible keys */
      test_k = i;
      count = 0;
      for (int j = 0; j < rounds; j++) { /* for all 'obtained' ciphertext */
         k_plain = text[j];
         k_cipher = spn_encrypt(k_plain,K);
         /* get desired bits */
         want = (k_cipher&0xf)>>0 | (k_cipher&0xf000)>>8;
         /* xor with test key */
         want ^= test_k;
         /* pass back through the s-boxes */
         want = (S[want&0xf]) | (S[(want&0xf0)>>4])<<4;
         /* check if linear equation holds */
         uint8_t kP = (k_plain&0x0010)>>4 ^ (k_plain&0x0040)>>6 ^ (k_plain&0x0080)>>7;
         uint8_t kW = (want&0x2)>>1 ^ (want&0x8)>>3 ^ (want&0x20)>>5 ^ (want&0x80)>>7;
         if ((kP ^ kW) == 0) {
            count++;
         }
      }
      /* evaluate results */
      count = abs(count-(rounds/2));
      prob = fabs((double)count / rounds);
      if ((K[4]>>8|((K[4]>>4)&0xf)) == test_k) 
         printf("%d probablitity of key bits: %x and %x = %f\n",count,(test_k&0xf),(test_k>>4),prob);
   }
   free(text);
}

int main(void) {

   srand(clock());


   uint16_t block = rand();
   uint16_t key[5] = { rand(), rand(), rand(), rand(), 0x4020 };

   printf("SPN I: %04x\n",block);
   block = spn_encrypt(block,key);
   printf("SPN E: %04x\n",block);
   block = spn_decrypt(block,key);
   printf("SPN D: %04x\n",block);

   calculate_bias_magnitude_spn();

   linear_attack_spn(1000000000,key);


   return 0;
}
