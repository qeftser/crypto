
#ifndef CRYPTOGRAPHY_DEFINITIONS

#define CRYPTOGRAPHY_DEFINITIONS
#include <stdint.h>
#include "./stream/stream.h"

/* BLOCK CIPHERS */

uint64_t encrypt_DES(uint64_t, uint64_t);
uint64_t decrypt_DES(uint64_t, uint64_t);

uint64_t encrypt_FEAL(uint64_t, uint64_t);
uint64_t decrypt_FEAL(uint64_t, uint64_t);

void encrypt_GOST(uint64_t *, uint32_t *);
void decrypt_GOST(uint64_t *, uint32_t *);

uint64_t encrypt_LOKI(uint64_t block, uint64_t key);
uint64_t decrypt_LOKI(uint64_t block, uint64_t key);

void encrypt_MMB(uint32_t *, uint32_t *);
void decrypt_MMB(uint32_t *, uint32_t *);

void encrypt_IDEA(uint64_t *, uint64_t *);
void decrypt_IDEA(uint64_t *, uint64_t *);

void generate_keys_AES(uint32_t * k, uint32_t * w);
void encrypt_AES(uint32_t * block, uint32_t * w);
void decrypt_AES(uint32_t * block, uint32_t * w);

/* STREAM CIPHERS */

struct Geffe;
void init_Geffe(uint64_t, uint64_t, uint64_t, uint64_t, struct Geffe *);
uint8_t shift_Geffe(struct Geffe *);

struct BPSaG;
void init_BPSaG(uint64_t, uint64_t, uint64_t, uint64_t, struct BPSaG *);
uint8_t shift_BPSaG(struct BPSaG *);

struct BSaG;
void init_BSaG(uint64_t, uint64_t, uint64_t, struct BSaG *);
uint8_t shift_BSaG(struct BSaG *);

struct ASaG;
void init_ASaG(uint64_t, uint64_t, uint64_t, uint64_t, struct ASaG *);
uint8_t shift_ASaG(struct ASaG *);

struct Fish;
void init_Fish(uint32_t *, uint32_t *, struct Fish *);
void shift_Fish(uint32_t *, uint32_t *, struct Fish *);

struct Pike;
void init_Pike(uint32_t *, uint32_t *, uint32_t *, struct Pike *);
uint32_t shift_Pike(struct Pike *);

struct Mush;
void init_Mush(uint32_t *, uint32_t *, struct Mush *);
uint32_t shift_Mush(struct Mush *);

struct ORYX;
uint8_t shift_ORYX(struct ORYX *);
void init_ORYX(uint32_t, uint32_t, uint32_t, uint64_t, struct ORYX *);

/* ONE-WAY HASHES */

void MD5(uint64_t *, uint8_t *, uint64_t);

void N_Hash(uint64_t *, uint8_t *, uint64_t);

void SHA(uint32_t *, uint8_t *, uint64_t);

void HAVAL(uint64_t *, uint8_t *, uint64_t);


#endif
