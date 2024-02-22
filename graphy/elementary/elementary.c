
#include "elementary.h"
#include <ctype.h>
#include <stdint.h>

char encipher_atbash(char c) {
   if (c < 65 || (c > 80 && c < 97) || c > 122) return c;
   uint8_t num = toupper(c) - 64;
   uint8_t dist = 13 - num;
   num += dist + dist;
   num += 64;
   return (char) num+1;
}

char decipher_atbash(char c) {
   if (c < 65 || (c > 90 && c < 97) || c > 122) return c;
   uint8_t num = toupper(c) - 65;
   uint8_t dist = 13 - num;
   num += dist + dist;
   num += 65;
   return (char) num-1;
}

char encipher_ROT13(char c) {
   if (c < 65 || (c > 90 && c < 97) || c > 122) return c;
   uint8_t num = toupper(c) - 65;
   return 65 + (num + 13) % 26;
}

char decipher_ROT13(char c) {
   return encipher_ROT13(c);
}

char encipher_ceaser(char c, int o) {
   if (c < 65 || (c > 90 && c < 97) || c > 122) return c;
   uint8_t num = toupper(c) - 65;
   return 65 + (num + o) % 26;
}

char decipher_ceaser(char c, int o) {
   if (c < 65 || (c > 90 && c < 97) || c > 122) return c;
   uint8_t num = toupper(c) - 65;
   return 65 + (num + (26 - o)) % 26;
}

