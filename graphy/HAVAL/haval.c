
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/* This is five-pass HAVAL */

#define HAVALf1(x6,x5,x4,x3,x2,x1,x0) (x1 & x4) ^ (x2 & x5) ^ (x3 & x6) ^ (x0 & x1) ^x0
#define HAVALf2(x6,x5,x4,x3,x2,x1,x0) (x1 & x2 & x3) ^ (x2 & x4 & x5) ^ (x1 & x2) ^ (x1 & x4) ^ \
                                      (x2 & x6) ^ (x3 & x5) ^ (x4 & x5) ^ (x0 & x2) ^ x0
#define HAVALf3(x6,x5,x4,x3,x2,x1,x0) (x1 & x2 & x3) ^ (x1 & x4) ^ (x2 & x5) ^ (x3 & x6) ^ (x0 & x3) ^ x0
#define HAVALf4(x6,x5,x4,x3,x2,x1,x0) (x1 & x2 & x3) ^ (x2 & x4 & x5) ^ (x3 & x4 & x6) ^ \
                                      (x1 & x4) ^ (x2 & x6) ^ (x3 & x4) ^ (x3 & x5) ^ \
                                      (x3 & x6) ^ (x4 & x5) ^ (x4 & x6) ^ (x0 & x4) ^ x0
#define HAVALf5(x6,x5,x4,x3,x2,x1,x0) (x1 & x4) ^ (x2 ^ x5) ^ (x3 & x6) ^ (x0 & x1 & x2 & x3) ^ (x0 & x5) ^ x0
#define ROT(X,s) ((X >> s) | (X << (32 - s)))

void H_HAVAL(uint32_t * Din, uint32_t * B) {
   static const uint8_t ord2[32] = {  5,14,26,18,11,28, 7,16, 0,23,20,22, 1,10, 4, 8,
                                     30, 3,21, 9,17,24,29, 6,19,12,15,13, 2,25,31,27 };
   static const uint8_t ord3[32] = { 19, 9, 4,20,28,17, 8,22,29,14,25,12,24,30,16,26,
                                     31,15, 7, 3, 1, 0,18,27,13, 6,21,10,23,11, 5, 2 };
   static const uint8_t ord4[32] = { 24, 4, 0,14, 2, 7,28,23,26, 6,30,20,18,25,19, 3,
                                     22,11,31,21, 8,27,12, 9, 1,29, 5,15,17,10,16,13 };
   static const uint8_t ord5[32] = { 27, 3,21,26,17,11,20,29,19, 0,12, 7,13, 8,31,10, 
                                      5, 9,14,30,18, 6,28,24, 2,23,16,22, 4, 1,25,15 };
   static const uint32_t K2[32] = { 0x452821e6, 0x38d01377, 0xbe5466cf, 0x34e90c6c, 0xc0ac29b7, 0xc97c50dd, 0x3f84d5b5, 0xb5470917,
                                    0x9216d5d9, 0x8979fb1b, 0xd1310ba6, 0x98dfb5ac, 0x2ffd72db, 0xd01adfb7, 0xb8e1afed, 0x6a267e96,
                                    0xba7c9045, 0xf12c7f99, 0x24a19947, 0xb3916cf7, 0x0801f2e2, 0x858efc16, 0x636920d8, 0x71574e69,
                                    0xa458fea3, 0xf4933d7e, 0x0d95748f, 0x728eb658, 0x718bcd58, 0x82154aee, 0x7b54a41d, 0xc25a59b5 };
   static const uint32_t K3[32] = { 0x9c30d539, 0x2af26013, 0xc5d1b023, 0x286085f0, 0xca417918, 0xb8db38ef, 0x8e79dcb0, 0x603a180e,
                                    0x6c9e0e8b, 0xb01e8a3e, 0xd71577c1, 0xbd314b27, 0x78af2fda, 0x55605c60, 0xe65525f3, 0xaa55ab94,
                                    0x57489862, 0x63e81440, 0x55ca396a, 0x2aab10b6, 0xb4cc5c34, 0x1141e8ce, 0xa15486af, 0x7c72e993,
                                    0xb3ee1411, 0x636fbc2a, 0x2ba9c55d, 0x741831f6, 0xce5c3e16, 0x9b87931e, 0xafd6ba33, 0x6c24cf5c };
   static const uint32_t K4[32] = { 0x7a325381, 0x28958677, 0x3b8f4898, 0x6b4bb9af, 0xc4bfe81b, 0x66282193, 0x61d809cc, 0xfb21a991,
                                    0x487cac60, 0x5dec8032, 0xef845d5d, 0xe98575b1, 0xdc262302, 0xeb651b88, 0x23893e81, 0xd396acc5,
                                    0x0f6d6ff3, 0x83f44239, 0x2e0b4482, 0xa4842004, 0x69c8f04a, 0x9e1f9b5e, 0x21c66842, 0xf6e96c9a,
                                    0x670c9c61, 0xabd388f0, 0x6a51a0d2, 0xd8542f68, 0x960fa728, 0xab5133a3, 0x6eef0b6c, 0x137a3be4 };
   static const uint32_t K5[32] = { 0xba3bf050, 0x7efb2a98, 0xa1f1651d, 0x39af0176, 0x66ca593e, 0x82430e88, 0x8cee8619, 0x456f9fb4,
                                    0x7d84a5c3, 0x3b8b5ebe, 0xe06f75d8, 0x85c12073, 0x401a449f, 0x56c16aa6, 0x4ed3aa62, 0x363f7706,
                                    0x1bfedf72, 0x429b023d, 0x37d0d724, 0xd00a1248, 0xdb0fead3, 0x49f1c09b, 0x075372c9, 0x80991b7b,
                                    0x25d479d8, 0xf6e8def7, 0xe3fe501a, 0xb6794c3b, 0x976ce0bd, 0x04c006ba, 0xc1a94fb6, 0x409f60c4 };

   uint32_t R, P;

   for (int PASS = 0; PASS < 5; PASS++) {
      /* H1 function */
      for (int i = 0; i < 32; i++) {
         P = HAVALf1(Din[3],Din[4],Din[1],Din[0],Din[5],Din[2],Din[6]);
         R = ROT(P,7) + ROT(Din[7],11) + B[i];
         Din[7] = Din[6]; Din[6] = Din[5]; Din[5] = Din[4]; Din[4] = Din[3];
         Din[3] = Din[2]; Din[2] = Din[1]; Din[1] = Din[0]; Din[0] = R;
      }
      /* H2 function */
      for (int i = 0; i < 32; i++) {
         P = HAVALf2(Din[6],Din[2],Din[1],Din[0],Din[3],Din[4],Din[5]);
         R = ROT(P,7) + ROT(Din[7],11) + B[ord2[i]] + K2[i];
         Din[7] = Din[6]; Din[6] = Din[5]; Din[5] = Din[4]; Din[4] = Din[3];
         Din[3] = Din[2]; Din[2] = Din[1]; Din[1] = Din[0]; Din[0] = R;
      }
      /* H3 function */
      for (int i = 0; i < 32; i++) {
         P = HAVALf3(Din[2],Din[6],Din[0],Din[4],Din[3],Din[1],Din[5]);
         R = ROT(P,7) + ROT(Din[7],11) + B[ord3[i]] + K3[i];
         Din[7] = Din[6]; Din[6] = Din[5]; Din[5] = Din[4]; Din[4] = Din[3];
         Din[3] = Din[2]; Din[2] = Din[1]; Din[1] = Din[0]; Din[0] = R;
      }
      /* H4 function */
      for (int i = 0; i < 32; i++) {
         P = HAVALf4(Din[1],Din[5],Din[3],Din[2],Din[0],Din[4],Din[6]);
         R = ROT(P,7) + ROT(Din[7],11) + B[ord4[i]] + K4[i];
         Din[7] = Din[6]; Din[6] = Din[5]; Din[5] = Din[4]; Din[4] = Din[3];
         Din[3] = Din[2]; Din[2] = Din[1]; Din[1] = Din[0]; Din[0] = R;
      }
      /* H5 function */
      for (int i = 0; i < 32; i++) {
         P = HAVALf5(Din[2],Din[5],Din[0],Din[6],Din[4],Din[3],Din[1]);
         R = ROT(P,7) + ROT(Din[7],11) + B[ord5[i]] + K5[i];
         Din[7] = Din[6]; Din[6] = Din[5]; Din[5] = Din[4]; Din[4] = Din[3];
         Din[3] = Din[2]; Din[2] = Din[1]; Din[1] = Din[0]; Din[0] = R;
      }
   }
}

void HAVAL(uint64_t * out, uint8_t * msg, uint64_t msg_len) {

   uint32_t D[8] = { 0x243f6a88, 0x85a308d3, 0x13198a2e, 0x03707344,
                     0xa4093822, 0x299f31d0, 0x082efa98, 0xec4e6c89 };
   uint32_t B[32];
   uint8_t padded[128];
   uint64_t og_len = msg_len;
   /* 3  bit version: 1
    * 3  bit pass:    5
    * 10 bit block: 256
    * 001 101 0100000000
    */
   uint16_t v_pass_block = 0x3500;


   while (msg_len >= 128) {
      memcpy(B,msg,128);
      msg+=128;msg_len-=128;
      H_HAVAL(D,B);
   }

   if (msg_len > 117) {
      uint64_t i;
      for (i = 0; i < msg_len; i++) {
         padded[i] = msg[i];
      }
      padded[i++] = 0x10;
      for (; i < 128; i++) {
         padded[i] = 0x0;
      }
      memcpy(B,padded,128);
      H_HAVAL(D,B);
      for (i = 0; i < 118; i++) {
         padded[i] = 0x0;
      }
      memcpy(padded+118,&v_pass_block,2);
      memcpy(padded+120,&og_len,8);
      memcpy(B,padded,128);
      H_HAVAL(D,B);
   }
   else {
      uint64_t i;
      for (i = 0; i < msg_len; i++) {
         padded[i] = msg[i];
      }
      padded[i++] = 0x10;
      for (; i < 118; i++) {
         padded[i] = 0x0;
      }
      memcpy(padded+118,&v_pass_block,2);
      memcpy(padded+120,&og_len,8);
      memcpy(B,padded,128);
      H_HAVAL(D,B);
   }

   memcpy(out,D,32);
}

/*
int main(void) {
   
   long int count = INT_MAX;

   char * msg;

   msg = malloc(sizeof(char)*count);
   for (long int i = 0; i < count; i++) {
      msg[i] = rand();
   }

   uint64_t out[4];

   HAVAL(out,(uint8_t *)msg,strlen(msg));

   printf("hash: %016lx %016lx %016lx %016lx\n",out[0],out[1],out[2],out[3]);

   return 0;
}
*/
