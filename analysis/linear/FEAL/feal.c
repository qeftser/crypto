
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <string.h>

#define swap(a,b) temp = b; b = a; a = temp

#define ROL2(x) ((x) << 2) | ((x) >> 6)

#define SA(x,y) ROL2((x+y+1)%256)
#define SB(x,y) ROL2((x+y)%256)

inline uint32_t m_F(uint32_t I, uint32_t K) {
   uint8_t Ib[4], Kb[4];
   memcpy(Ib,&I,4);
   memcpy(Kb,&K,4);
   Ib[1] ^= Ib[0] ^ Kb[1];
   Ib[2] ^= Ib[3] ^ Kb[2];
   Ib[0] ^= Kb[0];
   Ib[3] ^= Kb[3];
   Ib[1] = SA(Ib[1],Ib[2]);
   Ib[2] = SB(Ib[1],Ib[2]);
   Ib[0] = SB(Ib[1],Ib[0]);
   Ib[3] = SA(Ib[2],Ib[3]);
   uint32_t O;
   memcpy(&O,Ib,4);
   return O;
}

uint64_t FEAL4(uint64_t I, uint32_t * K) {
   uint32_t P[2], temp;
   memcpy(P,&I,8);

   P[1] ^= P[0];
   P[0] ^= m_F(P[1],K[0]);
   P[1] ^= m_F(P[0],K[1]);
   P[0] ^= K[4];
   P[1] ^= K[5];
   P[0] ^= m_F(P[1],K[2]);
   P[1] ^= m_F(P[0],K[3]);
   P[0] ^= P[1];

   uint64_t C;
   memcpy(&C,P,8);

   return C;
}



