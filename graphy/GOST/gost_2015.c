
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

uint32_t t_magma(uint32_t input) {
    static const uint32_t S[16] = { 0x1857cb6c, 0x7edf8384, 0xe2f52526, 0xd56a1832, 
                                    0x0698d29a, 0x59214fa5, 0x81c6fa5b, 0x3cad6dc9,
                                    0x4fb07e1e, 0xf47901e8, 0xab83a74d, 0x601e5477,
                                    0x9d4b3cb0, 0xca34e9d3, 0xb3e2960f, 0x270cb0f1 };
    return (S[(input&0x0000000f)>> 0]&0x0000000f) | (S[(input&0x000000f0)>> 4]&0x000000f0) |
           (S[(input&0x00000f00)>> 8]&0x00000f00) | (S[(input&0x0000f000)>>12]&0x0000f000) |
           (S[(input&0x000f0000)>>16]&0x000f0000) | (S[(input&0x00f00000)>>20]&0x00f00000) |
           (S[(input&0x0f000000)>>24]&0x0f000000) | (S[(input&0xf0000000)>>28]&0xf0000000);
}

uint32_t g_magma(uint32_t a, uint32_t k) {
   uint32_t o = (t_magma(a+ k));
   return ((o << 11) | (o >> 21));
}

uint32_t G_magma(uint32_t a0, uint32_t a1, uint32_t k) {
   return (g_magma(a0,k) ^ a1);
}

uint64_t GX_magma(uint32_t a0, uint32_t a1, uint32_t k) {
   return (uint64_t)(g_magma(a0,k) ^ a1)<<32 | a0;
}

void generate_keys_magma(uint32_t * k, uint32_t * gk) {
   gk[ 0] = k[0];
   gk[ 1] = k[1];
   gk[ 2] = k[2];
   gk[ 3] = k[3];
   gk[ 4] = k[4];
   gk[ 5] = k[5];
   gk[ 6] = k[6];
   gk[ 7] = k[7];
   gk[ 8] = k[0];
   gk[ 9] = k[1];
   gk[10] = k[2];
   gk[11] = k[3];
   gk[12] = k[4];
   gk[13] = k[5];
   gk[14] = k[6];
   gk[15] = k[7];
   gk[16] = k[0];
   gk[17] = k[1];
   gk[18] = k[2];
   gk[19] = k[3];
   gk[20] = k[4];
   gk[21] = k[5];
   gk[22] = k[6];
   gk[23] = k[7];
   gk[24] = k[7];
   gk[25] = k[6];
   gk[26] = k[5];
   gk[27] = k[4];
   gk[28] = k[3];
   gk[29] = k[2];
   gk[30] = k[1];
   gk[31] = k[0];
}

uint64_t encrypt_magma(uint64_t block, uint32_t * keys) {
   uint32_t a1 = block>>32;
   uint32_t a0 = block&0xffffffff;
   uint32_t temp;

   printf("a1: %08x a0: %08x\n",a1,a0);

   for (int i = 0; i < 31; i++) {
      temp = a0;
      a0 = G_magma(a0,a1,keys[i]);
      a1 = temp;
      printf("a1: %08x a0: %08x\n",a1,a0);
   }

   return GX_magma(a0,a1,keys[31]);
}

uint64_t decrypt_magma(uint64_t block, uint32_t * keys) {
   uint32_t b1 = block>>32;
   uint32_t b0 = block&0xffffffff;
   uint32_t temp;

   printf("b1: %08x b0: %08x\n",b1,b0);

   for (int i = 31; i > 0; i--) {
      temp = b0;
      b0 = G_magma(b0,b1,keys[i]);
      b1 = temp;
      printf("b1: %08x b0: %08x\n",b1,b0);
   }

   return GX_magma(b0,b1,keys[31]);
}

int main(void) {

   printf("t(fdb97531) = %08x\n",t_magma(0xfdb97531));
   printf("t(2a196f34) = %08x\n",t_magma(0x2a196f34));
   printf("t(ebd9f03a) = %08x\n",t_magma(0xebd9f03a));
   printf("t(b039bb3d) = %08x\n",t_magma(0xb039bb3d));

   printf("\n\n");

   printf("g[87654321](fedcba98) = %08x\n",g_magma(0xfedcba98,0x87654321));
   printf("g[fdcbc20c](87654321) = %08x\n",g_magma(0x87654321,0xfdcbc20c));
   printf("g[7e791a4b](fdcbc20c) = %08x\n",g_magma(0xfdcbc20c,0x7e791a4b));
   printf("g[c76549ec](7e791a4b) = %08x\n",g_magma(0x7e791a4b,0xc76549ec));

   printf("\n\n");

   printf("K: ffeeddccbbaa99887766554433221100f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff\n\n");
   uint32_t K[8] = { 0xffeeddcc, 0xbbaa9988, 0x77665544, 0x33221100,
                     0xf0f1f2f3, 0xf4f5f6f7, 0xf8f9fafb, 0xfcfdfeff };
   uint32_t KG[32];
   generate_keys_magma(K,KG);
   printf("K 1: %08x, K 9: %08x, K17: %08x, K25: %08x,\n",KG[0],KG[ 8],KG[16],KG[24]);
   printf("K 2: %08x, K10: %08x, K18: %08x, K26: %08x,\n",KG[1],KG[ 9],KG[17],KG[25]);
   printf("K 3: %08x, K11: %08x, K19: %08x, K27: %08x,\n",KG[2],KG[10],KG[18],KG[26]);
   printf("K 4: %08x, K12: %08x, K20: %08x, K28: %08x,\n",KG[3],KG[11],KG[19],KG[27]);
   printf("K 5: %08x, K13: %08x, K21: %08x, K29: %08x,\n",KG[4],KG[12],KG[20],KG[28]);
   printf("K 6: %08x, K14: %08x, K22: %08x, K30: %08x,\n",KG[5],KG[13],KG[21],KG[29]);
   printf("K 7: %08x, K15: %08x, K23: %08x, K31: %08x,\n",KG[6],KG[14],KG[22],KG[30]);
   printf("K 8: %08x, K16: %08x, K24: %08x, K32: %08x.\n",KG[7],KG[15],KG[23],KG[31]);

   printf("\n\n");

   printf("a: fedcba9876543210\n\n");
   uint64_t a = 0xfedcba9876543210;
   uint64_t b = encrypt_magma(a,KG);

   printf("\nb: %016lx\n\n",b);
   a = decrypt_magma(b,KG);
   printf("\na: %016lx\n\n",a);



   return 0;
}
