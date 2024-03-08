

#ifndef STREAMS

#define STREAMS
#include <stdint.h>

struct LSFR64 { uint64_t stream; uint8_t tap_num; uint8_t *taps; };

struct Geffe { uint64_t iteration; uint32_t stream1; uint32_t stream2; uint32_t stream3; };
void init_Geffe(uint64_t, uint64_t, uint64_t, uint64_t, struct Geffe *);
uint8_t shift_Geffe(struct Geffe *);

struct BPSaG { uint8_t a[3]; uint64_t iteration; uint32_t S1; uint32_t S2; uint32_t S3; };
void init_BPSaG(uint64_t, uint64_t, uint64_t, uint64_t, struct BPSaG *);
uint8_t shift_BPSaG(struct BPSaG *);

struct BSaG { uint8_t ab[6]; uint64_t iteration; uint64_t S1; uint64_t S2; };
void init_BSaG(uint64_t, uint64_t, uint64_t, struct BSaG *);
uint8_t shift_BSaG(struct BSaG *);

struct ASaG { uint8_t bc[2]; uint64_t iteration; uint64_t S1; uint64_t S2; uint64_t S3; };
void init_ASaG(uint64_t, uint64_t, uint64_t, uint64_t, struct ASaG *);
uint8_t shift_ASaG(struct ASaG *);

struct Fish { uint32_t A[56]; uint32_t B[56]; uint8_t I; };
void init_Fish(uint32_t *, uint32_t *, struct Fish *);
void shift_Fish(uint32_t *, uint32_t *, struct Fish *);

struct Pike { uint8_t carry[3]; uint32_t A[60]; uint32_t B[60]; uint32_t C[60]; uint8_t I[3]; };
void init_Pike(uint32_t *, uint32_t *, uint32_t *, struct Pike *);
uint32_t shift_Pike(struct Pike *);

struct Mush { uint8_t carry[2]; uint32_t A[56]; uint32_t B[56]; uint8_t I[2]; };
void init_Mush(uint32_t *, uint32_t *, struct Mush *);
uint32_t shift_Mush(struct Mush *);

struct ORYX { uint64_t iteration; uint32_t X; uint32_t A; uint32_t B; }; 
uint8_t shift_ORYX(struct ORYX *);
void init_ORYX(uint32_t, uint32_t, uint32_t, uint64_t, struct ORYX *);

#endif
