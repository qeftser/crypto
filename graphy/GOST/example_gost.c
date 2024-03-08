#include <stdio.h>

/*****************************************************************************
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 *
 * NOTE: I am pretty sure I implimented this correctly
 * from the book but it fails to really encrypt like
 * my version of the cipher
 *
 * \_(' z ')_/
 *
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 ****************************************************************************/

typedef unsigned long u4;
typedef unsigned char byte;

typedef struct {
   u4 k[8];
   /* Constant s-boxes -- set up in gost_init(). */
   char k87[256],k65[256],k43[256],k21[256];
} gost_ctx;

/* Note: encrypt and decrypt expect full bloks--padding blocks is
 * caller's responsibility. All bulk encryption is done in ECB mode
 * by these calls. Other modes may be added easily enough. */
void gost_enc(gost_ctx *, u4 *, int);
void gost_dec(gost_ctx *, u4 *, int);
void gost_key(gost_ctx *, u4 *);
void gost_init(gost_ctx *);
void gost_destroy(gost_ctx *);

#ifdef __alpha /* Any other 64-bit machines? */
typedef unsigned int word32;
#else 
typedef unsigned long word32;
#endif

void kboxinit(gost_ctx *c) {
   int i;

   byte k8[16] = { 14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7 };
   byte k7[16] = { 15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10 };
   byte k6[16] = { 10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8 };
   byte k5[16] = { 7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15 };
   byte k4[16] = { 2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9 };
   byte k3[16] = { 12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11 };
   byte k2[16] = { 4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1 };
   byte k1[16] = { 13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7 };

   for (i = 0; i < 256; i++) {
      c->k87[i] = k8[i >> 4] << 4 | k7[i & 15];
      c->k65[i] = k6[i >> 4] << 4 | k5[i & 15];
      c->k43[i] = k4[i >> 4] << 4 | k3[i & 15];
      c->k21[i] = k2[i >> 4] << 4 | k1[i & 15];
   }
}

static word32 f(gost_ctx *c, word32 x) {
   x = c->k87[x>>24 & 255] << 24 | c->k65[x>>16 &255] << 16 |
       c->k43[x>>8 & 255] << 8 | c->k21[x & 255];

   /* rotate left 11 bits */
   return x << 11 | x >> (32 - 11);
}

void gostcrypt(gost_ctx *c, word32 *d) {
   register word32 n1, n2; /* As named in the GOST */

   n1 = d[0];
   n2 = d[1];

   /* Instead of swapping halves, swap names each round */
   n2 ^= f(c,n1+c->k[0]); n1 ^= f(c,n2+c->k[1]);
   n2 ^= f(c,n1+c->k[2]); n1 ^= f(c,n2+c->k[3]);
   n2 ^= f(c,n1+c->k[4]); n1 ^= f(c,n2+c->k[5]);
   n2 ^= f(c,n1+c->k[6]); n1 ^= f(c,n2+c->k[7]);

   n2 ^= f(c,n1+c->k[0]); n1 ^= f(c,n2+c->k[1]);
   n2 ^= f(c,n1+c->k[2]); n1 ^= f(c,n2+c->k[3]);
   n2 ^= f(c,n1+c->k[4]); n1 ^= f(c,n2+c->k[5]);
   n2 ^= f(c,n1+c->k[6]); n1 ^= f(c,n2+c->k[7]);
   
   n2 ^= f(c,n1+c->k[0]); n1 ^= f(c,n2+c->k[1]);
   n2 ^= f(c,n1+c->k[2]); n1 ^= f(c,n2+c->k[3]);
   n2 ^= f(c,n1+c->k[4]); n1 ^= f(c,n2+c->k[5]);
   n2 ^= f(c,n1+c->k[6]); n1 ^= f(c,n2+c->k[7]);

   n2 ^= f(c,n1+c->k[0]); n1 ^= f(c,n2+c->k[1]);
   n2 ^= f(c,n1+c->k[2]); n1 ^= f(c,n2+c->k[3]);
   n2 ^= f(c,n1+c->k[4]); n1 ^= f(c,n2+c->k[5]);
   n2 ^= f(c,n1+c->k[6]); n1 ^= f(c,n2+c->k[7]);

   d[0] = n2; d[1] = n1;
}

void gostdecrypt(gost_ctx *c, u4 *d) {
   register word32 n1, n2; /* As named in the GOST */

   n1 = d[0]; n2 = d[1];

   /* Instead of swapping halves, swap names each round */
   n2 ^= f(c,n1+c->k[0]); n1 ^= f(c,n2+c->k[1]);
   n2 ^= f(c,n1+c->k[2]); n1 ^= f(c,n2+c->k[3]);
   n2 ^= f(c,n1+c->k[4]); n1 ^= f(c,n2+c->k[5]);
   n2 ^= f(c,n1+c->k[6]); n1 ^= f(c,n2+c->k[7]);

   n2 ^= f(c,n1+c->k[0]); n1 ^= f(c,n2+c->k[1]);
   n2 ^= f(c,n1+c->k[2]); n1 ^= f(c,n2+c->k[3]);
   n2 ^= f(c,n1+c->k[4]); n1 ^= f(c,n2+c->k[5]);
   n2 ^= f(c,n1+c->k[6]); n1 ^= f(c,n2+c->k[7]);
   
   n2 ^= f(c,n1+c->k[0]); n1 ^= f(c,n2+c->k[1]);
   n2 ^= f(c,n1+c->k[2]); n1 ^= f(c,n2+c->k[3]);
   n2 ^= f(c,n1+c->k[4]); n1 ^= f(c,n2+c->k[5]);
   n2 ^= f(c,n1+c->k[6]); n1 ^= f(c,n2+c->k[7]);

   n2 ^= f(c,n1+c->k[0]); n1 ^= f(c,n2+c->k[1]);
   n2 ^= f(c,n1+c->k[2]); n1 ^= f(c,n2+c->k[3]);
   n2 ^= f(c,n1+c->k[4]); n1 ^= f(c,n2+c->k[5]);
   n2 ^= f(c,n1+c->k[6]); n1 ^= f(c,n2+c->k[7]);

   d[0] = n2; d[1] = n1;
}

/*
int main(void) {

   gost_ctx GOST;
   GOST.k[0] = 0;
   GOST.k[1] = 0;
   GOST.k[2] = 0;
   GOST.k[3] = 0;
   GOST.k[4] = 0;
   GOST.k[5] = 0;
   GOST.k[6] = 0;
   GOST.k[7] = 0;

   u4 plain[2] = { 0xffffffff, 0xffffffff };

   kboxinit(&GOST);

   printf("Key: %08x %08x %08x %08x %08x %08x %08x %08x\n",GOST.k[0],GOST.k[1],GOST.k[2],GOST.k[3],GOST.k[4],GOST.k[5],GOST.k[6],GOST.k[7]);

   printf("plain:     %08x %08x\n",plain[0],plain[1]);

   gostcrypt(&GOST,plain);

   printf("encrpyt:   %08x %08x\n",plain[0],plain[1]);
   
   gostdecrypt(&GOST,plain);
   
   printf("decrpyt:   %08x %08x\n",plain[0],plain[1]);

   return 0;
}
*/
