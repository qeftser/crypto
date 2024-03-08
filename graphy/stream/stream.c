
#include <stdint.h>
#include <stdio.h>
#include <limits.h>
#include "../bits.h"
#include "stream.h"

/*********************************************************************
 * LSFR64 
 * 
 * Will hold all primitive polynomials that can be stored in
 * a 64-bit word
 *********************************************************************/

void init_LSFR64(struct LSFR64 *lsfr, uint8_t *init_a) {
   lsfr->tap_num = init_a[0];
   lsfr->stream = 0x0000000000000000;
   lsfr->taps = init_a+1;
   for (int i = 0; i < lsfr->tap_num; i++) {
      lsfr->taps[i]--;
   }
}

uint8_t shift_LSFR64(struct LSFR64 *lsfr) {
   uint64_t nbit = (lsfr->stream >> (lsfr->taps[0]));
   for (int i = 1; i < lsfr->tap_num; i++) {
      nbit ^= (lsfr->stream >> (lsfr->taps[i]));
   }
   lsfr->stream = ((nbit & 0x00000000000000001) << (lsfr->taps[0])) | (lsfr->stream >> 1);
   return lsfr->stream & 0x00000000000000001;
}

/*********************************************************************
 * Geffe Generator
 * 
 * I have used three primitive polynomials that are relativly prime. 
 * They are:
 *    31 6
 *    31 13
 *    32 30 26 25
 * If you want the generator to be maximal length please limit the 
 * length in bits of your keys to 31, 31, and 32 respectivly.
 *********************************************************************/

uint8_t shift_Geffe(struct Geffe *);
void init_Geffe(uint64_t key1, uint64_t key2, uint64_t key3, uint64_t pre_shift, struct Geffe *g) {
   g->iteration = 0;
   g->stream1 = key1;
   g->stream2 = key2;
   g->stream3 = key3;
   for (unsigned long int i = 0; i < pre_shift; i++) {
      shift_Geffe(g);
   }
}

uint8_t shift_Geffe(struct Geffe *g) {
   g->iteration = (g->iteration+1) % ULONG_MAX;
   g->stream1 = (((g->stream1>>5)^(g->stream1>>30))<<30)|
                (g->stream1>>1);
   g->stream2 = (((g->stream2>>12)^(g->stream2>>30))<<30)|
                (g->stream2>>1);
   g->stream3 = (((g->stream3>>24)^(g->stream3>>25)^(g->stream3>>29)^(g->stream3>>31))<<31)|
                (g->stream3>>1);
   return (((g->stream1&0x1) & (g->stream2&0x1)) ^ (((~ g->stream1)&0x1) ^ (g->stream3&0x1)));
}

/*********************************************************************
 * Beth Piper Stop-and-Go Generator
 * 
 * I have used three primitive polynomials that are relativly prime. 
 * They are:
 *    31 6
 *    31 13
 *    32 30 26 25
 * If you want the generator to be maximal length please limit the 
 * length in bits of your keys to 31, 31, and 32 respectivly.
 *********************************************************************/

uint8_t shift_BPSaG(struct BPSaG *);
void init_BPSaG(uint64_t key1, uint64_t key2, uint64_t key3, uint64_t pre_shift, struct BPSaG *b) {
   b->a[0]=0; b->a[1]=0; b->a[2]=0;
   b->iteration = 0;
   b->S1 = key1;
   b->S2 = key2;
   b->S3 = key3;
   for (unsigned long int i = 0; i < pre_shift; i++) {
      shift_BPSaG(b);
   }
}

uint8_t shift_BPSaG(struct BPSaG *b) {
   b->iteration = (b->iteration + 1) % ULONG_MAX;
   if (b->a[0]) {
      b->S2 = (((b->S2>>12)^(b->S2>>30))<<30)|
              (b->S2>>1);
      b->a[1] = b->S2&0x1;
   }
   b->S1 = (((b->S1>>5)^(b->S1>>30))<<30)|
           (b->S1>>1);
   b->S3 = (((b->S3>>24)^(b->S3>>25)^(b->S3>>29)^(b->S3>>31))<<31)|
           (b->S3>>1);
   b->a[0] = b->S1&0x1;
   b->a[2] = b->S3&0x1;
   return b->a[1] ^ b->a[2];
}

/*********************************************************************
 * Bilateral Stop and Go Generator
 * 
 * I have used two primitive polynomials that are relativly prime. 
 * They are:
 *    49 9
 *    50 4 3 2
 * If you want the generator to be maximal length please limit the 
 * length in bits of your keys to 49 and 50 respectivly.
 *********************************************************************/

uint8_t shift_BSaG(struct BSaG *);
void init_BSaG(uint64_t key1, uint64_t key2, uint64_t pre_shift, struct BSaG *b) {
   b->ab[0] = 0; b->ab[1] = 0; b->ab[2] = 0;
   b->ab[3] = 0; b->ab[4] = 0; b->ab[5] = 0;
   b->iteration = 0;
   b->S1 = key1;
   b->S2 = key2;
   for (unsigned long int i = 0; i < pre_shift; i++) {
      shift_BSaG(b);
   }
}

/* For the ab array:
 *  ab[0] = a, ab[1] = a-1, ab[2] = a-2
 *  ab[3] = b, ab[4] = b-1, ab[5] = b-2
 */
uint8_t shift_BSaG(struct BSaG *b) {
   static uint8_t D = 0;
   b->iteration = (b->iteration + 1) % ULONG_MAX;

   D = b->ab[1] && b->ab[2];
   if (D) {
      b->ab[5] = b->ab[4];
      b->ab[4] = b->ab[3];
      b->S2 = (((b->S2>>1)^(b->S2>>2)^(b->S2>>3)^(b->S2>>49))<<49)|
              (b->S2>>1);
      b->ab[3] = b->S2&0x1;
   }
   if (!(D&&(!b->ab[4])&&b->ab[5])) {
      b->ab[2] = b->ab[1];
      b->ab[1] = b->ab[0];
      b->S1 = (((b->S1>>8)^(b->S1>>48))<<48)|
              (b->S1>>1);
      b->ab[0] = b->S1&0x1;
   }
   b->ab[2] = b->ab[1];
   b->ab[1] = b->ab[0];
   b->S1 = (((b->S1>>8)^(b->S1>>48))<<48)|
           (b->S1>>1);
   b->ab[0] = b->S1&0x1;

   return b->ab[0] ^ b->ab[3];
}

/*********************************************************************
 * Alternating Stop and Go Generator
 * 
 * I have used three primitive polynomials that are relativly prime. 
 * They are:
 *    64 4 3 2 1
 *    55 24
 *    63 1
 * If you want the generator to be maximal length please limit the 
 * length in bits of your keys to 64, 55, and 63 respectivly.
 *********************************************************************/

uint8_t shift_ASaG(struct ASaG *);
void init_ASaG(uint64_t key1, uint64_t key2, uint64_t key3, uint64_t pre_shift, struct ASaG *a) {
   a->bc[0] = 0; a->bc[1] = 0;
   a->iteration = 0;
   a->S1 = key1;
   a->S2 = key2;
   a->S3 = key3;
   for (unsigned long int i = 0; i < pre_shift; i++) {
      shift_ASaG(a);
   }
}

/* For the bc array:
 *   bc[0] = previous output of lsfr2
 *   bc[1] = previous output of lsfr3
 */
uint8_t shift_ASaG(struct ASaG *a) {
   a->iteration = (a->iteration + 1) % ULONG_MAX;
   a->S1 = ((a->S1^(a->S1>>1)^(a->S1>>2)^(a->S1>>3)^(a->S1>>63))<<63)|
           (a->S1>>1);
   if (a->S1&0x1) {
      a->S2 = (((a->S2>>23)^(a->S2>>54))<<54)|
              (a->S2>>1);
      a->bc[0] = a->S2&0x1;
   }
   else {
      a->S3 = (((a->S3>>62)^a->S3)<<62)|
              (a->S3>1);
      a->bc[1] = a->S3&0x1;
   }
   return (a->bc[0] ^ a->bc[1]);
}

/*********************************************************************
 * Fish
 * 
 * This is an additive generator.
 * Both keys must be arrays of 56 32-bit integers
 *********************************************************************/

void shift_Fish(uint32_t *, uint32_t *, struct Fish *);
void init_Fish(uint32_t * keyA, uint32_t * keyB, struct Fish * fish) {
   for (int i = 0; i < 56; i++) {
      fish->A[i] = keyA[i];
      fish->B[i] = keyB[i];
   }
   fish->I = 55;
}

void shift_Fish(uint32_t * R2i, uint32_t * R2ip1, struct Fish * fish) {
   do {
      fish->I = (fish->I+1)%56;
      fish->A[fish->I] = ((uint64_t)fish->A[(fish->I+1)%56] + fish->A[(fish->I+32)%56])%0x1ffffffff;
      fish->B[fish->I] = ((uint64_t)fish->B[(fish->I+4)%56] + fish->B[(fish->I+37)%56])%0x1ffffffff;
   } while (fish->B[fish->I]&0x1);
   uint32_t z2i = fish->A[fish->I];
   uint32_t h2i = fish->B[fish->I];
   do {
      fish->I = (fish->I+1)%56;
      fish->A[fish->I] = ((uint64_t)fish->A[(fish->I+1)%56] + fish->A[(fish->I+32)%56])%0x1ffffffff;
      fish->B[fish->I] = ((uint64_t)fish->B[(fish->I+4)%56] + fish->B[(fish->I+37)%56])%0x1ffffffff;
   } while (fish->B[fish->I]&0x1);
   uint32_t z2ip1 = fish->A[fish->I];
   uint32_t h2ip1 = fish->B[fish->I];
   uint32_t c2i = z2i ^ (h2i & h2ip1);
   uint32_t d2i = h2ip1 & (c2i ^ z2ip1);
   *R2i = c2i ^ d2i;
   *R2ip1 = z2ip1 ^ d2i;
}

/*********************************************************************
 * Pike
 * 
 * This is an additive generator.
 * Both keys must be arrays of 60 32-bit integers
 *********************************************************************/

uint32_t shift_Pike(struct Pike *);
void init_Pike(uint32_t * keyA, uint32_t * keyB, uint32_t * keyC, struct Pike * pike) {
   for (int i = 0; i < 60; i++) {
      pike->A[i] = keyA[i];
      pike->B[i] = keyB[i];
      pike->C[i] = keyC[i];
   }
   pike->carry[0] = 0; pike->carry[1] = 0; pike->carry[2] = 0;
   pike->I[0] = 59; pike->I[1] = 59; pike->I[2] = 59;
}

uint32_t shift_Pike(struct Pike *pike) {
   uint64_t valA, valB, valC;
   if ((pike->carry[0] & pike->carry[1] & pike->carry[2]) ||
      !(pike->carry[0] | pike->carry[1] | pike->carry[2])) {
      
      pike->I[0] = (pike->I[0] + 1)%60;
      pike->I[1] = (pike->I[1] + 1)%60;
      pike->I[2] = (pike->I[2] + 1)%60;

      valA = (uint64_t)pike->A[(pike->I[0]+5)%60] + pike->A[(pike->I[0]+36)%60];
      valB = (uint64_t)pike->B[(pike->I[1]+3)%60] + pike->B[(pike->I[1]+53)%60];
      valC = (uint64_t)pike->C[(pike->I[2]+2)%60] + pike->C[(pike->I[2]+41)%60];
      pike->carry[0] = (valA >> 8) & 0x1;
      pike->carry[1] = (valB >> 8) & 0x1;
      pike->carry[2] = (valC >> 8) & 0x1;

      pike->A[pike->I[0]] = valA % 0x1ffffffff;
      pike->B[pike->I[1]] = valB % 0x1ffffffff;
      pike->C[pike->I[2]] = valC % 0x1ffffffff;
   }
   else if (pike->carry[0] == pike->carry[1]) {
      pike->I[0] = (pike->I[0] + 1)%60;
      pike->I[1] = (pike->I[1] + 1)%60;

      valA = (uint64_t)pike->A[(pike->I[0]+5)%60] + pike->A[(pike->I[0]+36)%60];
      valB = (uint64_t)pike->B[(pike->I[1]+3)%60] + pike->B[(pike->I[1]+53)%60];
      pike->carry[0] = (valA >> 8) & 0x1;
      pike->carry[1] = (valB >> 8) & 0x1;

      pike->A[pike->I[0]] = valA % 0x1ffffffff;
      pike->B[pike->I[1]] = valB % 0x1ffffffff;
   }
   else if (pike->carry[1] == pike->carry[2]) {
      pike->I[1] = (pike->I[1] + 1)%60;
      pike->I[2] = (pike->I[2] + 1)%60;

      valB = (uint64_t)pike->B[(pike->I[1]+3)%60] + pike->B[(pike->I[1]+53)%60];
      valC = (uint64_t)pike->C[(pike->I[2]+2)%60] + pike->C[(pike->I[2]+41)%60];
      pike->carry[1] = (valB >> 8) & 0x1;
      pike->carry[2] = (valC >> 8) & 0x1;

      pike->B[pike->I[1]] = valB % 0x1ffffffff;
      pike->C[pike->I[2]] = valC % 0x1ffffffff;
   }
   else if (pike->carry[0] == pike->carry[2]) {
      pike->I[0] = (pike->I[0] + 1)%60;
      pike->I[2] = (pike->I[2] + 1)%60;

      valA = (uint64_t)pike->A[(pike->I[0]+5)%60] + pike->A[(pike->I[0]+36)%60];
      valC = (uint64_t)pike->C[(pike->I[2]+2)%60] + pike->C[(pike->I[2]+41)%60];
      pike->carry[0] = (valA >> 8) & 0x1;
      pike->carry[2] = (valC >> 8) & 0x1;

      pike->A[pike->I[0]] = valA % 0x1ffffffff;
      pike->C[pike->I[2]] = valC % 0x1ffffffff;
   }
   return pike->A[pike->I[0]] ^ pike->B[pike->I[1]] ^ pike->C[pike->I[2]];
}

/*********************************************************************
 * Mush
 * 
 * This is an additive generator.
 * Both keys must be arrays of 56 32-bit integers
 *********************************************************************/

uint32_t shift_Mush(struct Mush *);
void init_Mush(uint32_t * keyA, uint32_t *keyB, struct Mush *mush) {
   for (int i = 0; i < 56; i++) {
      mush->A[i] = keyA[i];
      mush->B[i] = keyB[i];
   }
   mush->I[0] = 56; mush->I[1] = 56;
   mush->carry[0] = 0; mush->carry[1] = 0;
}

uint32_t shift_Mush(struct Mush *mush) {
   uint64_t valA, valB;

   if (mush->carry[0]) {
      mush->I[1] = (mush->I[1]+1)%56;
      valB = (uint64_t)mush->B[(mush->I[1]+4)%56] + mush->B[(mush->I[1]+37)%56];
      mush->carry[1] = (valB >> 8)&0x1;
      mush->B[mush->I[1]] = valB%0x1ffffffff;
   }
   if (mush->carry[1]) {
      mush->I[0] = (mush->I[0]+1)%56;
      valA = (uint64_t)mush->A[(mush->I[0]+1)%56] + mush->A[(mush->I[0]+32)%56];
      mush->carry[0] = (valA >> 8)&0x1;
      mush->A[mush->I[0]] = valA%0x1ffffffff;
   }

   mush->I[0] = (mush->I[0]+1)%56;
   mush->I[1] = (mush->I[1]+1)%56;

   valA = (uint64_t)mush->A[(mush->I[0]+1)%56] + mush->A[(mush->I[0]+32)%56];
   valB = (uint64_t)mush->B[(mush->I[1]+4)%56] + mush->B[(mush->I[1]+37)%56];
   
   mush->carry[0] = (valA >> 8)&0x1;
   mush->carry[1] = (valB >> 8)&0x1;

   mush->A[mush->I[0]] = valA%0x1ffffffff;
   mush->B[mush->I[1]] = valB%0x1ffffffff;

   return mush->A[mush->I[0]] ^ mush->B[mush->I[1]];
}

/*********************************************************************
 * ORYX
 * 
 * I have used three primitive polynomials that are relativly prime. 
 * They are:
 *    32 7 6 2
 *    32 30 26 25
 *    32 7 5 3 2 1
 *  
 * If you want the generator to be maximal length please limit the 
 * length in bits of your keys to 32, 32, and 32 respectivly
 *********************************************************************/

uint8_t shift_ORYX(struct ORYX *);
void init_ORYX(uint32_t key1, uint32_t key2, uint32_t key3, uint64_t pre_shift, struct ORYX *o) {
   o->X = key1;
   o->A = key2;
   o->B = key3;
   o->iteration = 0;
   for (unsigned long int i = 0; i < pre_shift; i++) {
      shift_ORYX(o);
   }
}

uint8_t shift_ORYX(struct ORYX *o) {
   static uint8_t L[16][16] = { { 0xed,0x3e,0x0d,0x20,0xa9,0xc3,0x36,0x75,0x4c,0x2c,0x57,0xa3,0x00,0xae,0x31,0x0f },
                                { 0x19,0x4d,0x44,0xa0,0x11,0x56,0x18,0x66,0x09,0x69,0x6e,0x3d,0x25,0x9c,0xdb,0x3f },
                                { 0x65,0x58,0x1a,0x6d,0xff,0xd7,0x46,0xb3,0xb1,0x2b,0x78,0xcf,0xbe,0x26,0x42,0x2f },
                                { 0xd8,0xd4,0x8e,0x48,0x05,0xb9,0x34,0x43,0xde,0x68,0x5a,0xaa,0x9d,0xbd,0x84,0xa2 },
                                { 0x3c,0x50,0xce,0x8b,0xc5,0xd0,0xa5,0x77,0x1f,0x12,0x6b,0xc2,0xb5,0xe6,0xab,0x54 },
                                { 0x81,0x22,0x9f,0xbb,0x5c,0xa8,0xdc,0xec,0x2d,0x1e,0xee,0xd6,0x6c,0x5f,0x9a,0xfd },
                                { 0xc8,0xd5,0x94,0xfc,0x0c,0x1c,0x96,0x4f,0xf9,0x51,0xda,0x9b,0xdf,0xe1,0x47,0x37 },
                                { 0xd1,0xeb,0xaf,0xf7,0xa4,0x03,0xf0,0xc7,0x60,0xe4,0xf4,0xb4,0x85,0xf6,0x62,0x04 },
                                { 0x71,0x87,0xea,0x17,0x99,0x1d,0x3a,0x15,0x52,0x0a,0x07,0x35,0xe0,0x70,0xb6,0xfa },
                                { 0xcb,0xb0,0x86,0xa6,0x92,0xfb,0x98,0x55,0x06,0x4b,0x5d,0x4a,0x45,0x83,0xbf,0x16 },
                                { 0x7c,0x10,0x95,0x28,0x38,0x82,0xf3,0x6a,0xf8,0xfe,0x79,0x39,0x27,0x2a,0x5e,0xe7 },
                                { 0x59,0xb8,0x1b,0xca,0x8d,0xd3,0x7b,0x30,0x33,0x90,0xd2,0xd9,0xac,0x76,0x8f,0x5b },
                                { 0xa7,0x0e,0x63,0xc4,0xb2,0xe9,0x97,0x91,0x53,0x7a,0x0b,0x41,0x08,0xc1,0x8c,0x7d },
                                { 0x88,0x24,0xf5,0xf2,0x01,0x72,0xe8,0x80,0x49,0x13,0x23,0x9e,0xc6,0x14,0x73,0xad },
                                { 0x8a,0x29,0xef,0xe5,0x67,0x61,0xba,0xe2,0x7e,0x89,0x64,0x02,0xc0,0x21,0x6f,0xf1 },
                                { 0xdd,0xb7,0xc9,0xe3,0xcd,0x3b,0x93,0x2e,0x40,0xbc,0x4e,0xa1,0xcc,0x74,0x32,0x7f } };
   o->X =  (((o->X>>4)^(o->X>>5)^(o->X>>8)^(o->X>>9)^ 
             (o->X>>10)^(o->X>>13)^(o->X>>15)^(o->X>>17)^
             (o->X>>18)^(o->X>>27)^(o->X>>31)^o->X)<<31)|
           (o->X >> 1);
   if ((o->X>>29)&0x1) {
      o->A = (((o->A)^(o->A>>1)^(o->A>>3)^(o->A>>4)^(o->A>>6)^
               (o->A>>7)^(o->A>>9)^(o->A>>10)^(o->A>>11)^(o->A>>15)^
               (o->A>>21)^(o->A>>22)^(o->A>>25)^(o->A>>31))<<31)|
             (o->A >> 1);
   }
   else {
      o->A = (((o->A)^(o->A>>1)^(o->A>>6)^(o->A>>7)^(o->A>>8)^
               (o->A>>9)^(o->A>>10)^(o->A>>12)^(o->A>>16)^(o->A>>21)^
               (o->A>>22)^(o->A>>23)^(o->A>>24)^(o->A>>25)^
               (o->A>>26)^(o->A>>31))<<31)|
             (o->A >> 1);
   }
   o->B = (((o->B)^(o->B>>2)^(o->B>>5)^(o->B>>14)^(o->B>>15)^
            (o->B>>19)^(o->B>>20)^(o->B>>30)^(o->B>>31))<<31)|
          (o->B >> 1);
   if ((!(o->X>>26))&0x1) {
      o->B = (((o->B)^(o->B>>2)^(o->B>>5)^(o->B>>14)^(o->B>>15)^
               (o->B>>19)^(o->B>>20)^(o->B>>30)^(o->B>>31))<<31)|
             (o->B >> 1);
   }
   return ((o->X&0xff)+(L[(o->A&0xf0)>>4][o->A&0x0f])+(L[(o->B&0xf0)>>4][o->B&0x0f])%256);
}

/*
int main(void) {

   struct BSaG bsag;
   init_BSaG(0xfffff,0xfffff,0,&bsag);
   for (int i = 0; i < 10000; i++) {
      printf("%d",shift_BSaG(&bsag));
      if (i % 100 == 0) putchar('\n');
   }
   putchar('\n');



   return 0;
}
*/
