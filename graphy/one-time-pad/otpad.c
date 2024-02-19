
#include "otpad.h"
#include <immintrin.h>
#include <unistd.h>
#include <string.h>

void pad_encrypt(uint8_t *message, uint8_t *pad, int message_size, int pad_size) {
   /* not much we can do here */
   if (message_size > pad_size) return;

   for (int i = 0; i < message_size; i++) {
      *(message+i) += *(pad+i); 
   }
}

void pad_decrypt(uint8_t *message, uint8_t *pad, int message_size, int pad_size) {
   /* not much we can do here */
   if (message_size > pad_size) return;

   for (int i = 0; i < message_size; i++) {
      *(message+i) -= *(pad+i);
   }
}

/* grab the pad and set it to zero */
void pad_read(FILE *file, uint8_t *pad, int pad_size) {
   memcpy(pad,file,pad_size);
   fseek(file,ftell(file)-pad_size,SEEK_SET);
   memset(file,0,pad_size);
}

/*
void SIMD_pad_encrypt(uint8_t *message, uint8_t *pad, int message_size, int pad_size) {
   // not much we can do here 
   if (message_size > pad_size) return;

   int i;
   for (i = 0; i+7 < message_size; i+=8) {
      _aadd_i64((long long int *) (message+i), *(pad+i));
   }
   while (i < message_size) {
      *(message+i) += *(pad+i);
      i++;
   }
}
*/

void SIMD_pad_decrypt(uint8_t *message, uint8_t *pad, int message_size, int pad_size) {
   /* not much we can do here */
   if (message_size > pad_size) return;

   int i = 0;

   //for (i = 0; i+7 < message_size; i+=8) {
   // }
   while (i < message_size) {
      *(message+i) -= *(pad+i);
      i++;
   }
}
