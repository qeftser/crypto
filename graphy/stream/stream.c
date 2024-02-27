
#include <stdint.h>
#include <stdio.h>
#include <limits.h>
#include "bits.h"
#include "primitive_polynomials.h"

/*********************************************************************
 * LSFR64 
 * 
 * Will hold all primitive polynomials that can be stored in
 * a 64-bit word
 *********************************************************************/
struct LSFR64 { uint64_t stream; uint8_t tap_num; uint8_t *taps; };

void init_LSFR64(struct LSFR64 *lsfr, uint8_t *init_a) {
   lsfr->tap_num = init_a[0];
   lsfr->stream = 0x0000000000000000;
   lsfr->taps = init_a+1;
   for (int i = 0; i < init_a[1]; i++) {
      lsfr->stream |= BIT[i];
   }
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
 *    11 2
 *    18 5 2 1
 *    35 2
 * If you want the generator to be maximal length please limit the 
 * length in bits of your keys to 11, 18, and 35 respectivly.
 *********************************************************************/
struct Geffe { uint64_t iteration; struct LSFR64 lsfr1; struct LSFR64 lsfr2; struct LSFR64 lsfr3; };

uint8_t shift_Geffe(struct Geffe *);
void init_Geffe(uint64_t key1, uint64_t key2, uint64_t key3, uint64_t pre_shift, struct Geffe *g) {
   uint8_t k1[3]={2,11,2},k2[5]={4,18,5,2,1},k3[3]={2,35,2};
   init_LSFR64(&g->lsfr1,k1);
   init_LSFR64(&g->lsfr2,k2);
   init_LSFR64(&g->lsfr3,k3);
   g->iteration = 0;
   g->lsfr1.stream = key1;
   g->lsfr2.stream = key2;
   g->lsfr3.stream = key3;
   for (int i = 0; i < pre_shift; i++) {
      shift_Geffe(g);
   }
}

uint8_t shift_Geffe(struct Geffe *g) {
   static uint8_t a1,a2,a3;
   g->iteration = (g->iteration+1) % ULONG_MAX;
   a1 = shift_LSFR64(&g->lsfr1);
   a2 = shift_LSFR64(&g->lsfr2);
   a3 = shift_LSFR64(&g->lsfr3);
   return ((a1 & a2) ^ (((~ a1)&0x1) ^ a3));
}

/*********************************************************************
 * Beth Piper Stop-and-Go Generator
 * 
 * I have used three primitive polynomials that are relativly prime. 
 * They are:
 *    11 2
 *    18 5 2 1
 *    35 2
 * If you want the generator to be maximal length please limit the 
 * length in bits of your keys to 11, 18, and 35 respectivly.
 *********************************************************************/
struct BPSaG { uint8_t a[3]; uint64_t iteration; struct LSFR64 lsfr1; struct LSFR64 lsfr2; struct LSFR64 lsfr3;};

uint8_t shift_BPSaG(struct BPSaG *);
void init_BPSaG(uint64_t key1, uint64_t key2, uint64_t key3, uint64_t pre_shift, struct BPSaG *b) {
   static uint8_t k1[3]={2,11,2},k2[5]={4,18,5,2,1},k3[3]={2,35,2};
   init_LSFR64(&b->lsfr1,k1);
   init_LSFR64(&b->lsfr2,k2);
   init_LSFR64(&b->lsfr3,k3);
   b->a[0]=0; b->a[1]=0; b->a[2]=0;
   b->iteration = 0;
   b->lsfr1.stream = key1;
   b->lsfr2.stream = key2;
   b->lsfr3.stream = key3;
   for (int i = 0; i < pre_shift; i++) {
      shift_BPSaG(b);
   }
}

uint8_t shift_BPSaG(struct BPSaG *b) {
   b->iteration = (b->iteration + 1) % ULONG_MAX;
   if (b->a[0]) {
      b->a[1] = shift_LSFR64(&b->lsfr2);
   }
   b->a[0] = shift_LSFR64(&b->lsfr1);
   b->a[2] = shift_LSFR64(&b->lsfr3);
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
struct BSaG { uint8_t ab[6]; uint64_t iteration; struct LSFR64 lsfr1; struct LSFR64 lsfr2; };

uint8_t shift_BSaG(struct BSaG *);
void init_BSaG(uint64_t key1, uint64_t key2, uint64_t pre_shift, struct BSaG *b) {
   uint8_t k1[3]={2,49,9},k2[5]={4,50,4,3,2};
   init_LSFR64(&b->lsfr1,k1);
   init_LSFR64(&b->lsfr2,k2);
   b->ab[0] = 0; b->ab[1] = 0; b->ab[2] = 0;
   b->ab[3] = 0; b->ab[4] = 0; b->ab[5] = 0;
   b->iteration = 0;
   b->lsfr1.stream = key1;
   b->lsfr2.stream = key2;
   for (int i = 0; i < pre_shift; i++) {
      shift_BSaG(b);
   }
}

/* For the ab array:
 *  ab[0] = a, ab[1] = a-1, ab[2] = a-2
 *  ab[3] = b, ab[4] = b-1, ab[5] = b-2
 */
uint8_t shift_BSaG(struct BSaG *b) {
   static uint8_t clocked = 0;
   b->iteration = (b->iteration + 1) % ULONG_MAX;
   b->ab[2] = b->ab[1];
   b->ab[1] = b->ab[0];
   b->ab[5] = b->ab[4];
   b->ab[4] = b->ab[3];
   if (b->ab[4] || !b->ab[5]) {
      b->ab[0] = shift_LSFR64(&b->lsfr1);
      clocked = 1;
   } else clocked = 0;
   if (!clocked && (b->ab[1] || !b->ab[2])) {
      b->ab[3] = shift_LSFR64(&b->lsfr2);
   }
   return b->ab[0] ^ b->ab[3];
}

/*********************************************************************
 * Alternating Stop and Go Generator
 * 
 * I have used three primitive polynomials that are relativly prime. 
 * They are:
 *    2 4 1
 *    55 24
 *    63 1
 * If you want the generator to be maximal length please limit the 
 * length in bits of your keys to 41, 55, and 63 respectivly.
 *********************************************************************/
struct ASaG { uint8_t bc[2]; uint64_t iteration; struct LSFR64 lsfr1; struct LSFR64 lsfr2; struct LSFR64 lsfr3;};

uint8_t shift_ASaG(struct ASaG *);
void init_ASaG(uint64_t key1, uint64_t key2, uint64_t key3, uint64_t pre_shift, struct ASaG *a) {
   static uint8_t k1[3]={2,4,1},k2[3]={2,55,24},k3[3]={2,63,1};
   init_LSFR64(&a->lsfr1,k1);
   init_LSFR64(&a->lsfr2,k2);
   init_LSFR64(&a->lsfr3,k3);
   a->bc[0] = 0; a->bc[1] = 0;
   a->iteration = 0;
   a->lsfr1.stream = key1;
   a->lsfr2.stream = key2;
   a->lsfr3.stream = key3;
   for (int i = 0; i < pre_shift; i++) {
      shift_ASaG(a);
   }
}

/* For the bc array:
 *   bc[0] = previous output of lsfr2
 *   bc[1] = previous output of lsfr3
 */
uint8_t shift_ASaG(struct ASaG *a) {
   a->iteration = (a->iteration + 1) % ULONG_MAX;
   if (shift_LSFR64(&a->lsfr1)) a->bc[0] = shift_LSFR64(&a->lsfr2);
   else a->bc[1] = shift_LSFR64(&a->lsfr3);
   return (a->bc[0] ^ a->bc[1]);
}

/*********************************************************************
 * Fish
 * 
 * This is an additive generator.
 * Both keys must be arrays of 56 32-bit integers
 *********************************************************************/
struct Fish { uint32_t A[56]; uint32_t B[56]; uint8_t I; };

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
struct Pike { uint8_t carry[3]; uint32_t A[60]; uint32_t B[60]; uint32_t C[60]; uint8_t I[3]; };

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
struct Mush { uint8_t carry[2]; uint32_t A[56]; uint32_t B[56]; uint8_t I[2]; };

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



int main(void) {

   uint32_t n1 = 0xffffffff;
   uint32_t n2 = 0xffffffff;
   uint32_t n3 = n1 + n2;

   printf("%u + %u = %u\n",n1,n2,n3);

   uint32_t keyA[60] = { 1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7,8,9,1,2,3,
                         4,5,6,7,8,9,1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7,8,9,1,2,3,4,5,6};
   uint32_t keyB[60] = { 1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7,8,9,1,2,3,
                         4,5,6,7,8,9,1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7,8,9,1,2,3,4,5,6};
   uint32_t keyC[60] = { 1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7,8,9,1,2,3,
                         4,5,6,7,8,9,1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7,8,9,1,2,3,4,5,6};
   int num = 0;

   struct Mush mush;

   init_Mush(keyA,keyB,&mush);

   for (int i = 0; i < 1000000000; i++) {
      num += shift_Mush(&mush);
      /*
      printf("%08x ",shift_Mush(&mush));
      if (i % 26 == 0) putchar('\n');
      */
   }
   putchar('\n');
   printf("num: %d\n",num);


   return 0;
}
