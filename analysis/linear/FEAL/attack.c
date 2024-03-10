
#include <stdio.h>
#include <stdlib.h>
#include "feal.h"

uint8_t check(uint32_t Ph, uint32_t Pl, uint32_t Ch, uint32_t Cl, uint32_t K1) {
   uint8_t Hp = (Ph&0x200)>>9 ^ (Ph&0x20000)>>17 ^ (Ph&0x8000)>>15 ^ (Ph&0x4000000)>>25;
   uint8_t Lp = (Pl&0x200)>>9 ^ (Pl&0x8000)>>15 ^ (Pl&0x4000000)>>25;
   uint8_t C = (Ch&0x200)>>9 ^ (Ch&0x20000)>>17 ^ (Ch&0x8000)>>15 ^ (Ch&0x4000000)>>25 ^ (Cl&0x8000)>>15;
   uint8_t K16 = (m_F(Ph^Pl,K1)&0x8000)>>15;
   return (Hp ^ Lp ^ C ^ K16);
}
