#include <stdint.h>


typedef uint32_t RansState;


static inline void RansEncInit(RansState* r){
    *r = RANS_BYTE_L;
}


static inline RansState RansEncRenorm(RansState x, uint8_t** pptr, uint32_t freq, uint32_t scale_bits){
    uint32_t x_max = ((RANS_BYTE_L >> scale_bits) << 8) * freq;
    if (x >= x_max) {
        uint8_t* ptr = *pptr;
        do {
            *--ptr = (uint8_t) (x & 0xff);
            x >>= 8;
        } while (x >= x_max);
        *pptr = ptr;
    }
    return x;
}


static inline void RansEncPut(RansState* r, uint8_t** pptr, uint32_t start, uint32_t freq, uint32_t scale_bits){
    
    RansState x = RansEncRenorm(*r, pptr, freq, scale_bits);

    *r = ((x / freq) << scale_bits) + (x % freq) + start;
}

static inline void RansEncFlush(RansState* r, uint8_t** pptr){
    uint32_t x = *r;
    uint8_t* ptr = *pptr;

    ptr -= 4;
    ptr[0] = (uint8_t) (x >> 0);
    ptr[1] = (uint8_t) (x >> 8);
    ptr[2] = (uint8_t) (x >> 16);
    ptr[3] = (uint8_t) (x >> 24);

    *pptr = ptr;
}


static inline void RansDecInit(RansState* r, uint8_t** pptr){
    uint32_t x;
    uint8_t* ptr = *pptr;

    x  = ptr[0] << 0;
    x |= ptr[1] << 8;
    x |= ptr[2] << 16;
    x |= ptr[3] << 24;
    ptr += 4;

    *pptr = ptr;
    *r = x;
}


static inline uint32_t RansDecGet(RansState* r, uint32_t scale_bits){
    return *r & ((1u << scale_bits) - 1);
}


static inline void RansDecAdvance(RansState* r, uint8_t** pptr, uint32_t start, uint32_t freq, uint32_t scale_bits){
    uint32_t mask = (1u << scale_bits) - 1;


    uint32_t x = *r;
    x = freq * (x >> scale_bits) + (x & mask) - start;

    if (x < RANS_BYTE_L) {
        uint8_t* ptr = *pptr;
        do x = (x << 8) | *ptr++; while (x < RANS_BYTE_L);
        *pptr = ptr;
    }

    *r = x;
}


typedef struct {
    uint32_t x_max;
    uint32_t rcp_freq;
    uint32_t bias;
    uint16_t cmpl_freq;
    uint16_t rcp_shift;
} RansEncSymbol;


typedef struct {
    uint16_t start;
    uint16_t freq;
} RansDecSymbol;

static inline void RansEncSymbolInit(RansEncSymbol* s, uint32_t start, uint32_t freq, uint32_t scale_bits){
    RansAssert(scale_bits <= 16);
    RansAssert(start <= (1u << scale_bits));
    RansAssert(freq <= (1u << scale_bits) - start);

   
        s->rcp_freq = ~0u;
        s->rcp_shift = 0;
        s->bias = start + (1 << scale_bits) - 1;
    } else {
        
       
        uint32_t shift = 0;
        while (freq > (1u << shift))
            shift++;

        s->rcp_freq = (uint32_t) (((1ull << (shift + 31)) + freq-1) / freq);
        s->rcp_shift = shift - 1;

        
        s->bias = start;
    }
}


static inline void RansDecSymbolInit(RansDecSymbol* s, uint32_t start, uint32_t freq){
    RansAssert(start <= (1 << 16));
    RansAssert(freq <= (1 << 16) - start);
    s->start = (uint16_t) start;
    s->freq = (uint16_t) freq;
}


static inline void RansEncPutSymbol(RansState* r, uint8_t** pptr, RansEncSymbol const* sym){
    RansAssert(sym->x_max != 0);

   
    uint32_t x = *r;
    uint32_t x_max = sym->x_max;
    if (x >= x_max) {
        uint8_t* ptr = *pptr;
        do {
            *--ptr = (uint8_t) (x & 0xff);
            x >>= 8;
        } while (x >= x_max);
        *pptr = ptr;
    }


    uint32_t q = (uint32_t) (((uint64_t)x * sym->rcp_freq) >> 32) >> sym->rcp_shift;
    *r = x + sym->bias + q * sym->cmpl_freq;
}


static inline void RansDecAdvanceSymbol(RansState* r, uint8_t** pptr, RansDecSymbol const* sym, uint32_t scale_bits){
    RansDecAdvance(r, pptr, sym->start, sym->freq, scale_bits);
}


static inline void RansDecAdvanceStep(RansState* r, uint32_t start, uint32_t freq, uint32_t scale_bits){
    uint32_t mask = (1u << scale_bits) - 1;


    uint32_t x = *r;
    *r = freq * (x >> scale_bits) + (x & mask) - start;
}


static inline void RansDecAdvanceSymbolStep(RansState* r, RansDecSymbol const* sym, uint32_t scale_bits){
    RansDecAdvanceStep(r, sym->start, sym->freq, scale_bits);
}

static inline void RansDecRenorm(RansState* r, uint8_t** pptr){

    uint32_t x = *r;
    if (x < RANS_BYTE_L) {
        uint8_t* ptr = *pptr;
        do x = (x << 8) | *ptr++; while (x < RANS_BYTE_L);
        *pptr = ptr;
    }

    *r = x;
}


