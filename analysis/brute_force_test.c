
#include "../graphy/cryptography.h"
#include "./brute-force/brute_force.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char **argv) {
   srand(clock());
   if (argc < 2) {
      printf(" Usage: ./force.e <ALGORITHM>\n");
      return 1;
   }

   if (strcmp("GOST",argv[1]) == 0) {
      uint32_t key[8];
      uint8_t plain[9] = "XxGOSTxX";
      uint64_t plain_copy;
      uint64_t cipher;
      memcpy(&cipher,plain,8);
      memcpy(&plain_copy,plain,8);
      printf("Generating random key: ");
      for (int i = 0; i < 7; i++) {
         key[i] = 0;
         printf("%08x ",key[i]);
      }
      key[7] = rand();
      printf("%08x ",key[7]);
      encrypt_GOST(&cipher,key);
      printf("\n");
      printf("Using known plaintext: \"XxGOSTxX\"\n");
      printf("With ciphertext:       %016lx\n",cipher);
      printf("\n\n");
      printf("Brute forcing...\n");
      crack_GOST(plain_copy,cipher);
      printf("\n\nFinished\n");
      printf("Initial Key: ");
      for (int i = 0; i < 8; i++) {
         printf("%08x ",key[i]);
      }
      printf("\n");
   }
}
