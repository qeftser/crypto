
#include <stdint.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include "bits.h"

void binary_print(uint64_t block) {
   for (int i = 31; i >= 0; i--) {
      if (block & BIT[i]) putchar('1'); else putchar('0');
   }
   printf("\n");
}

void half_binary_print(uint32_t bloc1, uint32_t bloc2) {
   for (int i = 31; i >= 0; i--) {
      if (bloc1 & BIT[i]) putchar('1'); else putchar('0');
   }
   printf(" ");
   for (int i = 31; i >= 0; i--) {
      if (bloc2 & BIT[i]) putchar('1'); else putchar('0');
   }
   printf("\n");
}

uint64_t operate_DES(uint64_t block, uint64_t key, uint8_t e_d) {
   /* constant arrays */
   static uint64_t initial_p[64] = { b58, b50, b42, b34, b26, b18, b10, b02, 
                                     b60, b52, b44, b36, b28, b20, b12, b04,
                                     b62, b54, b46, b38, b30, b22, b14, b06,
                                     b64, b56, b48, b40, b32, b24, b16, b08,
                                     b57, b49, b41, b33, b25, b17, b09, b01,
                                     b59, b51, b43, b35, b27, b19, b11, b03,
                                     b61, b53, b45, b37, b29, b21, b13, b05, 
                                     b63, b55, b47, b39, b31, b23, b15, b07 };
   static uint64_t initkey_p[56] = { b57, b49, b41, b33, b25, b17, b09, b01,
                                     b58, b50, b42, b34, b26, b18, b10, b02, 
                                     b59, b51, b43, b35, b27, b19, b11, b03,
                                     b60, b52, b44, b36, b63, b55, b47, b39, 
                                     b31, b23, b15, b07, b62, b54, b46, b38,
                                     b30, b22, b14, b06, b61, b53, b45, b37,
                                     b29, b21, b13, b05, b28, b20, b12, b04 };
   static uint8_t keyshift_p[16] = { 001, 001, 002, 002, 002, 002, 002, 002,
                                     001, 002, 002, 002, 002, 002, 002, 001 };
   static uint8_t rkeyshift_p[16]= { 000, 001, 002, 002, 002, 002, 002, 002,
                                     001, 002, 002, 002, 002, 002, 002, 001 };
   static uint64_t compress_p[48]= { b14, b17, b11, b24, b01, b05, b03, b28,
                                     b15, b06, b21, b10, b23, b19, b12, b04, 
                                     b26, b08, b16, b07, b27, b20, b13, b02,
                                     b41, b52, b31, b37, b47, b55, b30, b40, 
                                     b51, b45, b33, b48, b44, b49, b39, b56,
                                     b34, b53, b46, b42, b50, b36, b29, b32 };
   static uint64_t expand_p[48] =  { b32, b01, b02, b03, b04, b05, b04, b05, 
                                     b06, b07, b08, b09, b08, b09, b10, b11,
                                     b12, b13, b12, b13, b14, b15, b16, b17, 
                                     b16, b17, b18, b19, b20, b21, b20, b21,
                                     b22, b23, b24, b25, b24, b25, b26, b27, 
                                     b28, b29, b28, b29, b30, b31, b32, b01 };
   static uint8_t Sbox_1[64] =     { 0xe, 0x4, 0xd, 0x1, 0x2, 0xf, 0xb, 0x8,
                                     0x3, 0xa, 0x6, 0xc, 0x5, 0x9, 0x0, 0x7,
                                     0x0, 0xf, 0x7, 0x4, 0xe, 0x2, 0xd, 0x1,
                                     0xa, 0x6, 0xc, 0xb, 0x9, 0x5, 0x3, 0x8,
                                     0x4, 0x1, 0xe, 0x8, 0xd, 0x6, 0x2, 0xb,
                                     0xf, 0xc, 0x9, 0x7, 0x3, 0xa, 0x5, 0x0,
                                     0xf, 0xc, 0x8, 0x2, 0x4, 0x9, 0x1, 0x7,
                                     0x5, 0xb, 0x3, 0xe, 0xa, 0x0, 0x6, 0xd };
   static uint8_t Sbox_2[64] =     { 0xf, 0x1, 0x8, 0xe, 0x6, 0xb, 0x3, 0x4,
                                     0x9, 0x7, 0x2, 0xd, 0xc, 0x0, 0x5, 0xa,
                                     0x3, 0xd, 0x4, 0x7, 0xf, 0x2, 0x8, 0xe,
                                     0xc, 0x0, 0x1, 0xa, 0x6, 0x9, 0xb, 0x5,
                                     0x0, 0xe, 0x7, 0xb, 0xa, 0x4, 0xd, 0x1,
                                     0x5, 0x8, 0xc, 0x6, 0x9, 0x3, 0x2, 0xf,
                                     0xd, 0x8, 0xa, 0x1, 0x3, 0xf, 0x4, 0x2, 
                                     0xb, 0x6, 0x7, 0xc, 0x0, 0x5, 0xe, 0x9 };
   static uint8_t Sbox_3[64] =     { 0xa, 0x0, 0x9, 0xe, 0x6, 0x3, 0xf, 0x5,
                                     0x1, 0xd, 0xc, 0x7, 0xb, 0x4, 0x2, 0x8,
                                     0xd, 0x7, 0x0, 0x9, 0x3, 0x4, 0x6, 0xa,
                                     0x2, 0x8, 0x5, 0xe, 0xc, 0xb, 0xf, 0x1,
                                     0xd, 0x6, 0x4, 0x9, 0x8, 0xf, 0x3, 0x0,
                                     0xb, 0x1, 0x2, 0xc, 0x5, 0xa, 0xe, 0x7,
                                     0x1, 0xa, 0xd, 0x0, 0x6, 0x9, 0x8, 0x7,
                                     0x4, 0xf, 0xe, 0x3, 0xb, 0x5, 0x2, 0xc };
   static uint8_t Sbox_4[64] =     { 0x7, 0xd, 0xe, 0x3, 0x0, 0x6, 0x9, 0xa, 
                                     0x1, 0x2, 0x8, 0x5, 0xb, 0xc, 0x4, 0xf,
                                     0xd, 0x8, 0xb, 0x5, 0x6, 0xf, 0x0, 0x3,
                                     0x4, 0x7, 0x2, 0xc, 0x1, 0xa, 0xe, 0x9,
                                     0xa, 0x6, 0x9, 0x0, 0xc, 0xb, 0x7, 0xd,
                                     0xf, 0x1, 0x3, 0xe, 0x5, 0x2, 0x8, 0x4,
                                     0x3, 0xf, 0x0, 0x6, 0xa, 0x1, 0xd, 0x8,
                                     0x9, 0x4, 0x5, 0xb, 0xc, 0x7, 0x2, 0xe };
   static uint8_t Sbox_5[64] =     { 0x2, 0xc, 0x4, 0x1, 0x7, 0xa, 0xb, 0x6,
                                     0x8, 0x5, 0x3, 0xf, 0xd, 0x0, 0xe, 0x9,
                                     0xe, 0xb, 0x2, 0xc, 0x4, 0x7, 0xd, 0x1, 
                                     0x5, 0x0, 0xf, 0xa, 0x3, 0x9, 0x8, 0x6,
                                     0x4, 0x2, 0x1, 0xb, 0xa, 0xd, 0x7, 0x8,
                                     0xf, 0x9, 0xc, 0x5, 0x6, 0x3, 0x0, 0xe,
                                     0xb, 0x8, 0xc, 0x7, 0x1, 0xe, 0x2, 0xd,
                                     0x6, 0xf, 0x0, 0x9, 0xa, 0x4, 0x5, 0x3 };
   static uint8_t Sbox_6[64] =     { 0xc, 0x1, 0xa, 0xf, 0x9, 0x2, 0x6, 0x8,
                                     0x0, 0xd, 0x3, 0x4, 0xe, 0x7, 0x5, 0xb,
                                     0xa, 0xf, 0x4, 0x2, 0x7, 0xc, 0x9, 0x5, 
                                     0x6, 0x1, 0xd, 0xe, 0x0, 0xb, 0x3, 0x8,
                                     0x9, 0xe, 0xf, 0x5, 0x2, 0x8, 0xc, 0x3,
                                     0x7, 0x0, 0x4, 0xa, 0x1, 0xd, 0xb, 0x6,
                                     0x4, 0x3, 0x2, 0xc, 0x9, 0x5, 0xf, 0xa,
                                     0xb, 0xe, 0x1, 0x7, 0x6, 0x0, 0x8, 0xd };
   static uint8_t Sbox_7[64] =     { 0x4, 0xb, 0x2, 0xe, 0xf, 0x0, 0x8, 0xd,
                                     0x3, 0xc, 0x9, 0x7, 0x5, 0xa, 0x6, 0x1,
                                     0xd, 0x0, 0xb, 0x7, 0x4, 0x9, 0x1, 0xa,
                                     0xe, 0x3, 0x5, 0xc, 0x2, 0xf, 0x8, 0x6,
                                     0x1, 0x4, 0xb, 0xd, 0xc, 0x3, 0x7, 0xe,
                                     0xa, 0xf, 0x6, 0x8, 0x0, 0x5, 0x9, 0x2,
                                     0x6, 0xb, 0xd, 0x8, 0x1, 0x4, 0xa, 0x7,
                                     0x9, 0x5, 0x0, 0xf, 0xe, 0x2, 0x3, 0xc };
   static uint8_t Sbox_8[64] =     { 0xd, 0x2, 0x8, 0x4, 0x6, 0xf, 0xb, 0x1, 
                                     0xa, 0x9, 0x3, 0xe, 0x5, 0x0, 0xc, 0x7,
                                     0x1, 0xf, 0xd, 0x8, 0xa, 0x3, 0x7, 0x4,
                                     0xc, 0x5, 0x6, 0xb, 0x0, 0xe, 0x9, 0x2,
                                     0x7, 0xb, 0x4, 0x1, 0x9, 0xc, 0xe, 0x2, 
                                     0x0, 0x6, 0xa, 0xd, 0xf, 0x3, 0x5, 0x8,
                                     0x2, 0x1, 0xe, 0x7, 0x4, 0xa, 0x8, 0xd, 
                                     0xf, 0xc, 0x9, 0x0, 0x3, 0x5, 0x6, 0xb };
   static uint8_t *Sbox[8] =       { Sbox_1, Sbox_2, Sbox_3, Sbox_4,
                                     Sbox_5, Sbox_6, Sbox_7, Sbox_8 };
   static uint32_t Pbox_p[32] =    { b16, b07, b20, b21, b29, b12, b28, b17,
                                     b01, b15, b23, b26, b05, b18, b31, b10,
                                     b02, b08, b24, b14, b32, b27, b03, b09,
                                     b19, b13, b30, b06, b22, b11, b04, b25 };
       
   uint64_t block_t = 0;
   uint32_t rht_b = 0, lft_b = 0;
   uint32_t temp;
   uint64_t key_t = 0;
   uint64_t expand_rht;
   uint64_t key_c;
   uint8_t idx;

   /* initial permutation */
   for (int i = 0; i < 64; i++) {
      block_t |= ((block & initial_p[i]) ? BIT[i] : 0);
   }

   /* get left and right blocks */
   memcpy(&lft_b, (char *) &block_t, 4);
   memcpy(&rht_b, ((char *) &block_t)+4, 4);

   /* key permutation */
   for (int i = 0; i < 56; i++) {
      key_t |= (key & initkey_p[i]) ? BIT[i] : 0;
   }
   
   /* DES function */
   for (int i = 0; i < 16; i++) {
      if (e_d) { /* we are encrypting */
         /* left shift key */
         key_t <<= keyshift_p[i];
         /* adjust for 'circular' behavior */
         if (keyshift_p[i] & b01) {
            key_t &= ~b01;
            key_t |= (key_t & b29) ? b01 : 0;
            key_t &= ~b29;
            key_t |= (key_t & b57) ? b29 : 0;
            key_t &= ~b57;
         }
         else {
            key_t &= ~b01; key_t &= ~b02;
            key_t |= (key_t & b29) ? b01 : 0;
            key_t |= (key_t & b30) ? b02 : 0;
            key_t &= ~b29; key_t &= ~b30;
            key_t |= (key_t & b57) ? b29 : 0;
            key_t |= (key_t & b58) ? b30 : 0;
            key_t &= ~b57; key_t &= ~b58;
         }
      }
      else { /* we are decrypting */
         /* adjust for 'circular ' behavior */
         if (rkeyshift_p[i] & b01) {
            key_t &= ~b57;
            key_t |= (key_t & b29) ? b57 : 0;
            key_t &= ~b29;
            key_t |= (key_t & b01) ? b29 : 0;
         }
         else if (i) {
            key_t &= ~b57; key_t &= ~b58;
            key_t |= (key_t & b30) ? b58 : 0;
            key_t |= (key_t & b29) ? b57 : 0;
            key_t &= ~b30; key_t &= ~b29;
            key_t |= (key_t & b02) ? b30 : 0;
            key_t |= (key_t & b01) ? b29 : 0;
         }
         /* right shift key */
         key_t >>= rkeyshift_p[i];
      }

      /* get compressed key */
      key_c = 0;
      for (int j = 0; j < 48; j++) {
         key_c |= (key_t & compress_p[j]) ? BIT[j] : 0;
      }

      /* expand right chunk */
      expand_rht = 0;
      for (int j = 0; j < 48; j++) {
         expand_rht |= (rht_b & expand_p[j]) ? BIT[j] : 0;
      }

      /* save right */
      temp = rht_b;

      /* XOR with compression */
      expand_rht ^= key_c;

      /* S-Box substitutions */
      key_c = 0; /* using as holder for S-Box result */
      for (int j = 0; j < 7; j++) {
         idx = 0;
         idx += (expand_rht & b56) ? 32 : 0;
         idx += (expand_rht & b55) ? 16 : 0;
         idx += (expand_rht & b54) ? 8  : 0;
         idx += (expand_rht & b53) ? 4  : 0;
         idx += (expand_rht & b52) ? 2  : 0;
         idx += (expand_rht & b51) ? 1  : 0;
         key_c |= Sbox[j][idx];
         key_c <<= 4;
         expand_rht <<= 6;
      }
      idx = 0;
      idx += (expand_rht & b56) ? 32 : 0;
      idx += (expand_rht & b55) ? 16 : 0;
      idx += (expand_rht & b54) ? 8  : 0;
      idx += (expand_rht & b53) ? 4  : 0;
      idx += (expand_rht & b52) ? 2  : 0;
      idx += (expand_rht & b51) ? 1  : 0;
      key_c |= Sbox[7][idx];

      /* P-Box permutation */
      rht_b = 0;
      for (int j = 0; j < 32; j++) {
         rht_b |= (key_c & Pbox_p[j]) ? BIT[j] : 0;
      }

      if (i != 15) {

         /* XOR with left */
         rht_b ^= lft_b;

         /* set left to old right */
         lft_b = temp;
      }
      else {
         lft_b ^= rht_b;
         rht_b = temp;
      }
   }

   /* write to block */
   memcpy(&block_t,&lft_b,4);
   memcpy(((char *)&block_t)+4,&rht_b,4);

   /* final permutation */
   block = 0;
   for (int i = 0; i < 64; i++) {
      block |= (block_t & BIT[i]) ? initial_p[i] : 0;
   }

   return block;
}

uint64_t encrypt_DES(uint64_t block, uint64_t key) {
   return operate_DES(block,key,1);
}

uint64_t decrypt_DES(uint64_t block, uint64_t key) {
   return operate_DES(block,key,0);
}

int main(void) {

   unsigned long num = 0x0123456789abcde7;

   uint64_t key = 0x0123456789abcdef;

   printf("before: %lu\n", num);

   num = encrypt_DES(num,key);

   printf("after:  %lu\n", num);

   printf("expct:  %lu\n",0xc95744256a5ed31d);

   num = decrypt_DES(num,key);

   printf("decry:  %lu\n", num);
}

