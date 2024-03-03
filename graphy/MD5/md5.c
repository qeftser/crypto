
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define Fnl(x,y,z) ((x & y) | ((~ x) & z))
#define Gnl(x,y,z) ((x & z) | (y & (~ z)))
#define Hnl(x,y,z) (x ^ y ^ z)
#define Inl(x,y,z) (y ^ (x | (~ z)))

uint32_t ROL(uint8_t s, uint32_t a) {
   return ((a << s) | (a >> (32-s)));
}

void FF_MD5(uint32_t * a, uint32_t b, uint32_t c, uint32_t d, uint32_t M, uint32_t s, uint32_t t) {
   *a = b + ROL(s,(*a + Fnl(b,c,d) + M + t));
}

void GG_MD5(uint32_t * a, uint32_t b, uint32_t c, uint32_t d, uint32_t M, uint32_t s, uint32_t t) {
   *a = b + ROL(s,(*a + Gnl(b,c,d) + M + t));
}

void HH_MD5(uint32_t * a, uint32_t b, uint32_t c, uint32_t d, uint32_t M, uint32_t s, uint32_t t) {
   *a = b + ROL(s,(*a + Hnl(b,c,d) + M + t));
}

void II_MD5(uint32_t * a, uint32_t b, uint32_t c, uint32_t d, uint32_t M, uint32_t s, uint32_t t) {
   *a = b + ROL(s,(*a + Inl(b,c,d) + M + t));
}

void loop_MD5(uint32_t * V, uint32_t * M) {
   memcpy(V+4,V,16);

   FF_MD5(&V[4],V[5],V[6],V[7],M[ 0], 7,0xd76aa478);
   FF_MD5(&V[7],V[4],V[5],V[6],M[ 1],12,0xe8c7b756);
   FF_MD5(&V[6],V[7],V[4],V[5],M[ 2],17,0x242070db);
   FF_MD5(&V[5],V[6],V[7],V[4],M[ 3],22,0xc1bdceee);
   FF_MD5(&V[4],V[5],V[6],V[7],M[ 4], 7,0xf57c0faf);
   FF_MD5(&V[7],V[4],V[5],V[6],M[ 5],12,0x4787c62a);
   FF_MD5(&V[6],V[7],V[4],V[5],M[ 6],17,0xa8304613);
   FF_MD5(&V[5],V[6],V[7],V[4],M[ 7],22,0xfd469501);
   FF_MD5(&V[4],V[5],V[6],V[7],M[ 8], 7,0x698098d8);
   FF_MD5(&V[7],V[4],V[5],V[6],M[ 9],12,0x8b44f7af);
   FF_MD5(&V[6],V[7],V[4],V[5],M[10],17,0xffff5bb1);
   FF_MD5(&V[5],V[6],V[7],V[4],M[11],22,0x895cd7be);
   FF_MD5(&V[4],V[5],V[6],V[7],M[12], 7,0x6b901122);
   FF_MD5(&V[7],V[4],V[5],V[6],M[13],12,0xfd987193);
   FF_MD5(&V[6],V[7],V[4],V[5],M[14],17,0xa679438e);
   FF_MD5(&V[5],V[6],V[7],V[4],M[15],22,0x49b40821);

   GG_MD5(&V[4],V[5],V[6],V[7],M[ 1], 5,0xf61e2562);
   GG_MD5(&V[7],V[4],V[5],V[6],M[ 6], 9,0xc040b340);
   GG_MD5(&V[6],V[7],V[4],V[5],M[11],14,0x265e5a51);
   GG_MD5(&V[5],V[6],V[7],V[4],M[ 0],20,0xe9b6c7aa);
   GG_MD5(&V[4],V[5],V[6],V[7],M[ 5], 5,0xd62f105d);
   GG_MD5(&V[7],V[4],V[5],V[6],M[10], 9,0x02441453);
   GG_MD5(&V[6],V[7],V[4],V[5],M[15],14,0xd8a1e681);
   GG_MD5(&V[5],V[6],V[7],V[4],M[ 4],20,0xe7d3fbc8);
   GG_MD5(&V[4],V[5],V[6],V[7],M[ 9], 5,0x21e1cde6);
   GG_MD5(&V[7],V[4],V[5],V[6],M[14], 9,0xc33707d6);
   GG_MD5(&V[6],V[7],V[4],V[5],M[ 3],14,0xf4d50d87);
   GG_MD5(&V[5],V[6],V[7],V[4],M[ 8],20,0x455a14ed);
   GG_MD5(&V[4],V[5],V[6],V[7],M[13], 5,0xa9e3e905);
   GG_MD5(&V[7],V[4],V[5],V[6],M[ 2], 9,0xfcefa3f8);
   GG_MD5(&V[6],V[7],V[4],V[5],M[ 7],14,0x676f02d9);
   GG_MD5(&V[5],V[6],V[7],V[4],M[12],20,0x8d2a4c8a);

   HH_MD5(&V[4],V[5],V[6],V[7],M[ 5], 4,0xfffa3942);
   HH_MD5(&V[7],V[4],V[5],V[6],M[ 8],11,0x8771f681);
   HH_MD5(&V[6],V[7],V[4],V[5],M[11],16,0x6d9d6122);
   HH_MD5(&V[5],V[6],V[7],V[4],M[14],23,0xfde5380c);
   HH_MD5(&V[4],V[5],V[6],V[7],M[ 1], 4,0xa4beea44);
   HH_MD5(&V[7],V[4],V[5],V[6],M[ 4],11,0x4bdecfa9);
   HH_MD5(&V[6],V[7],V[4],V[5],M[ 7],16,0xf6bb4b60);
   HH_MD5(&V[5],V[6],V[7],V[4],M[10],23,0xbebfbc70);
   HH_MD5(&V[4],V[5],V[6],V[7],M[13], 4,0x289b7ec6);
   HH_MD5(&V[7],V[4],V[5],V[6],M[ 0],11,0xeaa127fa);
   HH_MD5(&V[6],V[7],V[4],V[5],M[ 3],16,0xd4ef3085);
   HH_MD5(&V[5],V[6],V[7],V[4],M[ 6],23,0x04881d05);
   HH_MD5(&V[4],V[5],V[6],V[7],M[ 9], 4,0xd9d4d039);
   HH_MD5(&V[7],V[4],V[5],V[6],M[12],11,0xe6db99e5);
   HH_MD5(&V[6],V[7],V[4],V[5],M[15],16,0x1fa27cf8);
   HH_MD5(&V[5],V[6],V[7],V[4],M[ 2],23,0xc4ac5665);

   II_MD5(&V[4],V[5],V[6],V[7],M[ 0], 6,0xf4292244);
   II_MD5(&V[7],V[4],V[5],V[6],M[ 7],10,0x432aff97);
   II_MD5(&V[6],V[7],V[4],V[5],M[14],15,0xab9423a7);
   II_MD5(&V[5],V[6],V[7],V[4],M[ 5],21,0xfc93a039);
   II_MD5(&V[4],V[5],V[6],V[7],M[12], 6,0x655b59c3);
   II_MD5(&V[7],V[4],V[5],V[6],M[ 3],10,0x8f0ccc92);
   II_MD5(&V[6],V[7],V[4],V[5],M[10],15,0xffeff47d);
   II_MD5(&V[5],V[6],V[7],V[4],M[ 1],21,0x85845dd1);
   II_MD5(&V[4],V[5],V[6],V[7],M[ 8], 6,0x6fa87e4f);
   II_MD5(&V[7],V[4],V[5],V[6],M[15],10,0xfe2ce6e0);
   II_MD5(&V[6],V[7],V[4],V[5],M[ 6],15,0xa3014314);
   II_MD5(&V[5],V[6],V[7],V[4],M[13],21,0x4e0811a1);
   II_MD5(&V[4],V[5],V[6],V[7],M[ 4], 6,0xf7537e82);
   II_MD5(&V[7],V[4],V[5],V[6],M[11],10,0xbd3af235);
   II_MD5(&V[6],V[7],V[4],V[5],M[ 2],15,0x2ad7d2bb);
   II_MD5(&V[5],V[6],V[7],V[4],M[ 9],21,0xeb86d391);

   V[0]+=V[4];V[1]+=V[5];V[2]+=V[6];V[3]+=V[7];
}

void MD5(uint64_t * out, uint8_t * msg, uint64_t msg_len) {
   uint32_t ABCD[8]; /* 0: A 1: B 2: C 3: D 4: a 5: b 6: c 7: d */
   uint8_t padded[64];
   uint32_t M[16];
   uint64_t og_len = msg_len;
   ABCD[0] = 0x01234567;
   ABCD[1] = 0x89abcdef;
   ABCD[2] = 0xfedcba98;
   ABCD[3] = 0x76543210;

   while (msg_len >= 64) {
      memcpy(M,msg,64);
      loop_MD5(ABCD,M);
      msg+=64; msg_len-=64;
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
      memcpy(M,padded,64);
      loop_MD5(ABCD,M);
      for (i = 0; i < 60; i++) {
         padded[i] = 0x0;
      }
      memcpy(padded+60,&og_len,4);
      memcpy(M,padded,64);
      loop_MD5(ABCD,M);
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
      memcpy(padded+60,&og_len,4);
      memcpy(M,padded,64);
      loop_MD5(ABCD,M);
   }

   memcpy(out,ABCD,16);
}

int main(void) {

   long int count = INT_MAX;

   char * msg;

   msg = malloc(sizeof(char)*count);
   for (long int i = 0; i < count; i++) {
      msg[i] = rand();
   }

   uint64_t out[2];

   MD5(out,(uint8_t *)msg,count);

   printf("hash: %lx %lx\n",out[0],out[1]);


   return 0;
}
