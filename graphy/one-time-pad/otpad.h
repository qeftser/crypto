
#ifndef ONETIMEPAD

#define ONETIMEPAD
#include <stdint.h>
#include <stdio.h>

void pad_encrypt(uint8_t *message, uint8_t *pad, int message_size, int pad_size);
void pad_decrypt(uint8_t *message, uint8_t *pad, int message_size, int pad_size);
void pad_read(FILE *file, uint8_t *pad, int pad_size);

#endif 

