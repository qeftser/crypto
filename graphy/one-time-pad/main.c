
#include "otpad.h"
#include <string.h>

int main(int argc, char **argv) {

   char pad[129] = "yeet reality";
   char message[129];

   printf("MESSAGE:\t");
   scanf("%s",message);

   pad_encrypt((uint8_t *) message,(uint8_t *) pad,strlen(message),strlen(pad));

   printf("ENCRYPTED:\t%s\n",message);

   pad_decrypt((uint8_t *) message,(uint8_t *) pad,strlen(message),strlen(pad));

   printf("DECRYPTED:\t%s\n",message);

   return 0;
}
