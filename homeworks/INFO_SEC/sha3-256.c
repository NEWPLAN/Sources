#include <stddef.h>
#include <string.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>

typedef unsigned int	sph_u32;
typedef int		sph_s32;
typedef unsigned long long	sph_u64;
typedef long long		sph_s64;

typedef struct
{
    unsigned char	buf[144];
    size_t		ptr, lim;
    union
    {
        sph_u64 wide[25];

        sph_u32 narrow[50];
    } u;
} sph_keccak_context;
typedef sph_keccak_context sph_keccak256_context;
typedef sph_keccak_context sph_keccak512_context;
typedef unsigned char BitSequence;
typedef unsigned long long DataLength;

int out=1;
void print_hex( BitSequence str[], int len )
{
    int idx;
    for ( idx = 0; idx < len; idx++ )
        printf( "%02x", str[idx] );
}

void print_state( sph_u32 narrow[50])
{
    int i;
    print_hex(narrow, 400);
}

static inline sph_u32
sph_bswap32( sph_u32 x )
{
    __asm__ __volatile__ ("bswapl %0" : "=r" (x) : "0" (x) );
    return(x);
}

static inline sph_u64
sph_bswap64( sph_u64 x )
{
    return( ( (sph_u64) sph_bswap32( (sph_u32) x ) << 32)
            | (sph_u64) sph_bswap32( (sph_u32) (x >> 32) ) );
}

static inline void
sph_enc16be( void *dst, unsigned val )
{
    ( (unsigned char *) dst)[0]	= (val >> 8);
    ( (unsigned char *) dst)[1]	= val;
}

static inline unsigned
sph_dec16be( const void *src )
{
    return( ( (unsigned) ( ( (const unsigned char *) src)[0]) << 8)
            | (unsigned) ( ( (const unsigned char *) src)[1]) );
}

static inline void
sph_enc16le( void *dst, unsigned val )
{
    ( (unsigned char *) dst)[0]	= val;
    ( (unsigned char *) dst)[1]	= val >> 8;
}

static inline unsigned
sph_dec16le( const void *src )
{
    return( (unsigned) ( ( (const unsigned char *) src)[0])
            | ( (unsigned) ( ( (const unsigned char *) src)[1]) << 8) );
}

static inline void
sph_enc32be( void *dst, sph_u32 val )
{
    val = sph_bswap32( val );

    *(sph_u32 *) dst = val;
}

static inline void
sph_enc32be_aligned( void *dst, sph_u32 val )
{
    *(sph_u32 *) dst = sph_bswap32( val );
}

static inline sph_u32
sph_dec32be( const void *src )
{
    return(sph_bswap32( *(const sph_u32 *) src ) );
}

static inline sph_u32
sph_dec32be_aligned( const void *src )
{
    return(sph_bswap32( *(const sph_u32 *) src ) );
}

static inline void
sph_enc32le( void *dst, sph_u32 val )
{
    *(sph_u32 *) dst = val;

}

static inline void
sph_enc32le_aligned( void *dst, sph_u32 val )
{
    *(sph_u32 *) dst = val;

}

static inline sph_u32
sph_dec32le( const void *src )
{
    return(*(const sph_u32 *) src);

}

static inline sph_u32
sph_dec32le_aligned( const void *src )
{
    return(*(const sph_u32 *) src);

}

static inline void
sph_enc64be( void *dst, sph_u64 val )
{
    val = sph_bswap64( val );

    *(sph_u64 *) dst = val;
}

static inline void
sph_enc64be_aligned( void *dst, sph_u64 val )
{
    *(sph_u64 *) dst = sph_bswap64( val );
}

static inline sph_u64
sph_dec64be( const void *src )
{
    return(sph_bswap64( *(const sph_u64 *) src ) );
}

static inline sph_u64
sph_dec64be_aligned( const void *src )
{
    return(sph_bswap64( *(const sph_u64 *) src ) );
}

static inline void
sph_enc64le( void *dst, sph_u64 val )
{
    *(sph_u64 *) dst = val;
}

static inline void
sph_enc64le_aligned( void *dst, sph_u64 val )
{
    *(sph_u64 *) dst = val;
}

static inline sph_u64
sph_dec64le( const void *src )
{
    return(*(const sph_u64 *) src);
}

static inline sph_u64
sph_dec64le_aligned( const void *src )
{
    return(*(const sph_u64 *) src);
}





void sph_keccak256_init( void *cc );
void sph_keccak256( void *cc, const void *data, size_t len );
void sph_keccak256_close( void *cc, void *dst );
void sph_keccak256_addbits_and_close(
    void *cc, unsigned ub, unsigned n, void *dst );
void sph_keccak512_init( void *cc );
void sph_keccak512( void *cc, const void *data, size_t len );
void sph_keccak512_close( void *cc, void *dst );
void sph_keccak512_addbits_and_close(
    void *cc, unsigned ub, unsigned n, void *dst );




typedef enum
{
    SUCCESS		= 0,
    FAIL		= 1,
    BAD_HASHBITLEN	= 2
} HashReturn;


typedef struct
{
    union
    {
        sph_keccak256_context	ctx256;
        sph_keccak512_context	ctx512;
    }		u;
    int		hashbitlen;
    BitSequence	output[64];
    int		output_computed;
} hashState;

HashReturn Init( hashState *state, int hashbitlen );


HashReturn Update( hashState *state,
                   const BitSequence *data, DataLength databitlen );


HashReturn Final( hashState *state, BitSequence *hashval );


HashReturn Hash( int hashbitlen, const BitSequence *data,
                 DataLength databitlen, BitSequence *hashval );


static const struct
{
    sph_u32 high, low;
} RC[] =
{
    { ( (sph_u32) (0x00000000U) ), ( (sph_u32) (0x00000001U) ) },
    { ( (sph_u32) (0x00000089U) ), ( (sph_u32) (0x00000000U) ) },
    { ( (sph_u32) (0x8000008BU) ), ( (sph_u32) (0x00000000U) ) },
    { ( (sph_u32) (0x80008080U) ), ( (sph_u32) (0x00000000U) ) },
    { ( (sph_u32) (0x0000008BU) ), ( (sph_u32) (0x00000001U) ) },
    { ( (sph_u32) (0x00008000U) ), ( (sph_u32) (0x00000001U) ) },
    { ( (sph_u32) (0x80008088U) ), ( (sph_u32) (0x00000001U) ) },
    { ( (sph_u32) (0x80000082U) ), ( (sph_u32) (0x00000001U) ) },
    { ( (sph_u32) (0x0000000BU) ), ( (sph_u32) (0x00000000U) ) },
    { ( (sph_u32) (0x0000000AU) ), ( (sph_u32) (0x00000000U) ) },
    { ( (sph_u32) (0x00008082U) ), ( (sph_u32) (0x00000001U) ) },
    { ( (sph_u32) (0x00008003U) ), ( (sph_u32) (0x00000000U) ) },
    { ( (sph_u32) (0x0000808BU) ), ( (sph_u32) (0x00000001U) ) },
    { ( (sph_u32) (0x8000000BU) ), ( (sph_u32) (0x00000001U) ) },
    { ( (sph_u32) (0x8000008AU) ), ( (sph_u32) (0x00000001U) ) },
    { ( (sph_u32) (0x80000081U) ), ( (sph_u32) (0x00000001U) ) },
    { ( (sph_u32) (0x80000081U) ), ( (sph_u32) (0x00000000U) ) },
    { ( (sph_u32) (0x80000008U) ), ( (sph_u32) (0x00000000U) ) },
    { ( (sph_u32) (0x00000083U) ), ( (sph_u32) (0x00000000U) ) },
    { ( (sph_u32) (0x80008003U) ), ( (sph_u32) (0x00000000U) ) },
    { ( (sph_u32) (0x80008088U) ), ( (sph_u32) (0x00000001U) ) },
    { ( (sph_u32) (0x80000088U) ), ( (sph_u32) (0x00000000U) ) },
    { ( (sph_u32) (0x00008000U) ), ( (sph_u32) (0x00000001U) ) },
    { ( (sph_u32) (0x80008082U) ), ( (sph_u32) (0x00000000U) ) }
};

static void
keccak_init( sph_keccak_context *kc, unsigned out_size )
{
    int i;

    for ( i = 0; i < 50; i++ )
        kc->u.narrow[i] = 0;

    kc->u.narrow[2]		= ( (sph_u32) (0xFFFFFFFFU) );
    kc->u.narrow[3]		= ( (sph_u32) (0xFFFFFFFFU) );
    kc->u.narrow[4]		= ( (sph_u32) (0xFFFFFFFFU) );
    kc->u.narrow[5]		= ( (sph_u32) (0xFFFFFFFFU) );
    kc->u.narrow[16]	= ( (sph_u32) (0xFFFFFFFFU) );
    kc->u.narrow[17]	= ( (sph_u32) (0xFFFFFFFFU) );
    kc->u.narrow[24]	= ( (sph_u32) (0xFFFFFFFFU) );
    kc->u.narrow[25]	= ( (sph_u32) (0xFFFFFFFFU) );
    kc->u.narrow[34]	= ( (sph_u32) (0xFFFFFFFFU) );
    kc->u.narrow[35]	= ( (sph_u32) (0xFFFFFFFFU) );
    kc->u.narrow[40]	= ( (sph_u32) (0xFFFFFFFFU) );
    kc->u.narrow[41]	= ( (sph_u32) (0xFFFFFFFFU) );

    kc->ptr = 0;
    kc->lim = 200 - (out_size >> 2);
}

static void
keccak_core( sph_keccak_context *kc, const void *data, size_t len, size_t lim )
{
    unsigned char	*buf;
    size_t		ptr;
    int k=1;
    buf	= kc->buf;
    ptr	= kc->ptr;

    if ( len < (lim - ptr) )
    {
        memcpy( buf + ptr, data, len );
        kc->ptr = ptr + len;
        return;
    }


    while ( len > 0 )
    {
        size_t clen;

        clen = (lim - ptr);
        if ( clen > len )
            clen = len;
        memcpy( buf + ptr, data, clen );
        ptr	+= clen;
        data	= (const unsigned char *) data + clen;
        len	-= clen;
        if ( ptr == lim )
        {
            do
            {
                size_t j;
                for ( j = 0; j < (lim); j += 8 )
                {
                    sph_u32 tl, th;
                    tl = sph_dec32le_aligned( buf + j + 0 );
                    th = sph_dec32le_aligned( buf + j + 4 );
                    do
                    {
                        sph_u32 l, h, t;
                        l = (tl);
                        h = (th);
                        t = (l ^ (l >> 1) ) & ( (sph_u32) (0x22222222U) );
                        l ^= t ^ (t << 1);
                        t = (h ^ (h >> 1) ) & ( (sph_u32) (0x22222222U) );
                        h ^= t ^ (t << 1);
                        t = (l ^ (l >> 2) ) & ( (sph_u32) (0x0C0C0C0CU) );
                        l ^= t ^ (t << 2);
                        t = (h ^ (h >> 2) ) & ( (sph_u32) (0x0C0C0C0CU) );
                        h ^= t ^ (t << 2);
                        t = (l ^ (l >> 4) ) & ( (sph_u32) (0x00F000F0U) );
                        l ^= t ^ (t << 4);
                        t = (h ^ (h >> 4) ) & ( (sph_u32) (0x00F000F0U) );
                        h ^= t ^ (t << 4);
                        t = (l ^ (l >> 8) ) & ( (sph_u32) (0x0000FF00U) );
                        l ^= t ^ (t << 8);
                        t = (h ^ (h >> 8) ) & ( (sph_u32) (0x0000FF00U) );
                        h ^= t ^ (t << 8);
                        t = (l ^ ( (h << 16) & ( (sph_u32) (0xFFFFFFFFU) ) ) ) & ( (sph_u32) (0xFFFF0000U) );
                        l ^= t;
                        h ^= t >> 16;
                        (tl) = l;
                        (th) = h;
                    }
                    while ( 0 );
                    kc->u.narrow[(j >> 2) + 0] ^= tl;
                    kc->u.narrow[(j >> 2) + 1] ^= th;



                }
            }
            while ( 0 );


            do
            {
                int j;
                for ( j = 0; j < 24; j += 8 )
                {
                    do
                    {
                        do
                        {
                            sph_u64 t0l, t0h;
                            sph_u64 t1l, t1h;
                            sph_u64 t2l, t2h;
                            sph_u64 t3l, t3h;
                            sph_u64 t4l, t4h;
                            do
                            {
                                sph_u64 tt0l, tt0h;
                                sph_u64 tt1l, tt1h;
                                sph_u64 tt2l, tt2h;
                                sph_u64 tt3l, tt3h;
                                (tt0l = (kc->u.narrow[2 * 1 + 0]) ^ (kc->u.narrow[2 * 6 + 0]), tt0h = (kc->u.narrow[2 * 1 + 1]) ^ (kc->u.narrow[2 * 6 + 1]) );
                                (tt1l = (kc->u.narrow[2 * 11 + 0]) ^ (kc->u.narrow[2 * 16 + 0]), tt1h = (kc->u.narrow[2 * 11 + 1]) ^ (kc->u.narrow[2 * 16 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 21 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 21 + 1]) );
                                (tt0l = tt0l ^ tt1l, tt0h = tt0h ^ tt1h);
                                do
                                {
                                    sph_u32 tmp;
                                    tmp = tt0l;
                                    tt0l = ( (tt0h << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | (tt0h >> 31);
                                    tt0h = tmp;
                                }
                                while ( 0 );
                                (tt2l = (kc->u.narrow[2 * 4 + 0]) ^ (kc->u.narrow[2 * 9 + 0]), tt2h = (kc->u.narrow[2 * 4 + 1]) ^ (kc->u.narrow[2 * 9 + 1]) );
                                (tt3l = (kc->u.narrow[2 * 14 + 0]) ^ (kc->u.narrow[2 * 19 + 0]), tt3h = (kc->u.narrow[2 * 14 + 1]) ^ (kc->u.narrow[2 * 19 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 24 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 24 + 1]) );
                                (tt2l = tt2l ^ tt3l, tt2h = tt2h ^ tt3h);
                                (t0l = tt0l ^ tt2l, t0h = tt0h ^ tt2h);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 tt0l, tt0h;
                                sph_u64 tt1l, tt1h;
                                sph_u64 tt2l, tt2h;
                                sph_u64 tt3l, tt3h;
                                (tt0l = (kc->u.narrow[2 * 2 + 0]) ^ (kc->u.narrow[2 * 7 + 0]), tt0h = (kc->u.narrow[2 * 2 + 1]) ^ (kc->u.narrow[2 * 7 + 1]) );
                                (tt1l = (kc->u.narrow[2 * 12 + 0]) ^ (kc->u.narrow[2 * 17 + 0]), tt1h = (kc->u.narrow[2 * 12 + 1]) ^ (kc->u.narrow[2 * 17 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 22 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 22 + 1]) );
                                (tt0l = tt0l ^ tt1l, tt0h = tt0h ^ tt1h);
                                do
                                {
                                    sph_u32 tmp;
                                    tmp = tt0l;
                                    tt0l = ( (tt0h << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | (tt0h >> 31);
                                    tt0h = tmp;
                                }
                                while ( 0 );
                                (tt2l = (kc->u.narrow[2 * 0 + 0]) ^ (kc->u.narrow[2 * 5 + 0]), tt2h = (kc->u.narrow[2 * 0 + 1]) ^ (kc->u.narrow[2 * 5 + 1]) );
                                (tt3l = (kc->u.narrow[2 * 10 + 0]) ^ (kc->u.narrow[2 * 15 + 0]), tt3h = (kc->u.narrow[2 * 10 + 1]) ^ (kc->u.narrow[2 * 15 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 20 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 20 + 1]) );
                                (tt2l = tt2l ^ tt3l, tt2h = tt2h ^ tt3h);
                                (t1l = tt0l ^ tt2l, t1h = tt0h ^ tt2h);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 tt0l, tt0h;
                                sph_u64 tt1l, tt1h;
                                sph_u64 tt2l, tt2h;
                                sph_u64 tt3l, tt3h;
                                (tt0l = (kc->u.narrow[2 * 3 + 0]) ^ (kc->u.narrow[2 * 8 + 0]), tt0h = (kc->u.narrow[2 * 3 + 1]) ^ (kc->u.narrow[2 * 8 + 1]) );
                                (tt1l = (kc->u.narrow[2 * 13 + 0]) ^ (kc->u.narrow[2 * 18 + 0]), tt1h = (kc->u.narrow[2 * 13 + 1]) ^ (kc->u.narrow[2 * 18 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 23 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 23 + 1]) );
                                (tt0l = tt0l ^ tt1l, tt0h = tt0h ^ tt1h);
                                do
                                {
                                    sph_u32 tmp;
                                    tmp = tt0l;
                                    tt0l = ( (tt0h << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | (tt0h >> 31);
                                    tt0h = tmp;
                                }
                                while ( 0 );
                                (tt2l = (kc->u.narrow[2 * 1 + 0]) ^ (kc->u.narrow[2 * 6 + 0]), tt2h = (kc->u.narrow[2 * 1 + 1]) ^ (kc->u.narrow[2 * 6 + 1]) );
                                (tt3l = (kc->u.narrow[2 * 11 + 0]) ^ (kc->u.narrow[2 * 16 + 0]), tt3h = (kc->u.narrow[2 * 11 + 1]) ^ (kc->u.narrow[2 * 16 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 21 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 21 + 1]) );
                                (tt2l = tt2l ^ tt3l, tt2h = tt2h ^ tt3h);
                                (t2l = tt0l ^ tt2l, t2h = tt0h ^ tt2h);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 tt0l, tt0h;
                                sph_u64 tt1l, tt1h;
                                sph_u64 tt2l, tt2h;
                                sph_u64 tt3l, tt3h;
                                (tt0l = (kc->u.narrow[2 * 4 + 0]) ^ (kc->u.narrow[2 * 9 + 0]), tt0h = (kc->u.narrow[2 * 4 + 1]) ^ (kc->u.narrow[2 * 9 + 1]) );
                                (tt1l = (kc->u.narrow[2 * 14 + 0]) ^ (kc->u.narrow[2 * 19 + 0]), tt1h = (kc->u.narrow[2 * 14 + 1]) ^ (kc->u.narrow[2 * 19 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 24 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 24 + 1]) );
                                (tt0l = tt0l ^ tt1l, tt0h = tt0h ^ tt1h);
                                do
                                {
                                    sph_u32 tmp;
                                    tmp = tt0l;
                                    tt0l = ( (tt0h << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | (tt0h >> 31);
                                    tt0h = tmp;
                                }
                                while ( 0 );
                                (tt2l = (kc->u.narrow[2 * 2 + 0]) ^ (kc->u.narrow[2 * 7 + 0]), tt2h = (kc->u.narrow[2 * 2 + 1]) ^ (kc->u.narrow[2 * 7 + 1]) );
                                (tt3l = (kc->u.narrow[2 * 12 + 0]) ^ (kc->u.narrow[2 * 17 + 0]), tt3h = (kc->u.narrow[2 * 12 + 1]) ^ (kc->u.narrow[2 * 17 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 22 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 22 + 1]) );
                                (tt2l = tt2l ^ tt3l, tt2h = tt2h ^ tt3h);
                                (t3l = tt0l ^ tt2l, t3h = tt0h ^ tt2h);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 tt0l, tt0h;
                                sph_u64 tt1l, tt1h;
                                sph_u64 tt2l, tt2h;
                                sph_u64 tt3l, tt3h;
                                (tt0l = (kc->u.narrow[2 * 0 + 0]) ^ (kc->u.narrow[2 * 5 + 0]), tt0h = (kc->u.narrow[2 * 0 + 1]) ^ (kc->u.narrow[2 * 5 + 1]) );
                                (tt1l = (kc->u.narrow[2 * 10 + 0]) ^ (kc->u.narrow[2 * 15 + 0]), tt1h = (kc->u.narrow[2 * 10 + 1]) ^ (kc->u.narrow[2 * 15 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 20 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 20 + 1]) );
                                (tt0l = tt0l ^ tt1l, tt0h = tt0h ^ tt1h);
                                do
                                {
                                    sph_u32 tmp;
                                    tmp = tt0l;
                                    tt0l = ( (tt0h << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | (tt0h >> 31);
                                    tt0h = tmp;
                                }
                                while ( 0 );
                                (tt2l = (kc->u.narrow[2 * 3 + 0]) ^ (kc->u.narrow[2 * 8 + 0]), tt2h = (kc->u.narrow[2 * 3 + 1]) ^ (kc->u.narrow[2 * 8 + 1]) );
                                (tt3l = (kc->u.narrow[2 * 13 + 0]) ^ (kc->u.narrow[2 * 18 + 0]), tt3h = (kc->u.narrow[2 * 13 + 1]) ^ (kc->u.narrow[2 * 18 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 23 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 23 + 1]) );
                                (tt2l = tt2l ^ tt3l, tt2h = tt2h ^ tt3h);
                                (t4l = tt0l ^ tt2l, t4h = tt0h ^ tt2h);
                            }
                            while ( 0 );
                            ( (kc->u.narrow[2 * 0 + 0]) = (kc->u.narrow[2 * 0 + 0]) ^ t0l, (kc->u.narrow[2 * 0 + 1]) = (kc->u.narrow[2 * 0 + 1]) ^ t0h);
                            ( (kc->u.narrow[2 * 5 + 0]) = (kc->u.narrow[2 * 5 + 0]) ^ t0l, (kc->u.narrow[2 * 5 + 1]) = (kc->u.narrow[2 * 5 + 1]) ^ t0h);
                            ( (kc->u.narrow[2 * 10 + 0]) = (kc->u.narrow[2 * 10 + 0]) ^ t0l, (kc->u.narrow[2 * 10 + 1]) = (kc->u.narrow[2 * 10 + 1]) ^ t0h);
                            ( (kc->u.narrow[2 * 15 + 0]) = (kc->u.narrow[2 * 15 + 0]) ^ t0l, (kc->u.narrow[2 * 15 + 1]) = (kc->u.narrow[2 * 15 + 1]) ^ t0h);
                            ( (kc->u.narrow[2 * 20 + 0]) = (kc->u.narrow[2 * 20 + 0]) ^ t0l, (kc->u.narrow[2 * 20 + 1]) = (kc->u.narrow[2 * 20 + 1]) ^ t0h);
                            ( (kc->u.narrow[2 * 1 + 0]) = (kc->u.narrow[2 * 1 + 0]) ^ t1l, (kc->u.narrow[2 * 1 + 1]) = (kc->u.narrow[2 * 1 + 1]) ^ t1h);
                            ( (kc->u.narrow[2 * 6 + 0]) = (kc->u.narrow[2 * 6 + 0]) ^ t1l, (kc->u.narrow[2 * 6 + 1]) = (kc->u.narrow[2 * 6 + 1]) ^ t1h);
                            ( (kc->u.narrow[2 * 11 + 0]) = (kc->u.narrow[2 * 11 + 0]) ^ t1l, (kc->u.narrow[2 * 11 + 1]) = (kc->u.narrow[2 * 11 + 1]) ^ t1h);
                            ( (kc->u.narrow[2 * 16 + 0]) = (kc->u.narrow[2 * 16 + 0]) ^ t1l, (kc->u.narrow[2 * 16 + 1]) = (kc->u.narrow[2 * 16 + 1]) ^ t1h);
                            ( (kc->u.narrow[2 * 21 + 0]) = (kc->u.narrow[2 * 21 + 0]) ^ t1l, (kc->u.narrow[2 * 21 + 1]) = (kc->u.narrow[2 * 21 + 1]) ^ t1h);
                            ( (kc->u.narrow[2 * 2 + 0]) = (kc->u.narrow[2 * 2 + 0]) ^ t2l, (kc->u.narrow[2 * 2 + 1]) = (kc->u.narrow[2 * 2 + 1]) ^ t2h);
                            ( (kc->u.narrow[2 * 7 + 0]) = (kc->u.narrow[2 * 7 + 0]) ^ t2l, (kc->u.narrow[2 * 7 + 1]) = (kc->u.narrow[2 * 7 + 1]) ^ t2h);
                            ( (kc->u.narrow[2 * 12 + 0]) = (kc->u.narrow[2 * 12 + 0]) ^ t2l, (kc->u.narrow[2 * 12 + 1]) = (kc->u.narrow[2 * 12 + 1]) ^ t2h);
                            ( (kc->u.narrow[2 * 17 + 0]) = (kc->u.narrow[2 * 17 + 0]) ^ t2l, (kc->u.narrow[2 * 17 + 1]) = (kc->u.narrow[2 * 17 + 1]) ^ t2h);
                            ( (kc->u.narrow[2 * 22 + 0]) = (kc->u.narrow[2 * 22 + 0]) ^ t2l, (kc->u.narrow[2 * 22 + 1]) = (kc->u.narrow[2 * 22 + 1]) ^ t2h);
                            ( (kc->u.narrow[2 * 3 + 0]) = (kc->u.narrow[2 * 3 + 0]) ^ t3l, (kc->u.narrow[2 * 3 + 1]) = (kc->u.narrow[2 * 3 + 1]) ^ t3h);
                            ( (kc->u.narrow[2 * 8 + 0]) = (kc->u.narrow[2 * 8 + 0]) ^ t3l, (kc->u.narrow[2 * 8 + 1]) = (kc->u.narrow[2 * 8 + 1]) ^ t3h);
                            ( (kc->u.narrow[2 * 13 + 0]) = (kc->u.narrow[2 * 13 + 0]) ^ t3l, (kc->u.narrow[2 * 13 + 1]) = (kc->u.narrow[2 * 13 + 1]) ^ t3h);
                            ( (kc->u.narrow[2 * 18 + 0]) = (kc->u.narrow[2 * 18 + 0]) ^ t3l, (kc->u.narrow[2 * 18 + 1]) = (kc->u.narrow[2 * 18 + 1]) ^ t3h);
                            ( (kc->u.narrow[2 * 23 + 0]) = (kc->u.narrow[2 * 23 + 0]) ^ t3l, (kc->u.narrow[2 * 23 + 1]) = (kc->u.narrow[2 * 23 + 1]) ^ t3h);
                            ( (kc->u.narrow[2 * 4 + 0]) = (kc->u.narrow[2 * 4 + 0]) ^ t4l, (kc->u.narrow[2 * 4 + 1]) = (kc->u.narrow[2 * 4 + 1]) ^ t4h);
                            ( (kc->u.narrow[2 * 9 + 0]) = (kc->u.narrow[2 * 9 + 0]) ^ t4l, (kc->u.narrow[2 * 9 + 1]) = (kc->u.narrow[2 * 9 + 1]) ^ t4h);
                            ( (kc->u.narrow[2 * 14 + 0]) = (kc->u.narrow[2 * 14 + 0]) ^ t4l, (kc->u.narrow[2 * 14 + 1]) = (kc->u.narrow[2 * 14 + 1]) ^ t4h);
                            ( (kc->u.narrow[2 * 19 + 0]) = (kc->u.narrow[2 * 19 + 0]) ^ t4l, (kc->u.narrow[2 * 19 + 1]) = (kc->u.narrow[2 * 19 + 1]) ^ t4h);
                            ( (kc->u.narrow[2 * 24 + 0]) = (kc->u.narrow[2 * 24 + 0]) ^ t4l, (kc->u.narrow[2 * 24 + 1]) = (kc->u.narrow[2 * 24 + 1]) ^ t4h);
                        }
                        while ( 0 );
                        do
                        {
                            do
                            {
                                (kc->u.narrow[2 * 5 + 0]) = ( ( (kc->u.narrow[2 * 5 + 0]) << 18) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 5 + 0]) >> (32 - 18) );
                                (kc->u.narrow[2 * 5 + 1]) = ( ( (kc->u.narrow[2 * 5 + 1]) << 18) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 5 + 1]) >> (32 - 18) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 10 + 0]) << (2 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 10 + 0]) >> (33 - 2) );
                                (kc->u.narrow[2 * 10 + 0]) = ( ( (kc->u.narrow[2 * 10 + 1]) << 2) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 10 + 1]) >> (32 - 2) );
                                (kc->u.narrow[2 * 10 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 15 + 0]) << (21 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 15 + 0]) >> (33 - 21) );
                                (kc->u.narrow[2 * 15 + 0]) = ( ( (kc->u.narrow[2 * 15 + 1]) << 21) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 15 + 1]) >> (32 - 21) );
                                (kc->u.narrow[2 * 15 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 20 + 0]) = ( ( (kc->u.narrow[2 * 20 + 0]) << 9) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 20 + 0]) >> (32 - 9) );
                                (kc->u.narrow[2 * 20 + 1]) = ( ( (kc->u.narrow[2 * 20 + 1]) << 9) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 20 + 1]) >> (32 - 9) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = (kc->u.narrow[2 * 1 + 0]);
                                (kc->u.narrow[2 * 1 + 0]) = ( ( (kc->u.narrow[2 * 1 + 1]) << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 1 + 1]) >> 31);
                                (kc->u.narrow[2 * 1 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 6 + 0]) = ( ( (kc->u.narrow[2 * 6 + 0]) << 22) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 6 + 0]) >> (32 - 22) );
                                (kc->u.narrow[2 * 6 + 1]) = ( ( (kc->u.narrow[2 * 6 + 1]) << 22) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 6 + 1]) >> (32 - 22) );
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 11 + 0]) = ( ( (kc->u.narrow[2 * 11 + 0]) << 5) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 11 + 0]) >> (32 - 5) );
                                (kc->u.narrow[2 * 11 + 1]) = ( ( (kc->u.narrow[2 * 11 + 1]) << 5) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 11 + 1]) >> (32 - 5) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 16 + 0]) << (23 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 16 + 0]) >> (33 - 23) );
                                (kc->u.narrow[2 * 16 + 0]) = ( ( (kc->u.narrow[2 * 16 + 1]) << 23) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 16 + 1]) >> (32 - 23) );
                                (kc->u.narrow[2 * 16 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 21 + 0]) = ( ( (kc->u.narrow[2 * 21 + 0]) << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 21 + 0]) >> (32 - 1) );
                                (kc->u.narrow[2 * 21 + 1]) = ( ( (kc->u.narrow[2 * 21 + 1]) << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 21 + 1]) >> (32 - 1) );
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 2 + 0]) = ( ( (kc->u.narrow[2 * 2 + 0]) << 31) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 2 + 0]) >> (32 - 31) );
                                (kc->u.narrow[2 * 2 + 1]) = ( ( (kc->u.narrow[2 * 2 + 1]) << 31) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 2 + 1]) >> (32 - 31) );
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 7 + 0]) = ( ( (kc->u.narrow[2 * 7 + 0]) << 3) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 7 + 0]) >> (32 - 3) );
                                (kc->u.narrow[2 * 7 + 1]) = ( ( (kc->u.narrow[2 * 7 + 1]) << 3) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 7 + 1]) >> (32 - 3) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 12 + 0]) << (22 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 12 + 0]) >> (33 - 22) );
                                (kc->u.narrow[2 * 12 + 0]) = ( ( (kc->u.narrow[2 * 12 + 1]) << 22) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 12 + 1]) >> (32 - 22) );
                                (kc->u.narrow[2 * 12 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 17 + 0]) << (8 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 17 + 0]) >> (33 - 8) );
                                (kc->u.narrow[2 * 17 + 0]) = ( ( (kc->u.narrow[2 * 17 + 1]) << 8) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 17 + 1]) >> (32 - 8) );
                                (kc->u.narrow[2 * 17 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 22 + 0]) << (31 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 22 + 0]) >> (33 - 31) );
                                (kc->u.narrow[2 * 22 + 0]) = ( ( (kc->u.narrow[2 * 22 + 1]) << 31) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 22 + 1]) >> (32 - 31) );
                                (kc->u.narrow[2 * 22 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 3 + 0]) = ( ( (kc->u.narrow[2 * 3 + 0]) << 14) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 3 + 0]) >> (32 - 14) );
                                (kc->u.narrow[2 * 3 + 1]) = ( ( (kc->u.narrow[2 * 3 + 1]) << 14) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 3 + 1]) >> (32 - 14) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 8 + 0]) << (28 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 8 + 0]) >> (33 - 28) );
                                (kc->u.narrow[2 * 8 + 0]) = ( ( (kc->u.narrow[2 * 8 + 1]) << 28) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 8 + 1]) >> (32 - 28) );
                                (kc->u.narrow[2 * 8 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 13 + 0]) << (13 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 13 + 0]) >> (33 - 13) );
                                (kc->u.narrow[2 * 13 + 0]) = ( ( (kc->u.narrow[2 * 13 + 1]) << 13) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 13 + 1]) >> (32 - 13) );
                                (kc->u.narrow[2 * 13 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 18 + 0]) << (11 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 18 + 0]) >> (33 - 11) );
                                (kc->u.narrow[2 * 18 + 0]) = ( ( (kc->u.narrow[2 * 18 + 1]) << 11) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 18 + 1]) >> (32 - 11) );
                                (kc->u.narrow[2 * 18 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 23 + 0]) = ( ( (kc->u.narrow[2 * 23 + 0]) << 28) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 23 + 0]) >> (32 - 28) );
                                (kc->u.narrow[2 * 23 + 1]) = ( ( (kc->u.narrow[2 * 23 + 1]) << 28) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 23 + 1]) >> (32 - 28) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 4 + 0]) << (14 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 4 + 0]) >> (33 - 14) );
                                (kc->u.narrow[2 * 4 + 0]) = ( ( (kc->u.narrow[2 * 4 + 1]) << 14) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 4 + 1]) >> (32 - 14) );
                                (kc->u.narrow[2 * 4 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 9 + 0]) = ( ( (kc->u.narrow[2 * 9 + 0]) << 10) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 9 + 0]) >> (32 - 10) );
                                (kc->u.narrow[2 * 9 + 1]) = ( ( (kc->u.narrow[2 * 9 + 1]) << 10) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 9 + 1]) >> (32 - 10) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 14 + 0]) << (20 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 14 + 0]) >> (33 - 20) );
                                (kc->u.narrow[2 * 14 + 0]) = ( ( (kc->u.narrow[2 * 14 + 1]) << 20) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 14 + 1]) >> (32 - 20) );
                                (kc->u.narrow[2 * 14 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 19 + 0]) = ( ( (kc->u.narrow[2 * 19 + 0]) << 4) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 19 + 0]) >> (32 - 4) );
                                (kc->u.narrow[2 * 19 + 1]) = ( ( (kc->u.narrow[2 * 19 + 1]) << 4) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 19 + 1]) >> (32 - 4) );
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 24 + 0]) = ( ( (kc->u.narrow[2 * 24 + 0]) << 7) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 24 + 0]) >> (32 - 7) );
                                (kc->u.narrow[2 * 24 + 1]) = ( ( (kc->u.narrow[2 * 24 + 1]) << 7) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 24 + 1]) >> (32 - 7) );
                            }
                            while ( 0 );
                        }
                        while ( 0 );
                        do
                        {
                            sph_u64 c0l, c0h;
                            sph_u64 c1l, c1h;
                            sph_u64 c2l, c2h;
                            sph_u64 c3l, c3h;
                            sph_u64 c4l, c4h;
                            sph_u64 bnnl, bnnh;
                            (bnnl = ( (~(kc->u.narrow[2 * 12 + 0]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ), bnnh = ( (~(kc->u.narrow[2 * 12 + 1]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 6 + 0]) | (kc->u.narrow[2 * 12 + 0]), kth = (kc->u.narrow[2 * 6 + 1]) | (kc->u.narrow[2 * 12 + 1]) );
                                (c0l = (kc->u.narrow[2 * 0 + 0]) ^ ktl, c0h = (kc->u.narrow[2 * 0 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = bnnl | (kc->u.narrow[2 * 18 + 0]), kth = bnnh | (kc->u.narrow[2 * 18 + 1]) );
                                (c1l = (kc->u.narrow[2 * 6 + 0]) ^ ktl, c1h = (kc->u.narrow[2 * 6 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 18 + 0]) & (kc->u.narrow[2 * 24 + 0]), kth = (kc->u.narrow[2 * 18 + 1]) & (kc->u.narrow[2 * 24 + 1]) );
                                (c2l = (kc->u.narrow[2 * 12 + 0]) ^ ktl, c2h = (kc->u.narrow[2 * 12 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 24 + 0]) | (kc->u.narrow[2 * 0 + 0]), kth = (kc->u.narrow[2 * 24 + 1]) | (kc->u.narrow[2 * 0 + 1]) );
                                (c3l = (kc->u.narrow[2 * 18 + 0]) ^ ktl, c3h = (kc->u.narrow[2 * 18 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 0 + 0]) & (kc->u.narrow[2 * 6 + 0]), kth = (kc->u.narrow[2 * 0 + 1]) & (kc->u.narrow[2 * 6 + 1]) );
                                (c4l = (kc->u.narrow[2 * 24 + 0]) ^ ktl, c4h = (kc->u.narrow[2 * 24 + 1]) ^ kth);
                            }
                            while ( 0 );
                            ( (kc->u.narrow[2 * 0 + 0]) = c0l, (kc->u.narrow[2 * 0 + 1]) = c0h);
                            ( (kc->u.narrow[2 * 6 + 0]) = c1l, (kc->u.narrow[2 * 6 + 1]) = c1h);
                            ( (kc->u.narrow[2 * 12 + 0]) = c2l, (kc->u.narrow[2 * 12 + 1]) = c2h);
                            ( (kc->u.narrow[2 * 18 + 0]) = c3l, (kc->u.narrow[2 * 18 + 1]) = c3h);
                            ( (kc->u.narrow[2 * 24 + 0]) = c4l, (kc->u.narrow[2 * 24 + 1]) = c4h);
                            (bnnl = ( (~(kc->u.narrow[2 * 22 + 0]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ), bnnh = ( (~(kc->u.narrow[2 * 22 + 1]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 9 + 0]) | (kc->u.narrow[2 * 10 + 0]), kth = (kc->u.narrow[2 * 9 + 1]) | (kc->u.narrow[2 * 10 + 1]) );
                                (c0l = (kc->u.narrow[2 * 3 + 0]) ^ ktl, c0h = (kc->u.narrow[2 * 3 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 10 + 0]) & (kc->u.narrow[2 * 16 + 0]), kth = (kc->u.narrow[2 * 10 + 1]) & (kc->u.narrow[2 * 16 + 1]) );
                                (c1l = (kc->u.narrow[2 * 9 + 0]) ^ ktl, c1h = (kc->u.narrow[2 * 9 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 16 + 0]) | bnnl, kth = (kc->u.narrow[2 * 16 + 1]) | bnnh);
                                (c2l = (kc->u.narrow[2 * 10 + 0]) ^ ktl, c2h = (kc->u.narrow[2 * 10 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 22 + 0]) | (kc->u.narrow[2 * 3 + 0]), kth = (kc->u.narrow[2 * 22 + 1]) | (kc->u.narrow[2 * 3 + 1]) );
                                (c3l = (kc->u.narrow[2 * 16 + 0]) ^ ktl, c3h = (kc->u.narrow[2 * 16 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 3 + 0]) & (kc->u.narrow[2 * 9 + 0]), kth = (kc->u.narrow[2 * 3 + 1]) & (kc->u.narrow[2 * 9 + 1]) );
                                (c4l = (kc->u.narrow[2 * 22 + 0]) ^ ktl, c4h = (kc->u.narrow[2 * 22 + 1]) ^ kth);
                            }
                            while ( 0 );
                            ( (kc->u.narrow[2 * 3 + 0]) = c0l, (kc->u.narrow[2 * 3 + 1]) = c0h);
                            ( (kc->u.narrow[2 * 9 + 0]) = c1l, (kc->u.narrow[2 * 9 + 1]) = c1h);
                            ( (kc->u.narrow[2 * 10 + 0]) = c2l, (kc->u.narrow[2 * 10 + 1]) = c2h);
                            ( (kc->u.narrow[2 * 16 + 0]) = c3l, (kc->u.narrow[2 * 16 + 1]) = c3h);
                            ( (kc->u.narrow[2 * 22 + 0]) = c4l, (kc->u.narrow[2 * 22 + 1]) = c4h);
                            (bnnl = ( (~(kc->u.narrow[2 * 19 + 0]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ), bnnh = ( (~(kc->u.narrow[2 * 19 + 1]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 7 + 0]) | (kc->u.narrow[2 * 13 + 0]), kth = (kc->u.narrow[2 * 7 + 1]) | (kc->u.narrow[2 * 13 + 1]) );
                                (c0l = (kc->u.narrow[2 * 1 + 0]) ^ ktl, c0h = (kc->u.narrow[2 * 1 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 13 + 0]) & (kc->u.narrow[2 * 19 + 0]), kth = (kc->u.narrow[2 * 13 + 1]) & (kc->u.narrow[2 * 19 + 1]) );
                                (c1l = (kc->u.narrow[2 * 7 + 0]) ^ ktl, c1h = (kc->u.narrow[2 * 7 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = bnnl & (kc->u.narrow[2 * 20 + 0]), kth = bnnh & (kc->u.narrow[2 * 20 + 1]) );
                                (c2l = (kc->u.narrow[2 * 13 + 0]) ^ ktl, c2h = (kc->u.narrow[2 * 13 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 20 + 0]) | (kc->u.narrow[2 * 1 + 0]), kth = (kc->u.narrow[2 * 20 + 1]) | (kc->u.narrow[2 * 1 + 1]) );
                                (c3l = bnnl ^ ktl, c3h = bnnh ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 1 + 0]) & (kc->u.narrow[2 * 7 + 0]), kth = (kc->u.narrow[2 * 1 + 1]) & (kc->u.narrow[2 * 7 + 1]) );
                                (c4l = (kc->u.narrow[2 * 20 + 0]) ^ ktl, c4h = (kc->u.narrow[2 * 20 + 1]) ^ kth);
                            }
                            while ( 0 );
                            ( (kc->u.narrow[2 * 1 + 0]) = c0l, (kc->u.narrow[2 * 1 + 1]) = c0h);
                            ( (kc->u.narrow[2 * 7 + 0]) = c1l, (kc->u.narrow[2 * 7 + 1]) = c1h);
                            ( (kc->u.narrow[2 * 13 + 0]) = c2l, (kc->u.narrow[2 * 13 + 1]) = c2h);
                            ( (kc->u.narrow[2 * 19 + 0]) = c3l, (kc->u.narrow[2 * 19 + 1]) = c3h);
                            ( (kc->u.narrow[2 * 20 + 0]) = c4l, (kc->u.narrow[2 * 20 + 1]) = c4h);
                            (bnnl = ( (~(kc->u.narrow[2 * 17 + 0]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ), bnnh = ( (~(kc->u.narrow[2 * 17 + 1]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 5 + 0]) & (kc->u.narrow[2 * 11 + 0]), kth = (kc->u.narrow[2 * 5 + 1]) & (kc->u.narrow[2 * 11 + 1]) );
                                (c0l = (kc->u.narrow[2 * 4 + 0]) ^ ktl, c0h = (kc->u.narrow[2 * 4 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 11 + 0]) | (kc->u.narrow[2 * 17 + 0]), kth = (kc->u.narrow[2 * 11 + 1]) | (kc->u.narrow[2 * 17 + 1]) );
                                (c1l = (kc->u.narrow[2 * 5 + 0]) ^ ktl, c1h = (kc->u.narrow[2 * 5 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = bnnl | (kc->u.narrow[2 * 23 + 0]), kth = bnnh | (kc->u.narrow[2 * 23 + 1]) );
                                (c2l = (kc->u.narrow[2 * 11 + 0]) ^ ktl, c2h = (kc->u.narrow[2 * 11 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 23 + 0]) & (kc->u.narrow[2 * 4 + 0]), kth = (kc->u.narrow[2 * 23 + 1]) & (kc->u.narrow[2 * 4 + 1]) );
                                (c3l = bnnl ^ ktl, c3h = bnnh ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 4 + 0]) | (kc->u.narrow[2 * 5 + 0]), kth = (kc->u.narrow[2 * 4 + 1]) | (kc->u.narrow[2 * 5 + 1]) );
                                (c4l = (kc->u.narrow[2 * 23 + 0]) ^ ktl, c4h = (kc->u.narrow[2 * 23 + 1]) ^ kth);
                            }
                            while ( 0 );
                            ( (kc->u.narrow[2 * 4 + 0]) = c0l, (kc->u.narrow[2 * 4 + 1]) = c0h);
                            ( (kc->u.narrow[2 * 5 + 0]) = c1l, (kc->u.narrow[2 * 5 + 1]) = c1h);
                            ( (kc->u.narrow[2 * 11 + 0]) = c2l, (kc->u.narrow[2 * 11 + 1]) = c2h);
                            ( (kc->u.narrow[2 * 17 + 0]) = c3l, (kc->u.narrow[2 * 17 + 1]) = c3h);
                            ( (kc->u.narrow[2 * 23 + 0]) = c4l, (kc->u.narrow[2 * 23 + 1]) = c4h);
                            (bnnl = ( (~(kc->u.narrow[2 * 8 + 0]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ), bnnh = ( (~(kc->u.narrow[2 * 8 + 1]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = bnnl & (kc->u.narrow[2 * 14 + 0]), kth = bnnh & (kc->u.narrow[2 * 14 + 1]) );
                                (c0l = (kc->u.narrow[2 * 2 + 0]) ^ ktl, c0h = (kc->u.narrow[2 * 2 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 14 + 0]) | (kc->u.narrow[2 * 15 + 0]), kth = (kc->u.narrow[2 * 14 + 1]) | (kc->u.narrow[2 * 15 + 1]) );
                                (c1l = bnnl ^ ktl, c1h = bnnh ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 15 + 0]) & (kc->u.narrow[2 * 21 + 0]), kth = (kc->u.narrow[2 * 15 + 1]) & (kc->u.narrow[2 * 21 + 1]) );
                                (c2l = (kc->u.narrow[2 * 14 + 0]) ^ ktl, c2h = (kc->u.narrow[2 * 14 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 21 + 0]) | (kc->u.narrow[2 * 2 + 0]), kth = (kc->u.narrow[2 * 21 + 1]) | (kc->u.narrow[2 * 2 + 1]) );
                                (c3l = (kc->u.narrow[2 * 15 + 0]) ^ ktl, c3h = (kc->u.narrow[2 * 15 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 2 + 0]) & (kc->u.narrow[2 * 8 + 0]), kth = (kc->u.narrow[2 * 2 + 1]) & (kc->u.narrow[2 * 8 + 1]) );
                                (c4l = (kc->u.narrow[2 * 21 + 0]) ^ ktl, c4h = (kc->u.narrow[2 * 21 + 1]) ^ kth);
                            }
                            while ( 0 );
                            ( (kc->u.narrow[2 * 2 + 0]) = c0l, (kc->u.narrow[2 * 2 + 1]) = c0h);
                            ( (kc->u.narrow[2 * 8 + 0]) = c1l, (kc->u.narrow[2 * 8 + 1]) = c1h);
                            ( (kc->u.narrow[2 * 14 + 0]) = c2l, (kc->u.narrow[2 * 14 + 1]) = c2h);
                            ( (kc->u.narrow[2 * 15 + 0]) = c3l, (kc->u.narrow[2 * 15 + 1]) = c3h);
                            ( (kc->u.narrow[2 * 21 + 0]) = c4l, (kc->u.narrow[2 * 21 + 1]) = c4h);
                        }
                        while ( 0 );
                        ( (kc->u.narrow[2 * 0 + 0]) = (kc->u.narrow[2 * 0 + 0]) ^ RC[j + 0].low, (kc->u.narrow[2 * 0 + 1]) = (kc->u.narrow[2 * 0 + 1]) ^ RC[j + 0].high);
                    }
                    while ( 0 );
                    if(out)
                    {
                        printf( "\n第%d轮中间状态：\n",k++ );
                        print_hex(kc->u.narrow, 1600);
                    }
                    do
                    {
                        do
                        {
                            sph_u64 t0l, t0h;
                            sph_u64 t1l, t1h;
                            sph_u64 t2l, t2h;
                            sph_u64 t3l, t3h;
                            sph_u64 t4l, t4h;
                            do
                            {
                                sph_u64 tt0l, tt0h;
                                sph_u64 tt1l, tt1h;
                                sph_u64 tt2l, tt2h;
                                sph_u64 tt3l, tt3h;
                                (tt0l = (kc->u.narrow[2 * 6 + 0]) ^ (kc->u.narrow[2 * 9 + 0]), tt0h = (kc->u.narrow[2 * 6 + 1]) ^ (kc->u.narrow[2 * 9 + 1]) );
                                (tt1l = (kc->u.narrow[2 * 7 + 0]) ^ (kc->u.narrow[2 * 5 + 0]), tt1h = (kc->u.narrow[2 * 7 + 1]) ^ (kc->u.narrow[2 * 5 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 8 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 8 + 1]) );
                                (tt0l = tt0l ^ tt1l, tt0h = tt0h ^ tt1h);
                                do
                                {
                                    sph_u32 tmp;
                                    tmp = tt0l;
                                    tt0l = ( (tt0h << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | (tt0h >> 31);
                                    tt0h = tmp;
                                }
                                while ( 0 );
                                (tt2l = (kc->u.narrow[2 * 24 + 0]) ^ (kc->u.narrow[2 * 22 + 0]), tt2h = (kc->u.narrow[2 * 24 + 1]) ^ (kc->u.narrow[2 * 22 + 1]) );
                                (tt3l = (kc->u.narrow[2 * 20 + 0]) ^ (kc->u.narrow[2 * 23 + 0]), tt3h = (kc->u.narrow[2 * 20 + 1]) ^ (kc->u.narrow[2 * 23 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 21 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 21 + 1]) );
                                (tt2l = tt2l ^ tt3l, tt2h = tt2h ^ tt3h);
                                (t0l = tt0l ^ tt2l, t0h = tt0h ^ tt2h);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 tt0l, tt0h;
                                sph_u64 tt1l, tt1h;
                                sph_u64 tt2l, tt2h;
                                sph_u64 tt3l, tt3h;
                                (tt0l = (kc->u.narrow[2 * 12 + 0]) ^ (kc->u.narrow[2 * 10 + 0]), tt0h = (kc->u.narrow[2 * 12 + 1]) ^ (kc->u.narrow[2 * 10 + 1]) );
                                (tt1l = (kc->u.narrow[2 * 13 + 0]) ^ (kc->u.narrow[2 * 11 + 0]), tt1h = (kc->u.narrow[2 * 13 + 1]) ^ (kc->u.narrow[2 * 11 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 14 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 14 + 1]) );
                                (tt0l = tt0l ^ tt1l, tt0h = tt0h ^ tt1h);
                                do
                                {
                                    sph_u32 tmp;
                                    tmp = tt0l;
                                    tt0l = ( (tt0h << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | (tt0h >> 31);
                                    tt0h = tmp;
                                }
                                while ( 0 );
                                (tt2l = (kc->u.narrow[2 * 0 + 0]) ^ (kc->u.narrow[2 * 3 + 0]), tt2h = (kc->u.narrow[2 * 0 + 1]) ^ (kc->u.narrow[2 * 3 + 1]) );
                                (tt3l = (kc->u.narrow[2 * 1 + 0]) ^ (kc->u.narrow[2 * 4 + 0]), tt3h = (kc->u.narrow[2 * 1 + 1]) ^ (kc->u.narrow[2 * 4 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 2 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 2 + 1]) );
                                (tt2l = tt2l ^ tt3l, tt2h = tt2h ^ tt3h);
                                (t1l = tt0l ^ tt2l, t1h = tt0h ^ tt2h);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 tt0l, tt0h;
                                sph_u64 tt1l, tt1h;
                                sph_u64 tt2l, tt2h;
                                sph_u64 tt3l, tt3h;
                                (tt0l = (kc->u.narrow[2 * 18 + 0]) ^ (kc->u.narrow[2 * 16 + 0]), tt0h = (kc->u.narrow[2 * 18 + 1]) ^ (kc->u.narrow[2 * 16 + 1]) );
                                (tt1l = (kc->u.narrow[2 * 19 + 0]) ^ (kc->u.narrow[2 * 17 + 0]), tt1h = (kc->u.narrow[2 * 19 + 1]) ^ (kc->u.narrow[2 * 17 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 15 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 15 + 1]) );
                                (tt0l = tt0l ^ tt1l, tt0h = tt0h ^ tt1h);
                                do
                                {
                                    sph_u32 tmp;
                                    tmp = tt0l;
                                    tt0l = ( (tt0h << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | (tt0h >> 31);
                                    tt0h = tmp;
                                }
                                while ( 0 );
                                (tt2l = (kc->u.narrow[2 * 6 + 0]) ^ (kc->u.narrow[2 * 9 + 0]), tt2h = (kc->u.narrow[2 * 6 + 1]) ^ (kc->u.narrow[2 * 9 + 1]) );
                                (tt3l = (kc->u.narrow[2 * 7 + 0]) ^ (kc->u.narrow[2 * 5 + 0]), tt3h = (kc->u.narrow[2 * 7 + 1]) ^ (kc->u.narrow[2 * 5 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 8 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 8 + 1]) );
                                (tt2l = tt2l ^ tt3l, tt2h = tt2h ^ tt3h);
                                (t2l = tt0l ^ tt2l, t2h = tt0h ^ tt2h);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 tt0l, tt0h;
                                sph_u64 tt1l, tt1h;
                                sph_u64 tt2l, tt2h;
                                sph_u64 tt3l, tt3h;
                                (tt0l = (kc->u.narrow[2 * 24 + 0]) ^ (kc->u.narrow[2 * 22 + 0]), tt0h = (kc->u.narrow[2 * 24 + 1]) ^ (kc->u.narrow[2 * 22 + 1]) );
                                (tt1l = (kc->u.narrow[2 * 20 + 0]) ^ (kc->u.narrow[2 * 23 + 0]), tt1h = (kc->u.narrow[2 * 20 + 1]) ^ (kc->u.narrow[2 * 23 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 21 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 21 + 1]) );
                                (tt0l = tt0l ^ tt1l, tt0h = tt0h ^ tt1h);
                                do
                                {
                                    sph_u32 tmp;
                                    tmp = tt0l;
                                    tt0l = ( (tt0h << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | (tt0h >> 31);
                                    tt0h = tmp;
                                }
                                while ( 0 );
                                (tt2l = (kc->u.narrow[2 * 12 + 0]) ^ (kc->u.narrow[2 * 10 + 0]), tt2h = (kc->u.narrow[2 * 12 + 1]) ^ (kc->u.narrow[2 * 10 + 1]) );
                                (tt3l = (kc->u.narrow[2 * 13 + 0]) ^ (kc->u.narrow[2 * 11 + 0]), tt3h = (kc->u.narrow[2 * 13 + 1]) ^ (kc->u.narrow[2 * 11 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 14 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 14 + 1]) );
                                (tt2l = tt2l ^ tt3l, tt2h = tt2h ^ tt3h);
                                (t3l = tt0l ^ tt2l, t3h = tt0h ^ tt2h);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 tt0l, tt0h;
                                sph_u64 tt1l, tt1h;
                                sph_u64 tt2l, tt2h;
                                sph_u64 tt3l, tt3h;
                                (tt0l = (kc->u.narrow[2 * 0 + 0]) ^ (kc->u.narrow[2 * 3 + 0]), tt0h = (kc->u.narrow[2 * 0 + 1]) ^ (kc->u.narrow[2 * 3 + 1]) );
                                (tt1l = (kc->u.narrow[2 * 1 + 0]) ^ (kc->u.narrow[2 * 4 + 0]), tt1h = (kc->u.narrow[2 * 1 + 1]) ^ (kc->u.narrow[2 * 4 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 2 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 2 + 1]) );
                                (tt0l = tt0l ^ tt1l, tt0h = tt0h ^ tt1h);
                                do
                                {
                                    sph_u32 tmp;
                                    tmp = tt0l;
                                    tt0l = ( (tt0h << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | (tt0h >> 31);
                                    tt0h = tmp;
                                }
                                while ( 0 );
                                (tt2l = (kc->u.narrow[2 * 18 + 0]) ^ (kc->u.narrow[2 * 16 + 0]), tt2h = (kc->u.narrow[2 * 18 + 1]) ^ (kc->u.narrow[2 * 16 + 1]) );
                                (tt3l = (kc->u.narrow[2 * 19 + 0]) ^ (kc->u.narrow[2 * 17 + 0]), tt3h = (kc->u.narrow[2 * 19 + 1]) ^ (kc->u.narrow[2 * 17 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 15 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 15 + 1]) );
                                (tt2l = tt2l ^ tt3l, tt2h = tt2h ^ tt3h);
                                (t4l = tt0l ^ tt2l, t4h = tt0h ^ tt2h);
                            }
                            while ( 0 );
                            ( (kc->u.narrow[2 * 0 + 0]) = (kc->u.narrow[2 * 0 + 0]) ^ t0l, (kc->u.narrow[2 * 0 + 1]) = (kc->u.narrow[2 * 0 + 1]) ^ t0h);
                            ( (kc->u.narrow[2 * 3 + 0]) = (kc->u.narrow[2 * 3 + 0]) ^ t0l, (kc->u.narrow[2 * 3 + 1]) = (kc->u.narrow[2 * 3 + 1]) ^ t0h);
                            ( (kc->u.narrow[2 * 1 + 0]) = (kc->u.narrow[2 * 1 + 0]) ^ t0l, (kc->u.narrow[2 * 1 + 1]) = (kc->u.narrow[2 * 1 + 1]) ^ t0h);
                            ( (kc->u.narrow[2 * 4 + 0]) = (kc->u.narrow[2 * 4 + 0]) ^ t0l, (kc->u.narrow[2 * 4 + 1]) = (kc->u.narrow[2 * 4 + 1]) ^ t0h);
                            ( (kc->u.narrow[2 * 2 + 0]) = (kc->u.narrow[2 * 2 + 0]) ^ t0l, (kc->u.narrow[2 * 2 + 1]) = (kc->u.narrow[2 * 2 + 1]) ^ t0h);
                            ( (kc->u.narrow[2 * 6 + 0]) = (kc->u.narrow[2 * 6 + 0]) ^ t1l, (kc->u.narrow[2 * 6 + 1]) = (kc->u.narrow[2 * 6 + 1]) ^ t1h);
                            ( (kc->u.narrow[2 * 9 + 0]) = (kc->u.narrow[2 * 9 + 0]) ^ t1l, (kc->u.narrow[2 * 9 + 1]) = (kc->u.narrow[2 * 9 + 1]) ^ t1h);
                            ( (kc->u.narrow[2 * 7 + 0]) = (kc->u.narrow[2 * 7 + 0]) ^ t1l, (kc->u.narrow[2 * 7 + 1]) = (kc->u.narrow[2 * 7 + 1]) ^ t1h);
                            ( (kc->u.narrow[2 * 5 + 0]) = (kc->u.narrow[2 * 5 + 0]) ^ t1l, (kc->u.narrow[2 * 5 + 1]) = (kc->u.narrow[2 * 5 + 1]) ^ t1h);
                            ( (kc->u.narrow[2 * 8 + 0]) = (kc->u.narrow[2 * 8 + 0]) ^ t1l, (kc->u.narrow[2 * 8 + 1]) = (kc->u.narrow[2 * 8 + 1]) ^ t1h);
                            ( (kc->u.narrow[2 * 12 + 0]) = (kc->u.narrow[2 * 12 + 0]) ^ t2l, (kc->u.narrow[2 * 12 + 1]) = (kc->u.narrow[2 * 12 + 1]) ^ t2h);
                            ( (kc->u.narrow[2 * 10 + 0]) = (kc->u.narrow[2 * 10 + 0]) ^ t2l, (kc->u.narrow[2 * 10 + 1]) = (kc->u.narrow[2 * 10 + 1]) ^ t2h);
                            ( (kc->u.narrow[2 * 13 + 0]) = (kc->u.narrow[2 * 13 + 0]) ^ t2l, (kc->u.narrow[2 * 13 + 1]) = (kc->u.narrow[2 * 13 + 1]) ^ t2h);
                            ( (kc->u.narrow[2 * 11 + 0]) = (kc->u.narrow[2 * 11 + 0]) ^ t2l, (kc->u.narrow[2 * 11 + 1]) = (kc->u.narrow[2 * 11 + 1]) ^ t2h);
                            ( (kc->u.narrow[2 * 14 + 0]) = (kc->u.narrow[2 * 14 + 0]) ^ t2l, (kc->u.narrow[2 * 14 + 1]) = (kc->u.narrow[2 * 14 + 1]) ^ t2h);
                            ( (kc->u.narrow[2 * 18 + 0]) = (kc->u.narrow[2 * 18 + 0]) ^ t3l, (kc->u.narrow[2 * 18 + 1]) = (kc->u.narrow[2 * 18 + 1]) ^ t3h);
                            ( (kc->u.narrow[2 * 16 + 0]) = (kc->u.narrow[2 * 16 + 0]) ^ t3l, (kc->u.narrow[2 * 16 + 1]) = (kc->u.narrow[2 * 16 + 1]) ^ t3h);
                            ( (kc->u.narrow[2 * 19 + 0]) = (kc->u.narrow[2 * 19 + 0]) ^ t3l, (kc->u.narrow[2 * 19 + 1]) = (kc->u.narrow[2 * 19 + 1]) ^ t3h);
                            ( (kc->u.narrow[2 * 17 + 0]) = (kc->u.narrow[2 * 17 + 0]) ^ t3l, (kc->u.narrow[2 * 17 + 1]) = (kc->u.narrow[2 * 17 + 1]) ^ t3h);
                            ( (kc->u.narrow[2 * 15 + 0]) = (kc->u.narrow[2 * 15 + 0]) ^ t3l, (kc->u.narrow[2 * 15 + 1]) = (kc->u.narrow[2 * 15 + 1]) ^ t3h);
                            ( (kc->u.narrow[2 * 24 + 0]) = (kc->u.narrow[2 * 24 + 0]) ^ t4l, (kc->u.narrow[2 * 24 + 1]) = (kc->u.narrow[2 * 24 + 1]) ^ t4h);
                            ( (kc->u.narrow[2 * 22 + 0]) = (kc->u.narrow[2 * 22 + 0]) ^ t4l, (kc->u.narrow[2 * 22 + 1]) = (kc->u.narrow[2 * 22 + 1]) ^ t4h);
                            ( (kc->u.narrow[2 * 20 + 0]) = (kc->u.narrow[2 * 20 + 0]) ^ t4l, (kc->u.narrow[2 * 20 + 1]) = (kc->u.narrow[2 * 20 + 1]) ^ t4h);
                            ( (kc->u.narrow[2 * 23 + 0]) = (kc->u.narrow[2 * 23 + 0]) ^ t4l, (kc->u.narrow[2 * 23 + 1]) = (kc->u.narrow[2 * 23 + 1]) ^ t4h);
                            ( (kc->u.narrow[2 * 21 + 0]) = (kc->u.narrow[2 * 21 + 0]) ^ t4l, (kc->u.narrow[2 * 21 + 1]) = (kc->u.narrow[2 * 21 + 1]) ^ t4h);
                        }
                        while ( 0 );
                        do
                        {
                            do
                            {
                                (kc->u.narrow[2 * 3 + 0]) = ( ( (kc->u.narrow[2 * 3 + 0]) << 18) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 3 + 0]) >> (32 - 18) );
                                (kc->u.narrow[2 * 3 + 1]) = ( ( (kc->u.narrow[2 * 3 + 1]) << 18) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 3 + 1]) >> (32 - 18) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 1 + 0]) << (2 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 1 + 0]) >> (33 - 2) );
                                (kc->u.narrow[2 * 1 + 0]) = ( ( (kc->u.narrow[2 * 1 + 1]) << 2) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 1 + 1]) >> (32 - 2) );
                                (kc->u.narrow[2 * 1 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 4 + 0]) << (21 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 4 + 0]) >> (33 - 21) );
                                (kc->u.narrow[2 * 4 + 0]) = ( ( (kc->u.narrow[2 * 4 + 1]) << 21) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 4 + 1]) >> (32 - 21) );
                                (kc->u.narrow[2 * 4 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 2 + 0]) = ( ( (kc->u.narrow[2 * 2 + 0]) << 9) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 2 + 0]) >> (32 - 9) );
                                (kc->u.narrow[2 * 2 + 1]) = ( ( (kc->u.narrow[2 * 2 + 1]) << 9) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 2 + 1]) >> (32 - 9) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = (kc->u.narrow[2 * 6 + 0]);
                                (kc->u.narrow[2 * 6 + 0]) = ( ( (kc->u.narrow[2 * 6 + 1]) << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 6 + 1]) >> 31);
                                (kc->u.narrow[2 * 6 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 9 + 0]) = ( ( (kc->u.narrow[2 * 9 + 0]) << 22) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 9 + 0]) >> (32 - 22) );
                                (kc->u.narrow[2 * 9 + 1]) = ( ( (kc->u.narrow[2 * 9 + 1]) << 22) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 9 + 1]) >> (32 - 22) );
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 7 + 0]) = ( ( (kc->u.narrow[2 * 7 + 0]) << 5) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 7 + 0]) >> (32 - 5) );
                                (kc->u.narrow[2 * 7 + 1]) = ( ( (kc->u.narrow[2 * 7 + 1]) << 5) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 7 + 1]) >> (32 - 5) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 5 + 0]) << (23 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 5 + 0]) >> (33 - 23) );
                                (kc->u.narrow[2 * 5 + 0]) = ( ( (kc->u.narrow[2 * 5 + 1]) << 23) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 5 + 1]) >> (32 - 23) );
                                (kc->u.narrow[2 * 5 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 8 + 0]) = ( ( (kc->u.narrow[2 * 8 + 0]) << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 8 + 0]) >> (32 - 1) );
                                (kc->u.narrow[2 * 8 + 1]) = ( ( (kc->u.narrow[2 * 8 + 1]) << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 8 + 1]) >> (32 - 1) );
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 12 + 0]) = ( ( (kc->u.narrow[2 * 12 + 0]) << 31) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 12 + 0]) >> (32 - 31) );
                                (kc->u.narrow[2 * 12 + 1]) = ( ( (kc->u.narrow[2 * 12 + 1]) << 31) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 12 + 1]) >> (32 - 31) );
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 10 + 0]) = ( ( (kc->u.narrow[2 * 10 + 0]) << 3) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 10 + 0]) >> (32 - 3) );
                                (kc->u.narrow[2 * 10 + 1]) = ( ( (kc->u.narrow[2 * 10 + 1]) << 3) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 10 + 1]) >> (32 - 3) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 13 + 0]) << (22 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 13 + 0]) >> (33 - 22) );
                                (kc->u.narrow[2 * 13 + 0]) = ( ( (kc->u.narrow[2 * 13 + 1]) << 22) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 13 + 1]) >> (32 - 22) );
                                (kc->u.narrow[2 * 13 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 11 + 0]) << (8 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 11 + 0]) >> (33 - 8) );
                                (kc->u.narrow[2 * 11 + 0]) = ( ( (kc->u.narrow[2 * 11 + 1]) << 8) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 11 + 1]) >> (32 - 8) );
                                (kc->u.narrow[2 * 11 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 14 + 0]) << (31 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 14 + 0]) >> (33 - 31) );
                                (kc->u.narrow[2 * 14 + 0]) = ( ( (kc->u.narrow[2 * 14 + 1]) << 31) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 14 + 1]) >> (32 - 31) );
                                (kc->u.narrow[2 * 14 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 18 + 0]) = ( ( (kc->u.narrow[2 * 18 + 0]) << 14) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 18 + 0]) >> (32 - 14) );
                                (kc->u.narrow[2 * 18 + 1]) = ( ( (kc->u.narrow[2 * 18 + 1]) << 14) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 18 + 1]) >> (32 - 14) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 16 + 0]) << (28 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 16 + 0]) >> (33 - 28) );
                                (kc->u.narrow[2 * 16 + 0]) = ( ( (kc->u.narrow[2 * 16 + 1]) << 28) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 16 + 1]) >> (32 - 28) );
                                (kc->u.narrow[2 * 16 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 19 + 0]) << (13 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 19 + 0]) >> (33 - 13) );
                                (kc->u.narrow[2 * 19 + 0]) = ( ( (kc->u.narrow[2 * 19 + 1]) << 13) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 19 + 1]) >> (32 - 13) );
                                (kc->u.narrow[2 * 19 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 17 + 0]) << (11 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 17 + 0]) >> (33 - 11) );
                                (kc->u.narrow[2 * 17 + 0]) = ( ( (kc->u.narrow[2 * 17 + 1]) << 11) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 17 + 1]) >> (32 - 11) );
                                (kc->u.narrow[2 * 17 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 15 + 0]) = ( ( (kc->u.narrow[2 * 15 + 0]) << 28) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 15 + 0]) >> (32 - 28) );
                                (kc->u.narrow[2 * 15 + 1]) = ( ( (kc->u.narrow[2 * 15 + 1]) << 28) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 15 + 1]) >> (32 - 28) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 24 + 0]) << (14 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 24 + 0]) >> (33 - 14) );
                                (kc->u.narrow[2 * 24 + 0]) = ( ( (kc->u.narrow[2 * 24 + 1]) << 14) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 24 + 1]) >> (32 - 14) );
                                (kc->u.narrow[2 * 24 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 22 + 0]) = ( ( (kc->u.narrow[2 * 22 + 0]) << 10) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 22 + 0]) >> (32 - 10) );
                                (kc->u.narrow[2 * 22 + 1]) = ( ( (kc->u.narrow[2 * 22 + 1]) << 10) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 22 + 1]) >> (32 - 10) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 20 + 0]) << (20 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 20 + 0]) >> (33 - 20) );
                                (kc->u.narrow[2 * 20 + 0]) = ( ( (kc->u.narrow[2 * 20 + 1]) << 20) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 20 + 1]) >> (32 - 20) );
                                (kc->u.narrow[2 * 20 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 23 + 0]) = ( ( (kc->u.narrow[2 * 23 + 0]) << 4) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 23 + 0]) >> (32 - 4) );
                                (kc->u.narrow[2 * 23 + 1]) = ( ( (kc->u.narrow[2 * 23 + 1]) << 4) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 23 + 1]) >> (32 - 4) );
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 21 + 0]) = ( ( (kc->u.narrow[2 * 21 + 0]) << 7) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 21 + 0]) >> (32 - 7) );
                                (kc->u.narrow[2 * 21 + 1]) = ( ( (kc->u.narrow[2 * 21 + 1]) << 7) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 21 + 1]) >> (32 - 7) );
                            }
                            while ( 0 );
                        }
                        while ( 0 );
                        do
                        {
                            sph_u64 c0l, c0h;
                            sph_u64 c1l, c1h;
                            sph_u64 c2l, c2h;
                            sph_u64 c3l, c3h;
                            sph_u64 c4l, c4h;
                            sph_u64 bnnl, bnnh;
                            (bnnl = ( (~(kc->u.narrow[2 * 13 + 0]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ), bnnh = ( (~(kc->u.narrow[2 * 13 + 1]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 9 + 0]) | (kc->u.narrow[2 * 13 + 0]), kth = (kc->u.narrow[2 * 9 + 1]) | (kc->u.narrow[2 * 13 + 1]) );
                                (c0l = (kc->u.narrow[2 * 0 + 0]) ^ ktl, c0h = (kc->u.narrow[2 * 0 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = bnnl | (kc->u.narrow[2 * 17 + 0]), kth = bnnh | (kc->u.narrow[2 * 17 + 1]) );
                                (c1l = (kc->u.narrow[2 * 9 + 0]) ^ ktl, c1h = (kc->u.narrow[2 * 9 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 17 + 0]) & (kc->u.narrow[2 * 21 + 0]), kth = (kc->u.narrow[2 * 17 + 1]) & (kc->u.narrow[2 * 21 + 1]) );
                                (c2l = (kc->u.narrow[2 * 13 + 0]) ^ ktl, c2h = (kc->u.narrow[2 * 13 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 21 + 0]) | (kc->u.narrow[2 * 0 + 0]), kth = (kc->u.narrow[2 * 21 + 1]) | (kc->u.narrow[2 * 0 + 1]) );
                                (c3l = (kc->u.narrow[2 * 17 + 0]) ^ ktl, c3h = (kc->u.narrow[2 * 17 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 0 + 0]) & (kc->u.narrow[2 * 9 + 0]), kth = (kc->u.narrow[2 * 0 + 1]) & (kc->u.narrow[2 * 9 + 1]) );
                                (c4l = (kc->u.narrow[2 * 21 + 0]) ^ ktl, c4h = (kc->u.narrow[2 * 21 + 1]) ^ kth);
                            }
                            while ( 0 );
                            ( (kc->u.narrow[2 * 0 + 0]) = c0l, (kc->u.narrow[2 * 0 + 1]) = c0h);
                            ( (kc->u.narrow[2 * 9 + 0]) = c1l, (kc->u.narrow[2 * 9 + 1]) = c1h);
                            ( (kc->u.narrow[2 * 13 + 0]) = c2l, (kc->u.narrow[2 * 13 + 1]) = c2h);
                            ( (kc->u.narrow[2 * 17 + 0]) = c3l, (kc->u.narrow[2 * 17 + 1]) = c3h);
                            ( (kc->u.narrow[2 * 21 + 0]) = c4l, (kc->u.narrow[2 * 21 + 1]) = c4h);
                            (bnnl = ( (~(kc->u.narrow[2 * 14 + 0]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ), bnnh = ( (~(kc->u.narrow[2 * 14 + 1]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 22 + 0]) | (kc->u.narrow[2 * 1 + 0]), kth = (kc->u.narrow[2 * 22 + 1]) | (kc->u.narrow[2 * 1 + 1]) );
                                (c0l = (kc->u.narrow[2 * 18 + 0]) ^ ktl, c0h = (kc->u.narrow[2 * 18 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 1 + 0]) & (kc->u.narrow[2 * 5 + 0]), kth = (kc->u.narrow[2 * 1 + 1]) & (kc->u.narrow[2 * 5 + 1]) );
                                (c1l = (kc->u.narrow[2 * 22 + 0]) ^ ktl, c1h = (kc->u.narrow[2 * 22 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 5 + 0]) | bnnl, kth = (kc->u.narrow[2 * 5 + 1]) | bnnh);
                                (c2l = (kc->u.narrow[2 * 1 + 0]) ^ ktl, c2h = (kc->u.narrow[2 * 1 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 14 + 0]) | (kc->u.narrow[2 * 18 + 0]), kth = (kc->u.narrow[2 * 14 + 1]) | (kc->u.narrow[2 * 18 + 1]) );
                                (c3l = (kc->u.narrow[2 * 5 + 0]) ^ ktl, c3h = (kc->u.narrow[2 * 5 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 18 + 0]) & (kc->u.narrow[2 * 22 + 0]), kth = (kc->u.narrow[2 * 18 + 1]) & (kc->u.narrow[2 * 22 + 1]) );
                                (c4l = (kc->u.narrow[2 * 14 + 0]) ^ ktl, c4h = (kc->u.narrow[2 * 14 + 1]) ^ kth);
                            }
                            while ( 0 );
                            ( (kc->u.narrow[2 * 18 + 0]) = c0l, (kc->u.narrow[2 * 18 + 1]) = c0h);
                            ( (kc->u.narrow[2 * 22 + 0]) = c1l, (kc->u.narrow[2 * 22 + 1]) = c1h);
                            ( (kc->u.narrow[2 * 1 + 0]) = c2l, (kc->u.narrow[2 * 1 + 1]) = c2h);
                            ( (kc->u.narrow[2 * 5 + 0]) = c3l, (kc->u.narrow[2 * 5 + 1]) = c3h);
                            ( (kc->u.narrow[2 * 14 + 0]) = c4l, (kc->u.narrow[2 * 14 + 1]) = c4h);
                            (bnnl = ( (~(kc->u.narrow[2 * 23 + 0]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ), bnnh = ( (~(kc->u.narrow[2 * 23 + 1]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 10 + 0]) | (kc->u.narrow[2 * 19 + 0]), kth = (kc->u.narrow[2 * 10 + 1]) | (kc->u.narrow[2 * 19 + 1]) );
                                (c0l = (kc->u.narrow[2 * 6 + 0]) ^ ktl, c0h = (kc->u.narrow[2 * 6 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 19 + 0]) & (kc->u.narrow[2 * 23 + 0]), kth = (kc->u.narrow[2 * 19 + 1]) & (kc->u.narrow[2 * 23 + 1]) );
                                (c1l = (kc->u.narrow[2 * 10 + 0]) ^ ktl, c1h = (kc->u.narrow[2 * 10 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = bnnl & (kc->u.narrow[2 * 2 + 0]), kth = bnnh & (kc->u.narrow[2 * 2 + 1]) );
                                (c2l = (kc->u.narrow[2 * 19 + 0]) ^ ktl, c2h = (kc->u.narrow[2 * 19 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 2 + 0]) | (kc->u.narrow[2 * 6 + 0]), kth = (kc->u.narrow[2 * 2 + 1]) | (kc->u.narrow[2 * 6 + 1]) );
                                (c3l = bnnl ^ ktl, c3h = bnnh ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 6 + 0]) & (kc->u.narrow[2 * 10 + 0]), kth = (kc->u.narrow[2 * 6 + 1]) & (kc->u.narrow[2 * 10 + 1]) );
                                (c4l = (kc->u.narrow[2 * 2 + 0]) ^ ktl, c4h = (kc->u.narrow[2 * 2 + 1]) ^ kth);
                            }
                            while ( 0 );
                            ( (kc->u.narrow[2 * 6 + 0]) = c0l, (kc->u.narrow[2 * 6 + 1]) = c0h);
                            ( (kc->u.narrow[2 * 10 + 0]) = c1l, (kc->u.narrow[2 * 10 + 1]) = c1h);
                            ( (kc->u.narrow[2 * 19 + 0]) = c2l, (kc->u.narrow[2 * 19 + 1]) = c2h);
                            ( (kc->u.narrow[2 * 23 + 0]) = c3l, (kc->u.narrow[2 * 23 + 1]) = c3h);
                            ( (kc->u.narrow[2 * 2 + 0]) = c4l, (kc->u.narrow[2 * 2 + 1]) = c4h);
                            (bnnl = ( (~(kc->u.narrow[2 * 11 + 0]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ), bnnh = ( (~(kc->u.narrow[2 * 11 + 1]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 3 + 0]) & (kc->u.narrow[2 * 7 + 0]), kth = (kc->u.narrow[2 * 3 + 1]) & (kc->u.narrow[2 * 7 + 1]) );
                                (c0l = (kc->u.narrow[2 * 24 + 0]) ^ ktl, c0h = (kc->u.narrow[2 * 24 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 7 + 0]) | (kc->u.narrow[2 * 11 + 0]), kth = (kc->u.narrow[2 * 7 + 1]) | (kc->u.narrow[2 * 11 + 1]) );
                                (c1l = (kc->u.narrow[2 * 3 + 0]) ^ ktl, c1h = (kc->u.narrow[2 * 3 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = bnnl | (kc->u.narrow[2 * 15 + 0]), kth = bnnh | (kc->u.narrow[2 * 15 + 1]) );
                                (c2l = (kc->u.narrow[2 * 7 + 0]) ^ ktl, c2h = (kc->u.narrow[2 * 7 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 15 + 0]) & (kc->u.narrow[2 * 24 + 0]), kth = (kc->u.narrow[2 * 15 + 1]) & (kc->u.narrow[2 * 24 + 1]) );
                                (c3l = bnnl ^ ktl, c3h = bnnh ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 24 + 0]) | (kc->u.narrow[2 * 3 + 0]), kth = (kc->u.narrow[2 * 24 + 1]) | (kc->u.narrow[2 * 3 + 1]) );
                                (c4l = (kc->u.narrow[2 * 15 + 0]) ^ ktl, c4h = (kc->u.narrow[2 * 15 + 1]) ^ kth);
                            }
                            while ( 0 );
                            ( (kc->u.narrow[2 * 24 + 0]) = c0l, (kc->u.narrow[2 * 24 + 1]) = c0h);
                            ( (kc->u.narrow[2 * 3 + 0]) = c1l, (kc->u.narrow[2 * 3 + 1]) = c1h);
                            ( (kc->u.narrow[2 * 7 + 0]) = c2l, (kc->u.narrow[2 * 7 + 1]) = c2h);
                            ( (kc->u.narrow[2 * 11 + 0]) = c3l, (kc->u.narrow[2 * 11 + 1]) = c3h);
                            ( (kc->u.narrow[2 * 15 + 0]) = c4l, (kc->u.narrow[2 * 15 + 1]) = c4h);
                            (bnnl = ( (~(kc->u.narrow[2 * 16 + 0]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ), bnnh = ( (~(kc->u.narrow[2 * 16 + 1]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = bnnl & (kc->u.narrow[2 * 20 + 0]), kth = bnnh & (kc->u.narrow[2 * 20 + 1]) );
                                (c0l = (kc->u.narrow[2 * 12 + 0]) ^ ktl, c0h = (kc->u.narrow[2 * 12 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 20 + 0]) | (kc->u.narrow[2 * 4 + 0]), kth = (kc->u.narrow[2 * 20 + 1]) | (kc->u.narrow[2 * 4 + 1]) );
                                (c1l = bnnl ^ ktl, c1h = bnnh ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 4 + 0]) & (kc->u.narrow[2 * 8 + 0]), kth = (kc->u.narrow[2 * 4 + 1]) & (kc->u.narrow[2 * 8 + 1]) );
                                (c2l = (kc->u.narrow[2 * 20 + 0]) ^ ktl, c2h = (kc->u.narrow[2 * 20 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 8 + 0]) | (kc->u.narrow[2 * 12 + 0]), kth = (kc->u.narrow[2 * 8 + 1]) | (kc->u.narrow[2 * 12 + 1]) );
                                (c3l = (kc->u.narrow[2 * 4 + 0]) ^ ktl, c3h = (kc->u.narrow[2 * 4 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 12 + 0]) & (kc->u.narrow[2 * 16 + 0]), kth = (kc->u.narrow[2 * 12 + 1]) & (kc->u.narrow[2 * 16 + 1]) );
                                (c4l = (kc->u.narrow[2 * 8 + 0]) ^ ktl, c4h = (kc->u.narrow[2 * 8 + 1]) ^ kth);
                            }
                            while ( 0 );
                            ( (kc->u.narrow[2 * 12 + 0]) = c0l, (kc->u.narrow[2 * 12 + 1]) = c0h);
                            ( (kc->u.narrow[2 * 16 + 0]) = c1l, (kc->u.narrow[2 * 16 + 1]) = c1h);
                            ( (kc->u.narrow[2 * 20 + 0]) = c2l, (kc->u.narrow[2 * 20 + 1]) = c2h);
                            ( (kc->u.narrow[2 * 4 + 0]) = c3l, (kc->u.narrow[2 * 4 + 1]) = c3h);
                            ( (kc->u.narrow[2 * 8 + 0]) = c4l, (kc->u.narrow[2 * 8 + 1]) = c4h);
                        }
                        while ( 0 );
                        ( (kc->u.narrow[2 * 0 + 0]) = (kc->u.narrow[2 * 0 + 0]) ^ RC[j + 1].low, (kc->u.narrow[2 * 0 + 1]) = (kc->u.narrow[2 * 0 + 1]) ^ RC[j + 1].high);
                    }
                    while ( 0 );
                    if(out)
                    {
                        printf( "\n第%d轮中间状态：\n",k++ );
                        print_hex(kc->u.narrow, 1600);
                    }
                    do
                    {
                        do
                        {
                            sph_u64 t0l, t0h;
                            sph_u64 t1l, t1h;
                            sph_u64 t2l, t2h;
                            sph_u64 t3l, t3h;
                            sph_u64 t4l, t4h;
                            do
                            {
                                sph_u64 tt0l, tt0h;
                                sph_u64 tt1l, tt1h;
                                sph_u64 tt2l, tt2h;
                                sph_u64 tt3l, tt3h;
                                (tt0l = (kc->u.narrow[2 * 9 + 0]) ^ (kc->u.narrow[2 * 22 + 0]), tt0h = (kc->u.narrow[2 * 9 + 1]) ^ (kc->u.narrow[2 * 22 + 1]) );
                                (tt1l = (kc->u.narrow[2 * 10 + 0]) ^ (kc->u.narrow[2 * 3 + 0]), tt1h = (kc->u.narrow[2 * 10 + 1]) ^ (kc->u.narrow[2 * 3 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 16 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 16 + 1]) );
                                (tt0l = tt0l ^ tt1l, tt0h = tt0h ^ tt1h);
                                do
                                {
                                    sph_u32 tmp;
                                    tmp = tt0l;
                                    tt0l = ( (tt0h << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | (tt0h >> 31);
                                    tt0h = tmp;
                                }
                                while ( 0 );
                                (tt2l = (kc->u.narrow[2 * 21 + 0]) ^ (kc->u.narrow[2 * 14 + 0]), tt2h = (kc->u.narrow[2 * 21 + 1]) ^ (kc->u.narrow[2 * 14 + 1]) );
                                (tt3l = (kc->u.narrow[2 * 2 + 0]) ^ (kc->u.narrow[2 * 15 + 0]), tt3h = (kc->u.narrow[2 * 2 + 1]) ^ (kc->u.narrow[2 * 15 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 8 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 8 + 1]) );
                                (tt2l = tt2l ^ tt3l, tt2h = tt2h ^ tt3h);
                                (t0l = tt0l ^ tt2l, t0h = tt0h ^ tt2h);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 tt0l, tt0h;
                                sph_u64 tt1l, tt1h;
                                sph_u64 tt2l, tt2h;
                                sph_u64 tt3l, tt3h;
                                (tt0l = (kc->u.narrow[2 * 13 + 0]) ^ (kc->u.narrow[2 * 1 + 0]), tt0h = (kc->u.narrow[2 * 13 + 1]) ^ (kc->u.narrow[2 * 1 + 1]) );
                                (tt1l = (kc->u.narrow[2 * 19 + 0]) ^ (kc->u.narrow[2 * 7 + 0]), tt1h = (kc->u.narrow[2 * 19 + 1]) ^ (kc->u.narrow[2 * 7 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 20 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 20 + 1]) );
                                (tt0l = tt0l ^ tt1l, tt0h = tt0h ^ tt1h);
                                do
                                {
                                    sph_u32 tmp;
                                    tmp = tt0l;
                                    tt0l = ( (tt0h << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | (tt0h >> 31);
                                    tt0h = tmp;
                                }
                                while ( 0 );
                                (tt2l = (kc->u.narrow[2 * 0 + 0]) ^ (kc->u.narrow[2 * 18 + 0]), tt2h = (kc->u.narrow[2 * 0 + 1]) ^ (kc->u.narrow[2 * 18 + 1]) );
                                (tt3l = (kc->u.narrow[2 * 6 + 0]) ^ (kc->u.narrow[2 * 24 + 0]), tt3h = (kc->u.narrow[2 * 6 + 1]) ^ (kc->u.narrow[2 * 24 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 12 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 12 + 1]) );
                                (tt2l = tt2l ^ tt3l, tt2h = tt2h ^ tt3h);
                                (t1l = tt0l ^ tt2l, t1h = tt0h ^ tt2h);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 tt0l, tt0h;
                                sph_u64 tt1l, tt1h;
                                sph_u64 tt2l, tt2h;
                                sph_u64 tt3l, tt3h;
                                (tt0l = (kc->u.narrow[2 * 17 + 0]) ^ (kc->u.narrow[2 * 5 + 0]), tt0h = (kc->u.narrow[2 * 17 + 1]) ^ (kc->u.narrow[2 * 5 + 1]) );
                                (tt1l = (kc->u.narrow[2 * 23 + 0]) ^ (kc->u.narrow[2 * 11 + 0]), tt1h = (kc->u.narrow[2 * 23 + 1]) ^ (kc->u.narrow[2 * 11 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 4 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 4 + 1]) );
                                (tt0l = tt0l ^ tt1l, tt0h = tt0h ^ tt1h);
                                do
                                {
                                    sph_u32 tmp;
                                    tmp = tt0l;
                                    tt0l = ( (tt0h << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | (tt0h >> 31);
                                    tt0h = tmp;
                                }
                                while ( 0 );
                                (tt2l = (kc->u.narrow[2 * 9 + 0]) ^ (kc->u.narrow[2 * 22 + 0]), tt2h = (kc->u.narrow[2 * 9 + 1]) ^ (kc->u.narrow[2 * 22 + 1]) );
                                (tt3l = (kc->u.narrow[2 * 10 + 0]) ^ (kc->u.narrow[2 * 3 + 0]), tt3h = (kc->u.narrow[2 * 10 + 1]) ^ (kc->u.narrow[2 * 3 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 16 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 16 + 1]) );
                                (tt2l = tt2l ^ tt3l, tt2h = tt2h ^ tt3h);
                                (t2l = tt0l ^ tt2l, t2h = tt0h ^ tt2h);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 tt0l, tt0h;
                                sph_u64 tt1l, tt1h;
                                sph_u64 tt2l, tt2h;
                                sph_u64 tt3l, tt3h;
                                (tt0l = (kc->u.narrow[2 * 21 + 0]) ^ (kc->u.narrow[2 * 14 + 0]), tt0h = (kc->u.narrow[2 * 21 + 1]) ^ (kc->u.narrow[2 * 14 + 1]) );
                                (tt1l = (kc->u.narrow[2 * 2 + 0]) ^ (kc->u.narrow[2 * 15 + 0]), tt1h = (kc->u.narrow[2 * 2 + 1]) ^ (kc->u.narrow[2 * 15 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 8 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 8 + 1]) );
                                (tt0l = tt0l ^ tt1l, tt0h = tt0h ^ tt1h);
                                do
                                {
                                    sph_u32 tmp;
                                    tmp = tt0l;
                                    tt0l = ( (tt0h << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | (tt0h >> 31);
                                    tt0h = tmp;
                                }
                                while ( 0 );
                                (tt2l = (kc->u.narrow[2 * 13 + 0]) ^ (kc->u.narrow[2 * 1 + 0]), tt2h = (kc->u.narrow[2 * 13 + 1]) ^ (kc->u.narrow[2 * 1 + 1]) );
                                (tt3l = (kc->u.narrow[2 * 19 + 0]) ^ (kc->u.narrow[2 * 7 + 0]), tt3h = (kc->u.narrow[2 * 19 + 1]) ^ (kc->u.narrow[2 * 7 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 20 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 20 + 1]) );
                                (tt2l = tt2l ^ tt3l, tt2h = tt2h ^ tt3h);
                                (t3l = tt0l ^ tt2l, t3h = tt0h ^ tt2h);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 tt0l, tt0h;
                                sph_u64 tt1l, tt1h;
                                sph_u64 tt2l, tt2h;
                                sph_u64 tt3l, tt3h;
                                (tt0l = (kc->u.narrow[2 * 0 + 0]) ^ (kc->u.narrow[2 * 18 + 0]), tt0h = (kc->u.narrow[2 * 0 + 1]) ^ (kc->u.narrow[2 * 18 + 1]) );
                                (tt1l = (kc->u.narrow[2 * 6 + 0]) ^ (kc->u.narrow[2 * 24 + 0]), tt1h = (kc->u.narrow[2 * 6 + 1]) ^ (kc->u.narrow[2 * 24 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 12 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 12 + 1]) );
                                (tt0l = tt0l ^ tt1l, tt0h = tt0h ^ tt1h);
                                do
                                {
                                    sph_u32 tmp;
                                    tmp = tt0l;
                                    tt0l = ( (tt0h << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | (tt0h >> 31);
                                    tt0h = tmp;
                                }
                                while ( 0 );
                                (tt2l = (kc->u.narrow[2 * 17 + 0]) ^ (kc->u.narrow[2 * 5 + 0]), tt2h = (kc->u.narrow[2 * 17 + 1]) ^ (kc->u.narrow[2 * 5 + 1]) );
                                (tt3l = (kc->u.narrow[2 * 23 + 0]) ^ (kc->u.narrow[2 * 11 + 0]), tt3h = (kc->u.narrow[2 * 23 + 1]) ^ (kc->u.narrow[2 * 11 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 4 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 4 + 1]) );
                                (tt2l = tt2l ^ tt3l, tt2h = tt2h ^ tt3h);
                                (t4l = tt0l ^ tt2l, t4h = tt0h ^ tt2h);
                            }
                            while ( 0 );
                            ( (kc->u.narrow[2 * 0 + 0]) = (kc->u.narrow[2 * 0 + 0]) ^ t0l, (kc->u.narrow[2 * 0 + 1]) = (kc->u.narrow[2 * 0 + 1]) ^ t0h);
                            ( (kc->u.narrow[2 * 18 + 0]) = (kc->u.narrow[2 * 18 + 0]) ^ t0l, (kc->u.narrow[2 * 18 + 1]) = (kc->u.narrow[2 * 18 + 1]) ^ t0h);
                            ( (kc->u.narrow[2 * 6 + 0]) = (kc->u.narrow[2 * 6 + 0]) ^ t0l, (kc->u.narrow[2 * 6 + 1]) = (kc->u.narrow[2 * 6 + 1]) ^ t0h);
                            ( (kc->u.narrow[2 * 24 + 0]) = (kc->u.narrow[2 * 24 + 0]) ^ t0l, (kc->u.narrow[2 * 24 + 1]) = (kc->u.narrow[2 * 24 + 1]) ^ t0h);
                            ( (kc->u.narrow[2 * 12 + 0]) = (kc->u.narrow[2 * 12 + 0]) ^ t0l, (kc->u.narrow[2 * 12 + 1]) = (kc->u.narrow[2 * 12 + 1]) ^ t0h);
                            ( (kc->u.narrow[2 * 9 + 0]) = (kc->u.narrow[2 * 9 + 0]) ^ t1l, (kc->u.narrow[2 * 9 + 1]) = (kc->u.narrow[2 * 9 + 1]) ^ t1h);
                            ( (kc->u.narrow[2 * 22 + 0]) = (kc->u.narrow[2 * 22 + 0]) ^ t1l, (kc->u.narrow[2 * 22 + 1]) = (kc->u.narrow[2 * 22 + 1]) ^ t1h);
                            ( (kc->u.narrow[2 * 10 + 0]) = (kc->u.narrow[2 * 10 + 0]) ^ t1l, (kc->u.narrow[2 * 10 + 1]) = (kc->u.narrow[2 * 10 + 1]) ^ t1h);
                            ( (kc->u.narrow[2 * 3 + 0]) = (kc->u.narrow[2 * 3 + 0]) ^ t1l, (kc->u.narrow[2 * 3 + 1]) = (kc->u.narrow[2 * 3 + 1]) ^ t1h);
                            ( (kc->u.narrow[2 * 16 + 0]) = (kc->u.narrow[2 * 16 + 0]) ^ t1l, (kc->u.narrow[2 * 16 + 1]) = (kc->u.narrow[2 * 16 + 1]) ^ t1h);
                            ( (kc->u.narrow[2 * 13 + 0]) = (kc->u.narrow[2 * 13 + 0]) ^ t2l, (kc->u.narrow[2 * 13 + 1]) = (kc->u.narrow[2 * 13 + 1]) ^ t2h);
                            ( (kc->u.narrow[2 * 1 + 0]) = (kc->u.narrow[2 * 1 + 0]) ^ t2l, (kc->u.narrow[2 * 1 + 1]) = (kc->u.narrow[2 * 1 + 1]) ^ t2h);
                            ( (kc->u.narrow[2 * 19 + 0]) = (kc->u.narrow[2 * 19 + 0]) ^ t2l, (kc->u.narrow[2 * 19 + 1]) = (kc->u.narrow[2 * 19 + 1]) ^ t2h);
                            ( (kc->u.narrow[2 * 7 + 0]) = (kc->u.narrow[2 * 7 + 0]) ^ t2l, (kc->u.narrow[2 * 7 + 1]) = (kc->u.narrow[2 * 7 + 1]) ^ t2h);
                            ( (kc->u.narrow[2 * 20 + 0]) = (kc->u.narrow[2 * 20 + 0]) ^ t2l, (kc->u.narrow[2 * 20 + 1]) = (kc->u.narrow[2 * 20 + 1]) ^ t2h);
                            ( (kc->u.narrow[2 * 17 + 0]) = (kc->u.narrow[2 * 17 + 0]) ^ t3l, (kc->u.narrow[2 * 17 + 1]) = (kc->u.narrow[2 * 17 + 1]) ^ t3h);
                            ( (kc->u.narrow[2 * 5 + 0]) = (kc->u.narrow[2 * 5 + 0]) ^ t3l, (kc->u.narrow[2 * 5 + 1]) = (kc->u.narrow[2 * 5 + 1]) ^ t3h);
                            ( (kc->u.narrow[2 * 23 + 0]) = (kc->u.narrow[2 * 23 + 0]) ^ t3l, (kc->u.narrow[2 * 23 + 1]) = (kc->u.narrow[2 * 23 + 1]) ^ t3h);
                            ( (kc->u.narrow[2 * 11 + 0]) = (kc->u.narrow[2 * 11 + 0]) ^ t3l, (kc->u.narrow[2 * 11 + 1]) = (kc->u.narrow[2 * 11 + 1]) ^ t3h);
                            ( (kc->u.narrow[2 * 4 + 0]) = (kc->u.narrow[2 * 4 + 0]) ^ t3l, (kc->u.narrow[2 * 4 + 1]) = (kc->u.narrow[2 * 4 + 1]) ^ t3h);
                            ( (kc->u.narrow[2 * 21 + 0]) = (kc->u.narrow[2 * 21 + 0]) ^ t4l, (kc->u.narrow[2 * 21 + 1]) = (kc->u.narrow[2 * 21 + 1]) ^ t4h);
                            ( (kc->u.narrow[2 * 14 + 0]) = (kc->u.narrow[2 * 14 + 0]) ^ t4l, (kc->u.narrow[2 * 14 + 1]) = (kc->u.narrow[2 * 14 + 1]) ^ t4h);
                            ( (kc->u.narrow[2 * 2 + 0]) = (kc->u.narrow[2 * 2 + 0]) ^ t4l, (kc->u.narrow[2 * 2 + 1]) = (kc->u.narrow[2 * 2 + 1]) ^ t4h);
                            ( (kc->u.narrow[2 * 15 + 0]) = (kc->u.narrow[2 * 15 + 0]) ^ t4l, (kc->u.narrow[2 * 15 + 1]) = (kc->u.narrow[2 * 15 + 1]) ^ t4h);
                            ( (kc->u.narrow[2 * 8 + 0]) = (kc->u.narrow[2 * 8 + 0]) ^ t4l, (kc->u.narrow[2 * 8 + 1]) = (kc->u.narrow[2 * 8 + 1]) ^ t4h);
                        }
                        while ( 0 );
                        do
                        {
                            do
                            {
                                (kc->u.narrow[2 * 18 + 0]) = ( ( (kc->u.narrow[2 * 18 + 0]) << 18) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 18 + 0]) >> (32 - 18) );
                                (kc->u.narrow[2 * 18 + 1]) = ( ( (kc->u.narrow[2 * 18 + 1]) << 18) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 18 + 1]) >> (32 - 18) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 6 + 0]) << (2 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 6 + 0]) >> (33 - 2) );
                                (kc->u.narrow[2 * 6 + 0]) = ( ( (kc->u.narrow[2 * 6 + 1]) << 2) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 6 + 1]) >> (32 - 2) );
                                (kc->u.narrow[2 * 6 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 24 + 0]) << (21 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 24 + 0]) >> (33 - 21) );
                                (kc->u.narrow[2 * 24 + 0]) = ( ( (kc->u.narrow[2 * 24 + 1]) << 21) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 24 + 1]) >> (32 - 21) );
                                (kc->u.narrow[2 * 24 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 12 + 0]) = ( ( (kc->u.narrow[2 * 12 + 0]) << 9) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 12 + 0]) >> (32 - 9) );
                                (kc->u.narrow[2 * 12 + 1]) = ( ( (kc->u.narrow[2 * 12 + 1]) << 9) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 12 + 1]) >> (32 - 9) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = (kc->u.narrow[2 * 9 + 0]);
                                (kc->u.narrow[2 * 9 + 0]) = ( ( (kc->u.narrow[2 * 9 + 1]) << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 9 + 1]) >> 31);
                                (kc->u.narrow[2 * 9 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 22 + 0]) = ( ( (kc->u.narrow[2 * 22 + 0]) << 22) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 22 + 0]) >> (32 - 22) );
                                (kc->u.narrow[2 * 22 + 1]) = ( ( (kc->u.narrow[2 * 22 + 1]) << 22) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 22 + 1]) >> (32 - 22) );
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 10 + 0]) = ( ( (kc->u.narrow[2 * 10 + 0]) << 5) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 10 + 0]) >> (32 - 5) );
                                (kc->u.narrow[2 * 10 + 1]) = ( ( (kc->u.narrow[2 * 10 + 1]) << 5) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 10 + 1]) >> (32 - 5) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 3 + 0]) << (23 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 3 + 0]) >> (33 - 23) );
                                (kc->u.narrow[2 * 3 + 0]) = ( ( (kc->u.narrow[2 * 3 + 1]) << 23) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 3 + 1]) >> (32 - 23) );
                                (kc->u.narrow[2 * 3 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 16 + 0]) = ( ( (kc->u.narrow[2 * 16 + 0]) << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 16 + 0]) >> (32 - 1) );
                                (kc->u.narrow[2 * 16 + 1]) = ( ( (kc->u.narrow[2 * 16 + 1]) << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 16 + 1]) >> (32 - 1) );
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 13 + 0]) = ( ( (kc->u.narrow[2 * 13 + 0]) << 31) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 13 + 0]) >> (32 - 31) );
                                (kc->u.narrow[2 * 13 + 1]) = ( ( (kc->u.narrow[2 * 13 + 1]) << 31) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 13 + 1]) >> (32 - 31) );
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 1 + 0]) = ( ( (kc->u.narrow[2 * 1 + 0]) << 3) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 1 + 0]) >> (32 - 3) );
                                (kc->u.narrow[2 * 1 + 1]) = ( ( (kc->u.narrow[2 * 1 + 1]) << 3) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 1 + 1]) >> (32 - 3) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 19 + 0]) << (22 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 19 + 0]) >> (33 - 22) );
                                (kc->u.narrow[2 * 19 + 0]) = ( ( (kc->u.narrow[2 * 19 + 1]) << 22) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 19 + 1]) >> (32 - 22) );
                                (kc->u.narrow[2 * 19 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 7 + 0]) << (8 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 7 + 0]) >> (33 - 8) );
                                (kc->u.narrow[2 * 7 + 0]) = ( ( (kc->u.narrow[2 * 7 + 1]) << 8) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 7 + 1]) >> (32 - 8) );
                                (kc->u.narrow[2 * 7 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 20 + 0]) << (31 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 20 + 0]) >> (33 - 31) );
                                (kc->u.narrow[2 * 20 + 0]) = ( ( (kc->u.narrow[2 * 20 + 1]) << 31) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 20 + 1]) >> (32 - 31) );
                                (kc->u.narrow[2 * 20 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 17 + 0]) = ( ( (kc->u.narrow[2 * 17 + 0]) << 14) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 17 + 0]) >> (32 - 14) );
                                (kc->u.narrow[2 * 17 + 1]) = ( ( (kc->u.narrow[2 * 17 + 1]) << 14) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 17 + 1]) >> (32 - 14) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 5 + 0]) << (28 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 5 + 0]) >> (33 - 28) );
                                (kc->u.narrow[2 * 5 + 0]) = ( ( (kc->u.narrow[2 * 5 + 1]) << 28) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 5 + 1]) >> (32 - 28) );
                                (kc->u.narrow[2 * 5 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 23 + 0]) << (13 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 23 + 0]) >> (33 - 13) );
                                (kc->u.narrow[2 * 23 + 0]) = ( ( (kc->u.narrow[2 * 23 + 1]) << 13) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 23 + 1]) >> (32 - 13) );
                                (kc->u.narrow[2 * 23 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 11 + 0]) << (11 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 11 + 0]) >> (33 - 11) );
                                (kc->u.narrow[2 * 11 + 0]) = ( ( (kc->u.narrow[2 * 11 + 1]) << 11) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 11 + 1]) >> (32 - 11) );
                                (kc->u.narrow[2 * 11 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 4 + 0]) = ( ( (kc->u.narrow[2 * 4 + 0]) << 28) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 4 + 0]) >> (32 - 28) );
                                (kc->u.narrow[2 * 4 + 1]) = ( ( (kc->u.narrow[2 * 4 + 1]) << 28) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 4 + 1]) >> (32 - 28) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 21 + 0]) << (14 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 21 + 0]) >> (33 - 14) );
                                (kc->u.narrow[2 * 21 + 0]) = ( ( (kc->u.narrow[2 * 21 + 1]) << 14) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 21 + 1]) >> (32 - 14) );
                                (kc->u.narrow[2 * 21 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 14 + 0]) = ( ( (kc->u.narrow[2 * 14 + 0]) << 10) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 14 + 0]) >> (32 - 10) );
                                (kc->u.narrow[2 * 14 + 1]) = ( ( (kc->u.narrow[2 * 14 + 1]) << 10) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 14 + 1]) >> (32 - 10) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 2 + 0]) << (20 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 2 + 0]) >> (33 - 20) );
                                (kc->u.narrow[2 * 2 + 0]) = ( ( (kc->u.narrow[2 * 2 + 1]) << 20) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 2 + 1]) >> (32 - 20) );
                                (kc->u.narrow[2 * 2 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 15 + 0]) = ( ( (kc->u.narrow[2 * 15 + 0]) << 4) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 15 + 0]) >> (32 - 4) );
                                (kc->u.narrow[2 * 15 + 1]) = ( ( (kc->u.narrow[2 * 15 + 1]) << 4) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 15 + 1]) >> (32 - 4) );
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 8 + 0]) = ( ( (kc->u.narrow[2 * 8 + 0]) << 7) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 8 + 0]) >> (32 - 7) );
                                (kc->u.narrow[2 * 8 + 1]) = ( ( (kc->u.narrow[2 * 8 + 1]) << 7) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 8 + 1]) >> (32 - 7) );
                            }
                            while ( 0 );
                        }
                        while ( 0 );
                        do
                        {
                            sph_u64 c0l, c0h;
                            sph_u64 c1l, c1h;
                            sph_u64 c2l, c2h;
                            sph_u64 c3l, c3h;
                            sph_u64 c4l, c4h;
                            sph_u64 bnnl, bnnh;
                            (bnnl = ( (~(kc->u.narrow[2 * 19 + 0]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ), bnnh = ( (~(kc->u.narrow[2 * 19 + 1]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 22 + 0]) | (kc->u.narrow[2 * 19 + 0]), kth = (kc->u.narrow[2 * 22 + 1]) | (kc->u.narrow[2 * 19 + 1]) );
                                (c0l = (kc->u.narrow[2 * 0 + 0]) ^ ktl, c0h = (kc->u.narrow[2 * 0 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = bnnl | (kc->u.narrow[2 * 11 + 0]), kth = bnnh | (kc->u.narrow[2 * 11 + 1]) );
                                (c1l = (kc->u.narrow[2 * 22 + 0]) ^ ktl, c1h = (kc->u.narrow[2 * 22 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 11 + 0]) & (kc->u.narrow[2 * 8 + 0]), kth = (kc->u.narrow[2 * 11 + 1]) & (kc->u.narrow[2 * 8 + 1]) );
                                (c2l = (kc->u.narrow[2 * 19 + 0]) ^ ktl, c2h = (kc->u.narrow[2 * 19 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 8 + 0]) | (kc->u.narrow[2 * 0 + 0]), kth = (kc->u.narrow[2 * 8 + 1]) | (kc->u.narrow[2 * 0 + 1]) );
                                (c3l = (kc->u.narrow[2 * 11 + 0]) ^ ktl, c3h = (kc->u.narrow[2 * 11 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 0 + 0]) & (kc->u.narrow[2 * 22 + 0]), kth = (kc->u.narrow[2 * 0 + 1]) & (kc->u.narrow[2 * 22 + 1]) );
                                (c4l = (kc->u.narrow[2 * 8 + 0]) ^ ktl, c4h = (kc->u.narrow[2 * 8 + 1]) ^ kth);
                            }
                            while ( 0 );
                            ( (kc->u.narrow[2 * 0 + 0]) = c0l, (kc->u.narrow[2 * 0 + 1]) = c0h);
                            ( (kc->u.narrow[2 * 22 + 0]) = c1l, (kc->u.narrow[2 * 22 + 1]) = c1h);
                            ( (kc->u.narrow[2 * 19 + 0]) = c2l, (kc->u.narrow[2 * 19 + 1]) = c2h);
                            ( (kc->u.narrow[2 * 11 + 0]) = c3l, (kc->u.narrow[2 * 11 + 1]) = c3h);
                            ( (kc->u.narrow[2 * 8 + 0]) = c4l, (kc->u.narrow[2 * 8 + 1]) = c4h);
                            (bnnl = ( (~(kc->u.narrow[2 * 20 + 0]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ), bnnh = ( (~(kc->u.narrow[2 * 20 + 1]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 14 + 0]) | (kc->u.narrow[2 * 6 + 0]), kth = (kc->u.narrow[2 * 14 + 1]) | (kc->u.narrow[2 * 6 + 1]) );
                                (c0l = (kc->u.narrow[2 * 17 + 0]) ^ ktl, c0h = (kc->u.narrow[2 * 17 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 6 + 0]) & (kc->u.narrow[2 * 3 + 0]), kth = (kc->u.narrow[2 * 6 + 1]) & (kc->u.narrow[2 * 3 + 1]) );
                                (c1l = (kc->u.narrow[2 * 14 + 0]) ^ ktl, c1h = (kc->u.narrow[2 * 14 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 3 + 0]) | bnnl, kth = (kc->u.narrow[2 * 3 + 1]) | bnnh);
                                (c2l = (kc->u.narrow[2 * 6 + 0]) ^ ktl, c2h = (kc->u.narrow[2 * 6 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 20 + 0]) | (kc->u.narrow[2 * 17 + 0]), kth = (kc->u.narrow[2 * 20 + 1]) | (kc->u.narrow[2 * 17 + 1]) );
                                (c3l = (kc->u.narrow[2 * 3 + 0]) ^ ktl, c3h = (kc->u.narrow[2 * 3 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 17 + 0]) & (kc->u.narrow[2 * 14 + 0]), kth = (kc->u.narrow[2 * 17 + 1]) & (kc->u.narrow[2 * 14 + 1]) );
                                (c4l = (kc->u.narrow[2 * 20 + 0]) ^ ktl, c4h = (kc->u.narrow[2 * 20 + 1]) ^ kth);
                            }
                            while ( 0 );
                            ( (kc->u.narrow[2 * 17 + 0]) = c0l, (kc->u.narrow[2 * 17 + 1]) = c0h);
                            ( (kc->u.narrow[2 * 14 + 0]) = c1l, (kc->u.narrow[2 * 14 + 1]) = c1h);
                            ( (kc->u.narrow[2 * 6 + 0]) = c2l, (kc->u.narrow[2 * 6 + 1]) = c2h);
                            ( (kc->u.narrow[2 * 3 + 0]) = c3l, (kc->u.narrow[2 * 3 + 1]) = c3h);
                            ( (kc->u.narrow[2 * 20 + 0]) = c4l, (kc->u.narrow[2 * 20 + 1]) = c4h);
                            (bnnl = ( (~(kc->u.narrow[2 * 15 + 0]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ), bnnh = ( (~(kc->u.narrow[2 * 15 + 1]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 1 + 0]) | (kc->u.narrow[2 * 23 + 0]), kth = (kc->u.narrow[2 * 1 + 1]) | (kc->u.narrow[2 * 23 + 1]) );
                                (c0l = (kc->u.narrow[2 * 9 + 0]) ^ ktl, c0h = (kc->u.narrow[2 * 9 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 23 + 0]) & (kc->u.narrow[2 * 15 + 0]), kth = (kc->u.narrow[2 * 23 + 1]) & (kc->u.narrow[2 * 15 + 1]) );
                                (c1l = (kc->u.narrow[2 * 1 + 0]) ^ ktl, c1h = (kc->u.narrow[2 * 1 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = bnnl & (kc->u.narrow[2 * 12 + 0]), kth = bnnh & (kc->u.narrow[2 * 12 + 1]) );
                                (c2l = (kc->u.narrow[2 * 23 + 0]) ^ ktl, c2h = (kc->u.narrow[2 * 23 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 12 + 0]) | (kc->u.narrow[2 * 9 + 0]), kth = (kc->u.narrow[2 * 12 + 1]) | (kc->u.narrow[2 * 9 + 1]) );
                                (c3l = bnnl ^ ktl, c3h = bnnh ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 9 + 0]) & (kc->u.narrow[2 * 1 + 0]), kth = (kc->u.narrow[2 * 9 + 1]) & (kc->u.narrow[2 * 1 + 1]) );
                                (c4l = (kc->u.narrow[2 * 12 + 0]) ^ ktl, c4h = (kc->u.narrow[2 * 12 + 1]) ^ kth);
                            }
                            while ( 0 );
                            ( (kc->u.narrow[2 * 9 + 0]) = c0l, (kc->u.narrow[2 * 9 + 1]) = c0h);
                            ( (kc->u.narrow[2 * 1 + 0]) = c1l, (kc->u.narrow[2 * 1 + 1]) = c1h);
                            ( (kc->u.narrow[2 * 23 + 0]) = c2l, (kc->u.narrow[2 * 23 + 1]) = c2h);
                            ( (kc->u.narrow[2 * 15 + 0]) = c3l, (kc->u.narrow[2 * 15 + 1]) = c3h);
                            ( (kc->u.narrow[2 * 12 + 0]) = c4l, (kc->u.narrow[2 * 12 + 1]) = c4h);
                            (bnnl = ( (~(kc->u.narrow[2 * 7 + 0]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ), bnnh = ( (~(kc->u.narrow[2 * 7 + 1]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 18 + 0]) & (kc->u.narrow[2 * 10 + 0]), kth = (kc->u.narrow[2 * 18 + 1]) & (kc->u.narrow[2 * 10 + 1]) );
                                (c0l = (kc->u.narrow[2 * 21 + 0]) ^ ktl, c0h = (kc->u.narrow[2 * 21 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 10 + 0]) | (kc->u.narrow[2 * 7 + 0]), kth = (kc->u.narrow[2 * 10 + 1]) | (kc->u.narrow[2 * 7 + 1]) );
                                (c1l = (kc->u.narrow[2 * 18 + 0]) ^ ktl, c1h = (kc->u.narrow[2 * 18 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = bnnl | (kc->u.narrow[2 * 4 + 0]), kth = bnnh | (kc->u.narrow[2 * 4 + 1]) );
                                (c2l = (kc->u.narrow[2 * 10 + 0]) ^ ktl, c2h = (kc->u.narrow[2 * 10 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 4 + 0]) & (kc->u.narrow[2 * 21 + 0]), kth = (kc->u.narrow[2 * 4 + 1]) & (kc->u.narrow[2 * 21 + 1]) );
                                (c3l = bnnl ^ ktl, c3h = bnnh ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 21 + 0]) | (kc->u.narrow[2 * 18 + 0]), kth = (kc->u.narrow[2 * 21 + 1]) | (kc->u.narrow[2 * 18 + 1]) );
                                (c4l = (kc->u.narrow[2 * 4 + 0]) ^ ktl, c4h = (kc->u.narrow[2 * 4 + 1]) ^ kth);
                            }
                            while ( 0 );
                            ( (kc->u.narrow[2 * 21 + 0]) = c0l, (kc->u.narrow[2 * 21 + 1]) = c0h);
                            ( (kc->u.narrow[2 * 18 + 0]) = c1l, (kc->u.narrow[2 * 18 + 1]) = c1h);
                            ( (kc->u.narrow[2 * 10 + 0]) = c2l, (kc->u.narrow[2 * 10 + 1]) = c2h);
                            ( (kc->u.narrow[2 * 7 + 0]) = c3l, (kc->u.narrow[2 * 7 + 1]) = c3h);
                            ( (kc->u.narrow[2 * 4 + 0]) = c4l, (kc->u.narrow[2 * 4 + 1]) = c4h);
                            (bnnl = ( (~(kc->u.narrow[2 * 5 + 0]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ), bnnh = ( (~(kc->u.narrow[2 * 5 + 1]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = bnnl & (kc->u.narrow[2 * 2 + 0]), kth = bnnh & (kc->u.narrow[2 * 2 + 1]) );
                                (c0l = (kc->u.narrow[2 * 13 + 0]) ^ ktl, c0h = (kc->u.narrow[2 * 13 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 2 + 0]) | (kc->u.narrow[2 * 24 + 0]), kth = (kc->u.narrow[2 * 2 + 1]) | (kc->u.narrow[2 * 24 + 1]) );
                                (c1l = bnnl ^ ktl, c1h = bnnh ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 24 + 0]) & (kc->u.narrow[2 * 16 + 0]), kth = (kc->u.narrow[2 * 24 + 1]) & (kc->u.narrow[2 * 16 + 1]) );
                                (c2l = (kc->u.narrow[2 * 2 + 0]) ^ ktl, c2h = (kc->u.narrow[2 * 2 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 16 + 0]) | (kc->u.narrow[2 * 13 + 0]), kth = (kc->u.narrow[2 * 16 + 1]) | (kc->u.narrow[2 * 13 + 1]) );
                                (c3l = (kc->u.narrow[2 * 24 + 0]) ^ ktl, c3h = (kc->u.narrow[2 * 24 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 13 + 0]) & (kc->u.narrow[2 * 5 + 0]), kth = (kc->u.narrow[2 * 13 + 1]) & (kc->u.narrow[2 * 5 + 1]) );
                                (c4l = (kc->u.narrow[2 * 16 + 0]) ^ ktl, c4h = (kc->u.narrow[2 * 16 + 1]) ^ kth);
                            }
                            while ( 0 );
                            ( (kc->u.narrow[2 * 13 + 0]) = c0l, (kc->u.narrow[2 * 13 + 1]) = c0h);
                            ( (kc->u.narrow[2 * 5 + 0]) = c1l, (kc->u.narrow[2 * 5 + 1]) = c1h);
                            ( (kc->u.narrow[2 * 2 + 0]) = c2l, (kc->u.narrow[2 * 2 + 1]) = c2h);
                            ( (kc->u.narrow[2 * 24 + 0]) = c3l, (kc->u.narrow[2 * 24 + 1]) = c3h);
                            ( (kc->u.narrow[2 * 16 + 0]) = c4l, (kc->u.narrow[2 * 16 + 1]) = c4h);
                        }
                        while ( 0 );
                        ( (kc->u.narrow[2 * 0 + 0]) = (kc->u.narrow[2 * 0 + 0]) ^ RC[j + 2].low, (kc->u.narrow[2 * 0 + 1]) = (kc->u.narrow[2 * 0 + 1]) ^ RC[j + 2].high);
                    }
                    while ( 0 );
                    if(out)
                    {
                        printf( "\n第%d轮中间状态：\n",k++ );
                        print_hex(kc->u.narrow, 1600);
                    }
                    do
                    {
                        do
                        {
                            sph_u64 t0l, t0h;
                            sph_u64 t1l, t1h;
                            sph_u64 t2l, t2h;
                            sph_u64 t3l, t3h;
                            sph_u64 t4l, t4h;
                            do
                            {
                                sph_u64 tt0l, tt0h;
                                sph_u64 tt1l, tt1h;
                                sph_u64 tt2l, tt2h;
                                sph_u64 tt3l, tt3h;
                                (tt0l = (kc->u.narrow[2 * 22 + 0]) ^ (kc->u.narrow[2 * 14 + 0]), tt0h = (kc->u.narrow[2 * 22 + 1]) ^ (kc->u.narrow[2 * 14 + 1]) );
                                (tt1l = (kc->u.narrow[2 * 1 + 0]) ^ (kc->u.narrow[2 * 18 + 0]), tt1h = (kc->u.narrow[2 * 1 + 1]) ^ (kc->u.narrow[2 * 18 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 5 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 5 + 1]) );
                                (tt0l = tt0l ^ tt1l, tt0h = tt0h ^ tt1h);
                                do
                                {
                                    sph_u32 tmp;
                                    tmp = tt0l;
                                    tt0l = ( (tt0h << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | (tt0h >> 31);
                                    tt0h = tmp;
                                }
                                while ( 0 );
                                (tt2l = (kc->u.narrow[2 * 8 + 0]) ^ (kc->u.narrow[2 * 20 + 0]), tt2h = (kc->u.narrow[2 * 8 + 1]) ^ (kc->u.narrow[2 * 20 + 1]) );
                                (tt3l = (kc->u.narrow[2 * 12 + 0]) ^ (kc->u.narrow[2 * 4 + 0]), tt3h = (kc->u.narrow[2 * 12 + 1]) ^ (kc->u.narrow[2 * 4 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 16 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 16 + 1]) );
                                (tt2l = tt2l ^ tt3l, tt2h = tt2h ^ tt3h);
                                (t0l = tt0l ^ tt2l, t0h = tt0h ^ tt2h);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 tt0l, tt0h;
                                sph_u64 tt1l, tt1h;
                                sph_u64 tt2l, tt2h;
                                sph_u64 tt3l, tt3h;
                                (tt0l = (kc->u.narrow[2 * 19 + 0]) ^ (kc->u.narrow[2 * 6 + 0]), tt0h = (kc->u.narrow[2 * 19 + 1]) ^ (kc->u.narrow[2 * 6 + 1]) );
                                (tt1l = (kc->u.narrow[2 * 23 + 0]) ^ (kc->u.narrow[2 * 10 + 0]), tt1h = (kc->u.narrow[2 * 23 + 1]) ^ (kc->u.narrow[2 * 10 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 2 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 2 + 1]) );
                                (tt0l = tt0l ^ tt1l, tt0h = tt0h ^ tt1h);
                                do
                                {
                                    sph_u32 tmp;
                                    tmp = tt0l;
                                    tt0l = ( (tt0h << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | (tt0h >> 31);
                                    tt0h = tmp;
                                }
                                while ( 0 );
                                (tt2l = (kc->u.narrow[2 * 0 + 0]) ^ (kc->u.narrow[2 * 17 + 0]), tt2h = (kc->u.narrow[2 * 0 + 1]) ^ (kc->u.narrow[2 * 17 + 1]) );
                                (tt3l = (kc->u.narrow[2 * 9 + 0]) ^ (kc->u.narrow[2 * 21 + 0]), tt3h = (kc->u.narrow[2 * 9 + 1]) ^ (kc->u.narrow[2 * 21 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 13 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 13 + 1]) );
                                (tt2l = tt2l ^ tt3l, tt2h = tt2h ^ tt3h);
                                (t1l = tt0l ^ tt2l, t1h = tt0h ^ tt2h);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 tt0l, tt0h;
                                sph_u64 tt1l, tt1h;
                                sph_u64 tt2l, tt2h;
                                sph_u64 tt3l, tt3h;
                                (tt0l = (kc->u.narrow[2 * 11 + 0]) ^ (kc->u.narrow[2 * 3 + 0]), tt0h = (kc->u.narrow[2 * 11 + 1]) ^ (kc->u.narrow[2 * 3 + 1]) );
                                (tt1l = (kc->u.narrow[2 * 15 + 0]) ^ (kc->u.narrow[2 * 7 + 0]), tt1h = (kc->u.narrow[2 * 15 + 1]) ^ (kc->u.narrow[2 * 7 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 24 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 24 + 1]) );
                                (tt0l = tt0l ^ tt1l, tt0h = tt0h ^ tt1h);
                                do
                                {
                                    sph_u32 tmp;
                                    tmp = tt0l;
                                    tt0l = ( (tt0h << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | (tt0h >> 31);
                                    tt0h = tmp;
                                }
                                while ( 0 );
                                (tt2l = (kc->u.narrow[2 * 22 + 0]) ^ (kc->u.narrow[2 * 14 + 0]), tt2h = (kc->u.narrow[2 * 22 + 1]) ^ (kc->u.narrow[2 * 14 + 1]) );
                                (tt3l = (kc->u.narrow[2 * 1 + 0]) ^ (kc->u.narrow[2 * 18 + 0]), tt3h = (kc->u.narrow[2 * 1 + 1]) ^ (kc->u.narrow[2 * 18 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 5 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 5 + 1]) );
                                (tt2l = tt2l ^ tt3l, tt2h = tt2h ^ tt3h);
                                (t2l = tt0l ^ tt2l, t2h = tt0h ^ tt2h);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 tt0l, tt0h;
                                sph_u64 tt1l, tt1h;
                                sph_u64 tt2l, tt2h;
                                sph_u64 tt3l, tt3h;
                                (tt0l = (kc->u.narrow[2 * 8 + 0]) ^ (kc->u.narrow[2 * 20 + 0]), tt0h = (kc->u.narrow[2 * 8 + 1]) ^ (kc->u.narrow[2 * 20 + 1]) );
                                (tt1l = (kc->u.narrow[2 * 12 + 0]) ^ (kc->u.narrow[2 * 4 + 0]), tt1h = (kc->u.narrow[2 * 12 + 1]) ^ (kc->u.narrow[2 * 4 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 16 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 16 + 1]) );
                                (tt0l = tt0l ^ tt1l, tt0h = tt0h ^ tt1h);
                                do
                                {
                                    sph_u32 tmp;
                                    tmp = tt0l;
                                    tt0l = ( (tt0h << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | (tt0h >> 31);
                                    tt0h = tmp;
                                }
                                while ( 0 );
                                (tt2l = (kc->u.narrow[2 * 19 + 0]) ^ (kc->u.narrow[2 * 6 + 0]), tt2h = (kc->u.narrow[2 * 19 + 1]) ^ (kc->u.narrow[2 * 6 + 1]) );
                                (tt3l = (kc->u.narrow[2 * 23 + 0]) ^ (kc->u.narrow[2 * 10 + 0]), tt3h = (kc->u.narrow[2 * 23 + 1]) ^ (kc->u.narrow[2 * 10 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 2 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 2 + 1]) );
                                (tt2l = tt2l ^ tt3l, tt2h = tt2h ^ tt3h);
                                (t3l = tt0l ^ tt2l, t3h = tt0h ^ tt2h);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 tt0l, tt0h;
                                sph_u64 tt1l, tt1h;
                                sph_u64 tt2l, tt2h;
                                sph_u64 tt3l, tt3h;
                                (tt0l = (kc->u.narrow[2 * 0 + 0]) ^ (kc->u.narrow[2 * 17 + 0]), tt0h = (kc->u.narrow[2 * 0 + 1]) ^ (kc->u.narrow[2 * 17 + 1]) );
                                (tt1l = (kc->u.narrow[2 * 9 + 0]) ^ (kc->u.narrow[2 * 21 + 0]), tt1h = (kc->u.narrow[2 * 9 + 1]) ^ (kc->u.narrow[2 * 21 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 13 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 13 + 1]) );
                                (tt0l = tt0l ^ tt1l, tt0h = tt0h ^ tt1h);
                                do
                                {
                                    sph_u32 tmp;
                                    tmp = tt0l;
                                    tt0l = ( (tt0h << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | (tt0h >> 31);
                                    tt0h = tmp;
                                }
                                while ( 0 );
                                (tt2l = (kc->u.narrow[2 * 11 + 0]) ^ (kc->u.narrow[2 * 3 + 0]), tt2h = (kc->u.narrow[2 * 11 + 1]) ^ (kc->u.narrow[2 * 3 + 1]) );
                                (tt3l = (kc->u.narrow[2 * 15 + 0]) ^ (kc->u.narrow[2 * 7 + 0]), tt3h = (kc->u.narrow[2 * 15 + 1]) ^ (kc->u.narrow[2 * 7 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 24 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 24 + 1]) );
                                (tt2l = tt2l ^ tt3l, tt2h = tt2h ^ tt3h);
                                (t4l = tt0l ^ tt2l, t4h = tt0h ^ tt2h);
                            }
                            while ( 0 );
                            ( (kc->u.narrow[2 * 0 + 0]) = (kc->u.narrow[2 * 0 + 0]) ^ t0l, (kc->u.narrow[2 * 0 + 1]) = (kc->u.narrow[2 * 0 + 1]) ^ t0h);
                            ( (kc->u.narrow[2 * 17 + 0]) = (kc->u.narrow[2 * 17 + 0]) ^ t0l, (kc->u.narrow[2 * 17 + 1]) = (kc->u.narrow[2 * 17 + 1]) ^ t0h);
                            ( (kc->u.narrow[2 * 9 + 0]) = (kc->u.narrow[2 * 9 + 0]) ^ t0l, (kc->u.narrow[2 * 9 + 1]) = (kc->u.narrow[2 * 9 + 1]) ^ t0h);
                            ( (kc->u.narrow[2 * 21 + 0]) = (kc->u.narrow[2 * 21 + 0]) ^ t0l, (kc->u.narrow[2 * 21 + 1]) = (kc->u.narrow[2 * 21 + 1]) ^ t0h);
                            ( (kc->u.narrow[2 * 13 + 0]) = (kc->u.narrow[2 * 13 + 0]) ^ t0l, (kc->u.narrow[2 * 13 + 1]) = (kc->u.narrow[2 * 13 + 1]) ^ t0h);
                            ( (kc->u.narrow[2 * 22 + 0]) = (kc->u.narrow[2 * 22 + 0]) ^ t1l, (kc->u.narrow[2 * 22 + 1]) = (kc->u.narrow[2 * 22 + 1]) ^ t1h);
                            ( (kc->u.narrow[2 * 14 + 0]) = (kc->u.narrow[2 * 14 + 0]) ^ t1l, (kc->u.narrow[2 * 14 + 1]) = (kc->u.narrow[2 * 14 + 1]) ^ t1h);
                            ( (kc->u.narrow[2 * 1 + 0]) = (kc->u.narrow[2 * 1 + 0]) ^ t1l, (kc->u.narrow[2 * 1 + 1]) = (kc->u.narrow[2 * 1 + 1]) ^ t1h);
                            ( (kc->u.narrow[2 * 18 + 0]) = (kc->u.narrow[2 * 18 + 0]) ^ t1l, (kc->u.narrow[2 * 18 + 1]) = (kc->u.narrow[2 * 18 + 1]) ^ t1h);
                            ( (kc->u.narrow[2 * 5 + 0]) = (kc->u.narrow[2 * 5 + 0]) ^ t1l, (kc->u.narrow[2 * 5 + 1]) = (kc->u.narrow[2 * 5 + 1]) ^ t1h);
                            ( (kc->u.narrow[2 * 19 + 0]) = (kc->u.narrow[2 * 19 + 0]) ^ t2l, (kc->u.narrow[2 * 19 + 1]) = (kc->u.narrow[2 * 19 + 1]) ^ t2h);
                            ( (kc->u.narrow[2 * 6 + 0]) = (kc->u.narrow[2 * 6 + 0]) ^ t2l, (kc->u.narrow[2 * 6 + 1]) = (kc->u.narrow[2 * 6 + 1]) ^ t2h);
                            ( (kc->u.narrow[2 * 23 + 0]) = (kc->u.narrow[2 * 23 + 0]) ^ t2l, (kc->u.narrow[2 * 23 + 1]) = (kc->u.narrow[2 * 23 + 1]) ^ t2h);
                            ( (kc->u.narrow[2 * 10 + 0]) = (kc->u.narrow[2 * 10 + 0]) ^ t2l, (kc->u.narrow[2 * 10 + 1]) = (kc->u.narrow[2 * 10 + 1]) ^ t2h);
                            ( (kc->u.narrow[2 * 2 + 0]) = (kc->u.narrow[2 * 2 + 0]) ^ t2l, (kc->u.narrow[2 * 2 + 1]) = (kc->u.narrow[2 * 2 + 1]) ^ t2h);
                            ( (kc->u.narrow[2 * 11 + 0]) = (kc->u.narrow[2 * 11 + 0]) ^ t3l, (kc->u.narrow[2 * 11 + 1]) = (kc->u.narrow[2 * 11 + 1]) ^ t3h);
                            ( (kc->u.narrow[2 * 3 + 0]) = (kc->u.narrow[2 * 3 + 0]) ^ t3l, (kc->u.narrow[2 * 3 + 1]) = (kc->u.narrow[2 * 3 + 1]) ^ t3h);
                            ( (kc->u.narrow[2 * 15 + 0]) = (kc->u.narrow[2 * 15 + 0]) ^ t3l, (kc->u.narrow[2 * 15 + 1]) = (kc->u.narrow[2 * 15 + 1]) ^ t3h);
                            ( (kc->u.narrow[2 * 7 + 0]) = (kc->u.narrow[2 * 7 + 0]) ^ t3l, (kc->u.narrow[2 * 7 + 1]) = (kc->u.narrow[2 * 7 + 1]) ^ t3h);
                            ( (kc->u.narrow[2 * 24 + 0]) = (kc->u.narrow[2 * 24 + 0]) ^ t3l, (kc->u.narrow[2 * 24 + 1]) = (kc->u.narrow[2 * 24 + 1]) ^ t3h);
                            ( (kc->u.narrow[2 * 8 + 0]) = (kc->u.narrow[2 * 8 + 0]) ^ t4l, (kc->u.narrow[2 * 8 + 1]) = (kc->u.narrow[2 * 8 + 1]) ^ t4h);
                            ( (kc->u.narrow[2 * 20 + 0]) = (kc->u.narrow[2 * 20 + 0]) ^ t4l, (kc->u.narrow[2 * 20 + 1]) = (kc->u.narrow[2 * 20 + 1]) ^ t4h);
                            ( (kc->u.narrow[2 * 12 + 0]) = (kc->u.narrow[2 * 12 + 0]) ^ t4l, (kc->u.narrow[2 * 12 + 1]) = (kc->u.narrow[2 * 12 + 1]) ^ t4h);
                            ( (kc->u.narrow[2 * 4 + 0]) = (kc->u.narrow[2 * 4 + 0]) ^ t4l, (kc->u.narrow[2 * 4 + 1]) = (kc->u.narrow[2 * 4 + 1]) ^ t4h);
                            ( (kc->u.narrow[2 * 16 + 0]) = (kc->u.narrow[2 * 16 + 0]) ^ t4l, (kc->u.narrow[2 * 16 + 1]) = (kc->u.narrow[2 * 16 + 1]) ^ t4h);
                        }
                        while ( 0 );
                        do
                        {
                            do
                            {
                                (kc->u.narrow[2 * 17 + 0]) = ( ( (kc->u.narrow[2 * 17 + 0]) << 18) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 17 + 0]) >> (32 - 18) );
                                (kc->u.narrow[2 * 17 + 1]) = ( ( (kc->u.narrow[2 * 17 + 1]) << 18) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 17 + 1]) >> (32 - 18) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 9 + 0]) << (2 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 9 + 0]) >> (33 - 2) );
                                (kc->u.narrow[2 * 9 + 0]) = ( ( (kc->u.narrow[2 * 9 + 1]) << 2) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 9 + 1]) >> (32 - 2) );
                                (kc->u.narrow[2 * 9 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 21 + 0]) << (21 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 21 + 0]) >> (33 - 21) );
                                (kc->u.narrow[2 * 21 + 0]) = ( ( (kc->u.narrow[2 * 21 + 1]) << 21) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 21 + 1]) >> (32 - 21) );
                                (kc->u.narrow[2 * 21 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 13 + 0]) = ( ( (kc->u.narrow[2 * 13 + 0]) << 9) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 13 + 0]) >> (32 - 9) );
                                (kc->u.narrow[2 * 13 + 1]) = ( ( (kc->u.narrow[2 * 13 + 1]) << 9) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 13 + 1]) >> (32 - 9) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = (kc->u.narrow[2 * 22 + 0]);
                                (kc->u.narrow[2 * 22 + 0]) = ( ( (kc->u.narrow[2 * 22 + 1]) << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 22 + 1]) >> 31);
                                (kc->u.narrow[2 * 22 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 14 + 0]) = ( ( (kc->u.narrow[2 * 14 + 0]) << 22) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 14 + 0]) >> (32 - 22) );
                                (kc->u.narrow[2 * 14 + 1]) = ( ( (kc->u.narrow[2 * 14 + 1]) << 22) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 14 + 1]) >> (32 - 22) );
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 1 + 0]) = ( ( (kc->u.narrow[2 * 1 + 0]) << 5) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 1 + 0]) >> (32 - 5) );
                                (kc->u.narrow[2 * 1 + 1]) = ( ( (kc->u.narrow[2 * 1 + 1]) << 5) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 1 + 1]) >> (32 - 5) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 18 + 0]) << (23 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 18 + 0]) >> (33 - 23) );
                                (kc->u.narrow[2 * 18 + 0]) = ( ( (kc->u.narrow[2 * 18 + 1]) << 23) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 18 + 1]) >> (32 - 23) );
                                (kc->u.narrow[2 * 18 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 5 + 0]) = ( ( (kc->u.narrow[2 * 5 + 0]) << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 5 + 0]) >> (32 - 1) );
                                (kc->u.narrow[2 * 5 + 1]) = ( ( (kc->u.narrow[2 * 5 + 1]) << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 5 + 1]) >> (32 - 1) );
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 19 + 0]) = ( ( (kc->u.narrow[2 * 19 + 0]) << 31) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 19 + 0]) >> (32 - 31) );
                                (kc->u.narrow[2 * 19 + 1]) = ( ( (kc->u.narrow[2 * 19 + 1]) << 31) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 19 + 1]) >> (32 - 31) );
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 6 + 0]) = ( ( (kc->u.narrow[2 * 6 + 0]) << 3) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 6 + 0]) >> (32 - 3) );
                                (kc->u.narrow[2 * 6 + 1]) = ( ( (kc->u.narrow[2 * 6 + 1]) << 3) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 6 + 1]) >> (32 - 3) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 23 + 0]) << (22 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 23 + 0]) >> (33 - 22) );
                                (kc->u.narrow[2 * 23 + 0]) = ( ( (kc->u.narrow[2 * 23 + 1]) << 22) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 23 + 1]) >> (32 - 22) );
                                (kc->u.narrow[2 * 23 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 10 + 0]) << (8 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 10 + 0]) >> (33 - 8) );
                                (kc->u.narrow[2 * 10 + 0]) = ( ( (kc->u.narrow[2 * 10 + 1]) << 8) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 10 + 1]) >> (32 - 8) );
                                (kc->u.narrow[2 * 10 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 2 + 0]) << (31 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 2 + 0]) >> (33 - 31) );
                                (kc->u.narrow[2 * 2 + 0]) = ( ( (kc->u.narrow[2 * 2 + 1]) << 31) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 2 + 1]) >> (32 - 31) );
                                (kc->u.narrow[2 * 2 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 11 + 0]) = ( ( (kc->u.narrow[2 * 11 + 0]) << 14) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 11 + 0]) >> (32 - 14) );
                                (kc->u.narrow[2 * 11 + 1]) = ( ( (kc->u.narrow[2 * 11 + 1]) << 14) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 11 + 1]) >> (32 - 14) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 3 + 0]) << (28 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 3 + 0]) >> (33 - 28) );
                                (kc->u.narrow[2 * 3 + 0]) = ( ( (kc->u.narrow[2 * 3 + 1]) << 28) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 3 + 1]) >> (32 - 28) );
                                (kc->u.narrow[2 * 3 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 15 + 0]) << (13 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 15 + 0]) >> (33 - 13) );
                                (kc->u.narrow[2 * 15 + 0]) = ( ( (kc->u.narrow[2 * 15 + 1]) << 13) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 15 + 1]) >> (32 - 13) );
                                (kc->u.narrow[2 * 15 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 7 + 0]) << (11 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 7 + 0]) >> (33 - 11) );
                                (kc->u.narrow[2 * 7 + 0]) = ( ( (kc->u.narrow[2 * 7 + 1]) << 11) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 7 + 1]) >> (32 - 11) );
                                (kc->u.narrow[2 * 7 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 24 + 0]) = ( ( (kc->u.narrow[2 * 24 + 0]) << 28) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 24 + 0]) >> (32 - 28) );
                                (kc->u.narrow[2 * 24 + 1]) = ( ( (kc->u.narrow[2 * 24 + 1]) << 28) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 24 + 1]) >> (32 - 28) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 8 + 0]) << (14 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 8 + 0]) >> (33 - 14) );
                                (kc->u.narrow[2 * 8 + 0]) = ( ( (kc->u.narrow[2 * 8 + 1]) << 14) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 8 + 1]) >> (32 - 14) );
                                (kc->u.narrow[2 * 8 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 20 + 0]) = ( ( (kc->u.narrow[2 * 20 + 0]) << 10) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 20 + 0]) >> (32 - 10) );
                                (kc->u.narrow[2 * 20 + 1]) = ( ( (kc->u.narrow[2 * 20 + 1]) << 10) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 20 + 1]) >> (32 - 10) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 12 + 0]) << (20 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 12 + 0]) >> (33 - 20) );
                                (kc->u.narrow[2 * 12 + 0]) = ( ( (kc->u.narrow[2 * 12 + 1]) << 20) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 12 + 1]) >> (32 - 20) );
                                (kc->u.narrow[2 * 12 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 4 + 0]) = ( ( (kc->u.narrow[2 * 4 + 0]) << 4) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 4 + 0]) >> (32 - 4) );
                                (kc->u.narrow[2 * 4 + 1]) = ( ( (kc->u.narrow[2 * 4 + 1]) << 4) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 4 + 1]) >> (32 - 4) );
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 16 + 0]) = ( ( (kc->u.narrow[2 * 16 + 0]) << 7) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 16 + 0]) >> (32 - 7) );
                                (kc->u.narrow[2 * 16 + 1]) = ( ( (kc->u.narrow[2 * 16 + 1]) << 7) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 16 + 1]) >> (32 - 7) );
                            }
                            while ( 0 );
                        }
                        while ( 0 );
                        do
                        {
                            sph_u64 c0l, c0h;
                            sph_u64 c1l, c1h;
                            sph_u64 c2l, c2h;
                            sph_u64 c3l, c3h;
                            sph_u64 c4l, c4h;
                            sph_u64 bnnl, bnnh;
                            (bnnl = ( (~(kc->u.narrow[2 * 23 + 0]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ), bnnh = ( (~(kc->u.narrow[2 * 23 + 1]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 14 + 0]) | (kc->u.narrow[2 * 23 + 0]), kth = (kc->u.narrow[2 * 14 + 1]) | (kc->u.narrow[2 * 23 + 1]) );
                                (c0l = (kc->u.narrow[2 * 0 + 0]) ^ ktl, c0h = (kc->u.narrow[2 * 0 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = bnnl | (kc->u.narrow[2 * 7 + 0]), kth = bnnh | (kc->u.narrow[2 * 7 + 1]) );
                                (c1l = (kc->u.narrow[2 * 14 + 0]) ^ ktl, c1h = (kc->u.narrow[2 * 14 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 7 + 0]) & (kc->u.narrow[2 * 16 + 0]), kth = (kc->u.narrow[2 * 7 + 1]) & (kc->u.narrow[2 * 16 + 1]) );
                                (c2l = (kc->u.narrow[2 * 23 + 0]) ^ ktl, c2h = (kc->u.narrow[2 * 23 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 16 + 0]) | (kc->u.narrow[2 * 0 + 0]), kth = (kc->u.narrow[2 * 16 + 1]) | (kc->u.narrow[2 * 0 + 1]) );
                                (c3l = (kc->u.narrow[2 * 7 + 0]) ^ ktl, c3h = (kc->u.narrow[2 * 7 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 0 + 0]) & (kc->u.narrow[2 * 14 + 0]), kth = (kc->u.narrow[2 * 0 + 1]) & (kc->u.narrow[2 * 14 + 1]) );
                                (c4l = (kc->u.narrow[2 * 16 + 0]) ^ ktl, c4h = (kc->u.narrow[2 * 16 + 1]) ^ kth);
                            }
                            while ( 0 );
                            ( (kc->u.narrow[2 * 0 + 0]) = c0l, (kc->u.narrow[2 * 0 + 1]) = c0h);
                            ( (kc->u.narrow[2 * 14 + 0]) = c1l, (kc->u.narrow[2 * 14 + 1]) = c1h);
                            ( (kc->u.narrow[2 * 23 + 0]) = c2l, (kc->u.narrow[2 * 23 + 1]) = c2h);
                            ( (kc->u.narrow[2 * 7 + 0]) = c3l, (kc->u.narrow[2 * 7 + 1]) = c3h);
                            ( (kc->u.narrow[2 * 16 + 0]) = c4l, (kc->u.narrow[2 * 16 + 1]) = c4h);
                            (bnnl = ( (~(kc->u.narrow[2 * 2 + 0]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ), bnnh = ( (~(kc->u.narrow[2 * 2 + 1]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 20 + 0]) | (kc->u.narrow[2 * 9 + 0]), kth = (kc->u.narrow[2 * 20 + 1]) | (kc->u.narrow[2 * 9 + 1]) );
                                (c0l = (kc->u.narrow[2 * 11 + 0]) ^ ktl, c0h = (kc->u.narrow[2 * 11 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 9 + 0]) & (kc->u.narrow[2 * 18 + 0]), kth = (kc->u.narrow[2 * 9 + 1]) & (kc->u.narrow[2 * 18 + 1]) );
                                (c1l = (kc->u.narrow[2 * 20 + 0]) ^ ktl, c1h = (kc->u.narrow[2 * 20 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 18 + 0]) | bnnl, kth = (kc->u.narrow[2 * 18 + 1]) | bnnh);
                                (c2l = (kc->u.narrow[2 * 9 + 0]) ^ ktl, c2h = (kc->u.narrow[2 * 9 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 2 + 0]) | (kc->u.narrow[2 * 11 + 0]), kth = (kc->u.narrow[2 * 2 + 1]) | (kc->u.narrow[2 * 11 + 1]) );
                                (c3l = (kc->u.narrow[2 * 18 + 0]) ^ ktl, c3h = (kc->u.narrow[2 * 18 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 11 + 0]) & (kc->u.narrow[2 * 20 + 0]), kth = (kc->u.narrow[2 * 11 + 1]) & (kc->u.narrow[2 * 20 + 1]) );
                                (c4l = (kc->u.narrow[2 * 2 + 0]) ^ ktl, c4h = (kc->u.narrow[2 * 2 + 1]) ^ kth);
                            }
                            while ( 0 );
                            ( (kc->u.narrow[2 * 11 + 0]) = c0l, (kc->u.narrow[2 * 11 + 1]) = c0h);
                            ( (kc->u.narrow[2 * 20 + 0]) = c1l, (kc->u.narrow[2 * 20 + 1]) = c1h);
                            ( (kc->u.narrow[2 * 9 + 0]) = c2l, (kc->u.narrow[2 * 9 + 1]) = c2h);
                            ( (kc->u.narrow[2 * 18 + 0]) = c3l, (kc->u.narrow[2 * 18 + 1]) = c3h);
                            ( (kc->u.narrow[2 * 2 + 0]) = c4l, (kc->u.narrow[2 * 2 + 1]) = c4h);
                            (bnnl = ( (~(kc->u.narrow[2 * 4 + 0]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ), bnnh = ( (~(kc->u.narrow[2 * 4 + 1]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 6 + 0]) | (kc->u.narrow[2 * 15 + 0]), kth = (kc->u.narrow[2 * 6 + 1]) | (kc->u.narrow[2 * 15 + 1]) );
                                (c0l = (kc->u.narrow[2 * 22 + 0]) ^ ktl, c0h = (kc->u.narrow[2 * 22 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 15 + 0]) & (kc->u.narrow[2 * 4 + 0]), kth = (kc->u.narrow[2 * 15 + 1]) & (kc->u.narrow[2 * 4 + 1]) );
                                (c1l = (kc->u.narrow[2 * 6 + 0]) ^ ktl, c1h = (kc->u.narrow[2 * 6 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = bnnl & (kc->u.narrow[2 * 13 + 0]), kth = bnnh & (kc->u.narrow[2 * 13 + 1]) );
                                (c2l = (kc->u.narrow[2 * 15 + 0]) ^ ktl, c2h = (kc->u.narrow[2 * 15 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 13 + 0]) | (kc->u.narrow[2 * 22 + 0]), kth = (kc->u.narrow[2 * 13 + 1]) | (kc->u.narrow[2 * 22 + 1]) );
                                (c3l = bnnl ^ ktl, c3h = bnnh ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 22 + 0]) & (kc->u.narrow[2 * 6 + 0]), kth = (kc->u.narrow[2 * 22 + 1]) & (kc->u.narrow[2 * 6 + 1]) );
                                (c4l = (kc->u.narrow[2 * 13 + 0]) ^ ktl, c4h = (kc->u.narrow[2 * 13 + 1]) ^ kth);
                            }
                            while ( 0 );
                            ( (kc->u.narrow[2 * 22 + 0]) = c0l, (kc->u.narrow[2 * 22 + 1]) = c0h);
                            ( (kc->u.narrow[2 * 6 + 0]) = c1l, (kc->u.narrow[2 * 6 + 1]) = c1h);
                            ( (kc->u.narrow[2 * 15 + 0]) = c2l, (kc->u.narrow[2 * 15 + 1]) = c2h);
                            ( (kc->u.narrow[2 * 4 + 0]) = c3l, (kc->u.narrow[2 * 4 + 1]) = c3h);
                            ( (kc->u.narrow[2 * 13 + 0]) = c4l, (kc->u.narrow[2 * 13 + 1]) = c4h);
                            (bnnl = ( (~(kc->u.narrow[2 * 10 + 0]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ), bnnh = ( (~(kc->u.narrow[2 * 10 + 1]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 17 + 0]) & (kc->u.narrow[2 * 1 + 0]), kth = (kc->u.narrow[2 * 17 + 1]) & (kc->u.narrow[2 * 1 + 1]) );
                                (c0l = (kc->u.narrow[2 * 8 + 0]) ^ ktl, c0h = (kc->u.narrow[2 * 8 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 1 + 0]) | (kc->u.narrow[2 * 10 + 0]), kth = (kc->u.narrow[2 * 1 + 1]) | (kc->u.narrow[2 * 10 + 1]) );
                                (c1l = (kc->u.narrow[2 * 17 + 0]) ^ ktl, c1h = (kc->u.narrow[2 * 17 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = bnnl | (kc->u.narrow[2 * 24 + 0]), kth = bnnh | (kc->u.narrow[2 * 24 + 1]) );
                                (c2l = (kc->u.narrow[2 * 1 + 0]) ^ ktl, c2h = (kc->u.narrow[2 * 1 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 24 + 0]) & (kc->u.narrow[2 * 8 + 0]), kth = (kc->u.narrow[2 * 24 + 1]) & (kc->u.narrow[2 * 8 + 1]) );
                                (c3l = bnnl ^ ktl, c3h = bnnh ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 8 + 0]) | (kc->u.narrow[2 * 17 + 0]), kth = (kc->u.narrow[2 * 8 + 1]) | (kc->u.narrow[2 * 17 + 1]) );
                                (c4l = (kc->u.narrow[2 * 24 + 0]) ^ ktl, c4h = (kc->u.narrow[2 * 24 + 1]) ^ kth);
                            }
                            while ( 0 );
                            ( (kc->u.narrow[2 * 8 + 0]) = c0l, (kc->u.narrow[2 * 8 + 1]) = c0h);
                            ( (kc->u.narrow[2 * 17 + 0]) = c1l, (kc->u.narrow[2 * 17 + 1]) = c1h);
                            ( (kc->u.narrow[2 * 1 + 0]) = c2l, (kc->u.narrow[2 * 1 + 1]) = c2h);
                            ( (kc->u.narrow[2 * 10 + 0]) = c3l, (kc->u.narrow[2 * 10 + 1]) = c3h);
                            ( (kc->u.narrow[2 * 24 + 0]) = c4l, (kc->u.narrow[2 * 24 + 1]) = c4h);
                            (bnnl = ( (~(kc->u.narrow[2 * 3 + 0]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ), bnnh = ( (~(kc->u.narrow[2 * 3 + 1]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = bnnl & (kc->u.narrow[2 * 12 + 0]), kth = bnnh & (kc->u.narrow[2 * 12 + 1]) );
                                (c0l = (kc->u.narrow[2 * 19 + 0]) ^ ktl, c0h = (kc->u.narrow[2 * 19 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 12 + 0]) | (kc->u.narrow[2 * 21 + 0]), kth = (kc->u.narrow[2 * 12 + 1]) | (kc->u.narrow[2 * 21 + 1]) );
                                (c1l = bnnl ^ ktl, c1h = bnnh ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 21 + 0]) & (kc->u.narrow[2 * 5 + 0]), kth = (kc->u.narrow[2 * 21 + 1]) & (kc->u.narrow[2 * 5 + 1]) );
                                (c2l = (kc->u.narrow[2 * 12 + 0]) ^ ktl, c2h = (kc->u.narrow[2 * 12 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 5 + 0]) | (kc->u.narrow[2 * 19 + 0]), kth = (kc->u.narrow[2 * 5 + 1]) | (kc->u.narrow[2 * 19 + 1]) );
                                (c3l = (kc->u.narrow[2 * 21 + 0]) ^ ktl, c3h = (kc->u.narrow[2 * 21 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 19 + 0]) & (kc->u.narrow[2 * 3 + 0]), kth = (kc->u.narrow[2 * 19 + 1]) & (kc->u.narrow[2 * 3 + 1]) );
                                (c4l = (kc->u.narrow[2 * 5 + 0]) ^ ktl, c4h = (kc->u.narrow[2 * 5 + 1]) ^ kth);
                            }
                            while ( 0 );
                            ( (kc->u.narrow[2 * 19 + 0]) = c0l, (kc->u.narrow[2 * 19 + 1]) = c0h);
                            ( (kc->u.narrow[2 * 3 + 0]) = c1l, (kc->u.narrow[2 * 3 + 1]) = c1h);
                            ( (kc->u.narrow[2 * 12 + 0]) = c2l, (kc->u.narrow[2 * 12 + 1]) = c2h);
                            ( (kc->u.narrow[2 * 21 + 0]) = c3l, (kc->u.narrow[2 * 21 + 1]) = c3h);
                            ( (kc->u.narrow[2 * 5 + 0]) = c4l, (kc->u.narrow[2 * 5 + 1]) = c4h);
                        }
                        while ( 0 );
                        ( (kc->u.narrow[2 * 0 + 0]) = (kc->u.narrow[2 * 0 + 0]) ^ RC[j + 3].low, (kc->u.narrow[2 * 0 + 1]) = (kc->u.narrow[2 * 0 + 1]) ^ RC[j + 3].high);
                    }
                    while ( 0 );
                    if(out)
                    {
                        printf( "\n第%d轮中间状态：\n",k++ );
                        print_hex(kc->u.narrow, 1600);
                    }
                    do
                    {
                        do
                        {
                            sph_u64 t0l, t0h;
                            sph_u64 t1l, t1h;
                            sph_u64 t2l, t2h;
                            sph_u64 t3l, t3h;
                            sph_u64 t4l, t4h;
                            do
                            {
                                sph_u64 tt0l, tt0h;
                                sph_u64 tt1l, tt1h;
                                sph_u64 tt2l, tt2h;
                                sph_u64 tt3l, tt3h;
                                (tt0l = (kc->u.narrow[2 * 14 + 0]) ^ (kc->u.narrow[2 * 20 + 0]), tt0h = (kc->u.narrow[2 * 14 + 1]) ^ (kc->u.narrow[2 * 20 + 1]) );
                                (tt1l = (kc->u.narrow[2 * 6 + 0]) ^ (kc->u.narrow[2 * 17 + 0]), tt1h = (kc->u.narrow[2 * 6 + 1]) ^ (kc->u.narrow[2 * 17 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 3 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 3 + 1]) );
                                (tt0l = tt0l ^ tt1l, tt0h = tt0h ^ tt1h);
                                do
                                {
                                    sph_u32 tmp;
                                    tmp = tt0l;
                                    tt0l = ( (tt0h << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | (tt0h >> 31);
                                    tt0h = tmp;
                                }
                                while ( 0 );
                                (tt2l = (kc->u.narrow[2 * 16 + 0]) ^ (kc->u.narrow[2 * 2 + 0]), tt2h = (kc->u.narrow[2 * 16 + 1]) ^ (kc->u.narrow[2 * 2 + 1]) );
                                (tt3l = (kc->u.narrow[2 * 13 + 0]) ^ (kc->u.narrow[2 * 24 + 0]), tt3h = (kc->u.narrow[2 * 13 + 1]) ^ (kc->u.narrow[2 * 24 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 5 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 5 + 1]) );
                                (tt2l = tt2l ^ tt3l, tt2h = tt2h ^ tt3h);
                                (t0l = tt0l ^ tt2l, t0h = tt0h ^ tt2h);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 tt0l, tt0h;
                                sph_u64 tt1l, tt1h;
                                sph_u64 tt2l, tt2h;
                                sph_u64 tt3l, tt3h;
                                (tt0l = (kc->u.narrow[2 * 23 + 0]) ^ (kc->u.narrow[2 * 9 + 0]), tt0h = (kc->u.narrow[2 * 23 + 1]) ^ (kc->u.narrow[2 * 9 + 1]) );
                                (tt1l = (kc->u.narrow[2 * 15 + 0]) ^ (kc->u.narrow[2 * 1 + 0]), tt1h = (kc->u.narrow[2 * 15 + 1]) ^ (kc->u.narrow[2 * 1 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 12 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 12 + 1]) );
                                (tt0l = tt0l ^ tt1l, tt0h = tt0h ^ tt1h);
                                do
                                {
                                    sph_u32 tmp;
                                    tmp = tt0l;
                                    tt0l = ( (tt0h << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | (tt0h >> 31);
                                    tt0h = tmp;
                                }
                                while ( 0 );
                                (tt2l = (kc->u.narrow[2 * 0 + 0]) ^ (kc->u.narrow[2 * 11 + 0]), tt2h = (kc->u.narrow[2 * 0 + 1]) ^ (kc->u.narrow[2 * 11 + 1]) );
                                (tt3l = (kc->u.narrow[2 * 22 + 0]) ^ (kc->u.narrow[2 * 8 + 0]), tt3h = (kc->u.narrow[2 * 22 + 1]) ^ (kc->u.narrow[2 * 8 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 19 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 19 + 1]) );
                                (tt2l = tt2l ^ tt3l, tt2h = tt2h ^ tt3h);
                                (t1l = tt0l ^ tt2l, t1h = tt0h ^ tt2h);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 tt0l, tt0h;
                                sph_u64 tt1l, tt1h;
                                sph_u64 tt2l, tt2h;
                                sph_u64 tt3l, tt3h;
                                (tt0l = (kc->u.narrow[2 * 7 + 0]) ^ (kc->u.narrow[2 * 18 + 0]), tt0h = (kc->u.narrow[2 * 7 + 1]) ^ (kc->u.narrow[2 * 18 + 1]) );
                                (tt1l = (kc->u.narrow[2 * 4 + 0]) ^ (kc->u.narrow[2 * 10 + 0]), tt1h = (kc->u.narrow[2 * 4 + 1]) ^ (kc->u.narrow[2 * 10 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 21 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 21 + 1]) );
                                (tt0l = tt0l ^ tt1l, tt0h = tt0h ^ tt1h);
                                do
                                {
                                    sph_u32 tmp;
                                    tmp = tt0l;
                                    tt0l = ( (tt0h << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | (tt0h >> 31);
                                    tt0h = tmp;
                                }
                                while ( 0 );
                                (tt2l = (kc->u.narrow[2 * 14 + 0]) ^ (kc->u.narrow[2 * 20 + 0]), tt2h = (kc->u.narrow[2 * 14 + 1]) ^ (kc->u.narrow[2 * 20 + 1]) );
                                (tt3l = (kc->u.narrow[2 * 6 + 0]) ^ (kc->u.narrow[2 * 17 + 0]), tt3h = (kc->u.narrow[2 * 6 + 1]) ^ (kc->u.narrow[2 * 17 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 3 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 3 + 1]) );
                                (tt2l = tt2l ^ tt3l, tt2h = tt2h ^ tt3h);
                                (t2l = tt0l ^ tt2l, t2h = tt0h ^ tt2h);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 tt0l, tt0h;
                                sph_u64 tt1l, tt1h;
                                sph_u64 tt2l, tt2h;
                                sph_u64 tt3l, tt3h;
                                (tt0l = (kc->u.narrow[2 * 16 + 0]) ^ (kc->u.narrow[2 * 2 + 0]), tt0h = (kc->u.narrow[2 * 16 + 1]) ^ (kc->u.narrow[2 * 2 + 1]) );
                                (tt1l = (kc->u.narrow[2 * 13 + 0]) ^ (kc->u.narrow[2 * 24 + 0]), tt1h = (kc->u.narrow[2 * 13 + 1]) ^ (kc->u.narrow[2 * 24 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 5 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 5 + 1]) );
                                (tt0l = tt0l ^ tt1l, tt0h = tt0h ^ tt1h);
                                do
                                {
                                    sph_u32 tmp;
                                    tmp = tt0l;
                                    tt0l = ( (tt0h << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | (tt0h >> 31);
                                    tt0h = tmp;
                                }
                                while ( 0 );
                                (tt2l = (kc->u.narrow[2 * 23 + 0]) ^ (kc->u.narrow[2 * 9 + 0]), tt2h = (kc->u.narrow[2 * 23 + 1]) ^ (kc->u.narrow[2 * 9 + 1]) );
                                (tt3l = (kc->u.narrow[2 * 15 + 0]) ^ (kc->u.narrow[2 * 1 + 0]), tt3h = (kc->u.narrow[2 * 15 + 1]) ^ (kc->u.narrow[2 * 1 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 12 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 12 + 1]) );
                                (tt2l = tt2l ^ tt3l, tt2h = tt2h ^ tt3h);
                                (t3l = tt0l ^ tt2l, t3h = tt0h ^ tt2h);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 tt0l, tt0h;
                                sph_u64 tt1l, tt1h;
                                sph_u64 tt2l, tt2h;
                                sph_u64 tt3l, tt3h;
                                (tt0l = (kc->u.narrow[2 * 0 + 0]) ^ (kc->u.narrow[2 * 11 + 0]), tt0h = (kc->u.narrow[2 * 0 + 1]) ^ (kc->u.narrow[2 * 11 + 1]) );
                                (tt1l = (kc->u.narrow[2 * 22 + 0]) ^ (kc->u.narrow[2 * 8 + 0]), tt1h = (kc->u.narrow[2 * 22 + 1]) ^ (kc->u.narrow[2 * 8 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 19 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 19 + 1]) );
                                (tt0l = tt0l ^ tt1l, tt0h = tt0h ^ tt1h);
                                do
                                {
                                    sph_u32 tmp;
                                    tmp = tt0l;
                                    tt0l = ( (tt0h << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | (tt0h >> 31);
                                    tt0h = tmp;
                                }
                                while ( 0 );
                                (tt2l = (kc->u.narrow[2 * 7 + 0]) ^ (kc->u.narrow[2 * 18 + 0]), tt2h = (kc->u.narrow[2 * 7 + 1]) ^ (kc->u.narrow[2 * 18 + 1]) );
                                (tt3l = (kc->u.narrow[2 * 4 + 0]) ^ (kc->u.narrow[2 * 10 + 0]), tt3h = (kc->u.narrow[2 * 4 + 1]) ^ (kc->u.narrow[2 * 10 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 21 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 21 + 1]) );
                                (tt2l = tt2l ^ tt3l, tt2h = tt2h ^ tt3h);
                                (t4l = tt0l ^ tt2l, t4h = tt0h ^ tt2h);
                            }
                            while ( 0 );
                            ( (kc->u.narrow[2 * 0 + 0]) = (kc->u.narrow[2 * 0 + 0]) ^ t0l, (kc->u.narrow[2 * 0 + 1]) = (kc->u.narrow[2 * 0 + 1]) ^ t0h);
                            ( (kc->u.narrow[2 * 11 + 0]) = (kc->u.narrow[2 * 11 + 0]) ^ t0l, (kc->u.narrow[2 * 11 + 1]) = (kc->u.narrow[2 * 11 + 1]) ^ t0h);
                            ( (kc->u.narrow[2 * 22 + 0]) = (kc->u.narrow[2 * 22 + 0]) ^ t0l, (kc->u.narrow[2 * 22 + 1]) = (kc->u.narrow[2 * 22 + 1]) ^ t0h);
                            ( (kc->u.narrow[2 * 8 + 0]) = (kc->u.narrow[2 * 8 + 0]) ^ t0l, (kc->u.narrow[2 * 8 + 1]) = (kc->u.narrow[2 * 8 + 1]) ^ t0h);
                            ( (kc->u.narrow[2 * 19 + 0]) = (kc->u.narrow[2 * 19 + 0]) ^ t0l, (kc->u.narrow[2 * 19 + 1]) = (kc->u.narrow[2 * 19 + 1]) ^ t0h);
                            ( (kc->u.narrow[2 * 14 + 0]) = (kc->u.narrow[2 * 14 + 0]) ^ t1l, (kc->u.narrow[2 * 14 + 1]) = (kc->u.narrow[2 * 14 + 1]) ^ t1h);
                            ( (kc->u.narrow[2 * 20 + 0]) = (kc->u.narrow[2 * 20 + 0]) ^ t1l, (kc->u.narrow[2 * 20 + 1]) = (kc->u.narrow[2 * 20 + 1]) ^ t1h);
                            ( (kc->u.narrow[2 * 6 + 0]) = (kc->u.narrow[2 * 6 + 0]) ^ t1l, (kc->u.narrow[2 * 6 + 1]) = (kc->u.narrow[2 * 6 + 1]) ^ t1h);
                            ( (kc->u.narrow[2 * 17 + 0]) = (kc->u.narrow[2 * 17 + 0]) ^ t1l, (kc->u.narrow[2 * 17 + 1]) = (kc->u.narrow[2 * 17 + 1]) ^ t1h);
                            ( (kc->u.narrow[2 * 3 + 0]) = (kc->u.narrow[2 * 3 + 0]) ^ t1l, (kc->u.narrow[2 * 3 + 1]) = (kc->u.narrow[2 * 3 + 1]) ^ t1h);
                            ( (kc->u.narrow[2 * 23 + 0]) = (kc->u.narrow[2 * 23 + 0]) ^ t2l, (kc->u.narrow[2 * 23 + 1]) = (kc->u.narrow[2 * 23 + 1]) ^ t2h);
                            ( (kc->u.narrow[2 * 9 + 0]) = (kc->u.narrow[2 * 9 + 0]) ^ t2l, (kc->u.narrow[2 * 9 + 1]) = (kc->u.narrow[2 * 9 + 1]) ^ t2h);
                            ( (kc->u.narrow[2 * 15 + 0]) = (kc->u.narrow[2 * 15 + 0]) ^ t2l, (kc->u.narrow[2 * 15 + 1]) = (kc->u.narrow[2 * 15 + 1]) ^ t2h);
                            ( (kc->u.narrow[2 * 1 + 0]) = (kc->u.narrow[2 * 1 + 0]) ^ t2l, (kc->u.narrow[2 * 1 + 1]) = (kc->u.narrow[2 * 1 + 1]) ^ t2h);
                            ( (kc->u.narrow[2 * 12 + 0]) = (kc->u.narrow[2 * 12 + 0]) ^ t2l, (kc->u.narrow[2 * 12 + 1]) = (kc->u.narrow[2 * 12 + 1]) ^ t2h);
                            ( (kc->u.narrow[2 * 7 + 0]) = (kc->u.narrow[2 * 7 + 0]) ^ t3l, (kc->u.narrow[2 * 7 + 1]) = (kc->u.narrow[2 * 7 + 1]) ^ t3h);
                            ( (kc->u.narrow[2 * 18 + 0]) = (kc->u.narrow[2 * 18 + 0]) ^ t3l, (kc->u.narrow[2 * 18 + 1]) = (kc->u.narrow[2 * 18 + 1]) ^ t3h);
                            ( (kc->u.narrow[2 * 4 + 0]) = (kc->u.narrow[2 * 4 + 0]) ^ t3l, (kc->u.narrow[2 * 4 + 1]) = (kc->u.narrow[2 * 4 + 1]) ^ t3h);
                            ( (kc->u.narrow[2 * 10 + 0]) = (kc->u.narrow[2 * 10 + 0]) ^ t3l, (kc->u.narrow[2 * 10 + 1]) = (kc->u.narrow[2 * 10 + 1]) ^ t3h);
                            ( (kc->u.narrow[2 * 21 + 0]) = (kc->u.narrow[2 * 21 + 0]) ^ t3l, (kc->u.narrow[2 * 21 + 1]) = (kc->u.narrow[2 * 21 + 1]) ^ t3h);
                            ( (kc->u.narrow[2 * 16 + 0]) = (kc->u.narrow[2 * 16 + 0]) ^ t4l, (kc->u.narrow[2 * 16 + 1]) = (kc->u.narrow[2 * 16 + 1]) ^ t4h);
                            ( (kc->u.narrow[2 * 2 + 0]) = (kc->u.narrow[2 * 2 + 0]) ^ t4l, (kc->u.narrow[2 * 2 + 1]) = (kc->u.narrow[2 * 2 + 1]) ^ t4h);
                            ( (kc->u.narrow[2 * 13 + 0]) = (kc->u.narrow[2 * 13 + 0]) ^ t4l, (kc->u.narrow[2 * 13 + 1]) = (kc->u.narrow[2 * 13 + 1]) ^ t4h);
                            ( (kc->u.narrow[2 * 24 + 0]) = (kc->u.narrow[2 * 24 + 0]) ^ t4l, (kc->u.narrow[2 * 24 + 1]) = (kc->u.narrow[2 * 24 + 1]) ^ t4h);
                            ( (kc->u.narrow[2 * 5 + 0]) = (kc->u.narrow[2 * 5 + 0]) ^ t4l, (kc->u.narrow[2 * 5 + 1]) = (kc->u.narrow[2 * 5 + 1]) ^ t4h);
                        }
                        while ( 0 );
                        do
                        {
                            do
                            {
                                (kc->u.narrow[2 * 11 + 0]) = ( ( (kc->u.narrow[2 * 11 + 0]) << 18) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 11 + 0]) >> (32 - 18) );
                                (kc->u.narrow[2 * 11 + 1]) = ( ( (kc->u.narrow[2 * 11 + 1]) << 18) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 11 + 1]) >> (32 - 18) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 22 + 0]) << (2 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 22 + 0]) >> (33 - 2) );
                                (kc->u.narrow[2 * 22 + 0]) = ( ( (kc->u.narrow[2 * 22 + 1]) << 2) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 22 + 1]) >> (32 - 2) );
                                (kc->u.narrow[2 * 22 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 8 + 0]) << (21 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 8 + 0]) >> (33 - 21) );
                                (kc->u.narrow[2 * 8 + 0]) = ( ( (kc->u.narrow[2 * 8 + 1]) << 21) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 8 + 1]) >> (32 - 21) );
                                (kc->u.narrow[2 * 8 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 19 + 0]) = ( ( (kc->u.narrow[2 * 19 + 0]) << 9) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 19 + 0]) >> (32 - 9) );
                                (kc->u.narrow[2 * 19 + 1]) = ( ( (kc->u.narrow[2 * 19 + 1]) << 9) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 19 + 1]) >> (32 - 9) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = (kc->u.narrow[2 * 14 + 0]);
                                (kc->u.narrow[2 * 14 + 0]) = ( ( (kc->u.narrow[2 * 14 + 1]) << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 14 + 1]) >> 31);
                                (kc->u.narrow[2 * 14 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 20 + 0]) = ( ( (kc->u.narrow[2 * 20 + 0]) << 22) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 20 + 0]) >> (32 - 22) );
                                (kc->u.narrow[2 * 20 + 1]) = ( ( (kc->u.narrow[2 * 20 + 1]) << 22) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 20 + 1]) >> (32 - 22) );
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 6 + 0]) = ( ( (kc->u.narrow[2 * 6 + 0]) << 5) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 6 + 0]) >> (32 - 5) );
                                (kc->u.narrow[2 * 6 + 1]) = ( ( (kc->u.narrow[2 * 6 + 1]) << 5) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 6 + 1]) >> (32 - 5) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 17 + 0]) << (23 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 17 + 0]) >> (33 - 23) );
                                (kc->u.narrow[2 * 17 + 0]) = ( ( (kc->u.narrow[2 * 17 + 1]) << 23) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 17 + 1]) >> (32 - 23) );
                                (kc->u.narrow[2 * 17 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 3 + 0]) = ( ( (kc->u.narrow[2 * 3 + 0]) << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 3 + 0]) >> (32 - 1) );
                                (kc->u.narrow[2 * 3 + 1]) = ( ( (kc->u.narrow[2 * 3 + 1]) << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 3 + 1]) >> (32 - 1) );
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 23 + 0]) = ( ( (kc->u.narrow[2 * 23 + 0]) << 31) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 23 + 0]) >> (32 - 31) );
                                (kc->u.narrow[2 * 23 + 1]) = ( ( (kc->u.narrow[2 * 23 + 1]) << 31) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 23 + 1]) >> (32 - 31) );
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 9 + 0]) = ( ( (kc->u.narrow[2 * 9 + 0]) << 3) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 9 + 0]) >> (32 - 3) );
                                (kc->u.narrow[2 * 9 + 1]) = ( ( (kc->u.narrow[2 * 9 + 1]) << 3) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 9 + 1]) >> (32 - 3) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 15 + 0]) << (22 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 15 + 0]) >> (33 - 22) );
                                (kc->u.narrow[2 * 15 + 0]) = ( ( (kc->u.narrow[2 * 15 + 1]) << 22) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 15 + 1]) >> (32 - 22) );
                                (kc->u.narrow[2 * 15 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 1 + 0]) << (8 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 1 + 0]) >> (33 - 8) );
                                (kc->u.narrow[2 * 1 + 0]) = ( ( (kc->u.narrow[2 * 1 + 1]) << 8) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 1 + 1]) >> (32 - 8) );
                                (kc->u.narrow[2 * 1 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 12 + 0]) << (31 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 12 + 0]) >> (33 - 31) );
                                (kc->u.narrow[2 * 12 + 0]) = ( ( (kc->u.narrow[2 * 12 + 1]) << 31) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 12 + 1]) >> (32 - 31) );
                                (kc->u.narrow[2 * 12 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 7 + 0]) = ( ( (kc->u.narrow[2 * 7 + 0]) << 14) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 7 + 0]) >> (32 - 14) );
                                (kc->u.narrow[2 * 7 + 1]) = ( ( (kc->u.narrow[2 * 7 + 1]) << 14) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 7 + 1]) >> (32 - 14) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 18 + 0]) << (28 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 18 + 0]) >> (33 - 28) );
                                (kc->u.narrow[2 * 18 + 0]) = ( ( (kc->u.narrow[2 * 18 + 1]) << 28) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 18 + 1]) >> (32 - 28) );
                                (kc->u.narrow[2 * 18 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 4 + 0]) << (13 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 4 + 0]) >> (33 - 13) );
                                (kc->u.narrow[2 * 4 + 0]) = ( ( (kc->u.narrow[2 * 4 + 1]) << 13) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 4 + 1]) >> (32 - 13) );
                                (kc->u.narrow[2 * 4 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 10 + 0]) << (11 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 10 + 0]) >> (33 - 11) );
                                (kc->u.narrow[2 * 10 + 0]) = ( ( (kc->u.narrow[2 * 10 + 1]) << 11) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 10 + 1]) >> (32 - 11) );
                                (kc->u.narrow[2 * 10 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 21 + 0]) = ( ( (kc->u.narrow[2 * 21 + 0]) << 28) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 21 + 0]) >> (32 - 28) );
                                (kc->u.narrow[2 * 21 + 1]) = ( ( (kc->u.narrow[2 * 21 + 1]) << 28) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 21 + 1]) >> (32 - 28) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 16 + 0]) << (14 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 16 + 0]) >> (33 - 14) );
                                (kc->u.narrow[2 * 16 + 0]) = ( ( (kc->u.narrow[2 * 16 + 1]) << 14) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 16 + 1]) >> (32 - 14) );
                                (kc->u.narrow[2 * 16 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 2 + 0]) = ( ( (kc->u.narrow[2 * 2 + 0]) << 10) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 2 + 0]) >> (32 - 10) );
                                (kc->u.narrow[2 * 2 + 1]) = ( ( (kc->u.narrow[2 * 2 + 1]) << 10) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 2 + 1]) >> (32 - 10) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 13 + 0]) << (20 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 13 + 0]) >> (33 - 20) );
                                (kc->u.narrow[2 * 13 + 0]) = ( ( (kc->u.narrow[2 * 13 + 1]) << 20) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 13 + 1]) >> (32 - 20) );
                                (kc->u.narrow[2 * 13 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 24 + 0]) = ( ( (kc->u.narrow[2 * 24 + 0]) << 4) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 24 + 0]) >> (32 - 4) );
                                (kc->u.narrow[2 * 24 + 1]) = ( ( (kc->u.narrow[2 * 24 + 1]) << 4) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 24 + 1]) >> (32 - 4) );
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 5 + 0]) = ( ( (kc->u.narrow[2 * 5 + 0]) << 7) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 5 + 0]) >> (32 - 7) );
                                (kc->u.narrow[2 * 5 + 1]) = ( ( (kc->u.narrow[2 * 5 + 1]) << 7) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 5 + 1]) >> (32 - 7) );
                            }
                            while ( 0 );
                        }
                        while ( 0 );
                        do
                        {
                            sph_u64 c0l, c0h;
                            sph_u64 c1l, c1h;
                            sph_u64 c2l, c2h;
                            sph_u64 c3l, c3h;
                            sph_u64 c4l, c4h;
                            sph_u64 bnnl, bnnh;
                            (bnnl = ( (~(kc->u.narrow[2 * 15 + 0]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ), bnnh = ( (~(kc->u.narrow[2 * 15 + 1]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 20 + 0]) | (kc->u.narrow[2 * 15 + 0]), kth = (kc->u.narrow[2 * 20 + 1]) | (kc->u.narrow[2 * 15 + 1]) );
                                (c0l = (kc->u.narrow[2 * 0 + 0]) ^ ktl, c0h = (kc->u.narrow[2 * 0 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = bnnl | (kc->u.narrow[2 * 10 + 0]), kth = bnnh | (kc->u.narrow[2 * 10 + 1]) );
                                (c1l = (kc->u.narrow[2 * 20 + 0]) ^ ktl, c1h = (kc->u.narrow[2 * 20 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 10 + 0]) & (kc->u.narrow[2 * 5 + 0]), kth = (kc->u.narrow[2 * 10 + 1]) & (kc->u.narrow[2 * 5 + 1]) );
                                (c2l = (kc->u.narrow[2 * 15 + 0]) ^ ktl, c2h = (kc->u.narrow[2 * 15 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 5 + 0]) | (kc->u.narrow[2 * 0 + 0]), kth = (kc->u.narrow[2 * 5 + 1]) | (kc->u.narrow[2 * 0 + 1]) );
                                (c3l = (kc->u.narrow[2 * 10 + 0]) ^ ktl, c3h = (kc->u.narrow[2 * 10 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 0 + 0]) & (kc->u.narrow[2 * 20 + 0]), kth = (kc->u.narrow[2 * 0 + 1]) & (kc->u.narrow[2 * 20 + 1]) );
                                (c4l = (kc->u.narrow[2 * 5 + 0]) ^ ktl, c4h = (kc->u.narrow[2 * 5 + 1]) ^ kth);
                            }
                            while ( 0 );
                            ( (kc->u.narrow[2 * 0 + 0]) = c0l, (kc->u.narrow[2 * 0 + 1]) = c0h);
                            ( (kc->u.narrow[2 * 20 + 0]) = c1l, (kc->u.narrow[2 * 20 + 1]) = c1h);
                            ( (kc->u.narrow[2 * 15 + 0]) = c2l, (kc->u.narrow[2 * 15 + 1]) = c2h);
                            ( (kc->u.narrow[2 * 10 + 0]) = c3l, (kc->u.narrow[2 * 10 + 1]) = c3h);
                            ( (kc->u.narrow[2 * 5 + 0]) = c4l, (kc->u.narrow[2 * 5 + 1]) = c4h);
                            (bnnl = ( (~(kc->u.narrow[2 * 12 + 0]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ), bnnh = ( (~(kc->u.narrow[2 * 12 + 1]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 2 + 0]) | (kc->u.narrow[2 * 22 + 0]), kth = (kc->u.narrow[2 * 2 + 1]) | (kc->u.narrow[2 * 22 + 1]) );
                                (c0l = (kc->u.narrow[2 * 7 + 0]) ^ ktl, c0h = (kc->u.narrow[2 * 7 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 22 + 0]) & (kc->u.narrow[2 * 17 + 0]), kth = (kc->u.narrow[2 * 22 + 1]) & (kc->u.narrow[2 * 17 + 1]) );
                                (c1l = (kc->u.narrow[2 * 2 + 0]) ^ ktl, c1h = (kc->u.narrow[2 * 2 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 17 + 0]) | bnnl, kth = (kc->u.narrow[2 * 17 + 1]) | bnnh);
                                (c2l = (kc->u.narrow[2 * 22 + 0]) ^ ktl, c2h = (kc->u.narrow[2 * 22 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 12 + 0]) | (kc->u.narrow[2 * 7 + 0]), kth = (kc->u.narrow[2 * 12 + 1]) | (kc->u.narrow[2 * 7 + 1]) );
                                (c3l = (kc->u.narrow[2 * 17 + 0]) ^ ktl, c3h = (kc->u.narrow[2 * 17 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 7 + 0]) & (kc->u.narrow[2 * 2 + 0]), kth = (kc->u.narrow[2 * 7 + 1]) & (kc->u.narrow[2 * 2 + 1]) );
                                (c4l = (kc->u.narrow[2 * 12 + 0]) ^ ktl, c4h = (kc->u.narrow[2 * 12 + 1]) ^ kth);
                            }
                            while ( 0 );
                            ( (kc->u.narrow[2 * 7 + 0]) = c0l, (kc->u.narrow[2 * 7 + 1]) = c0h);
                            ( (kc->u.narrow[2 * 2 + 0]) = c1l, (kc->u.narrow[2 * 2 + 1]) = c1h);
                            ( (kc->u.narrow[2 * 22 + 0]) = c2l, (kc->u.narrow[2 * 22 + 1]) = c2h);
                            ( (kc->u.narrow[2 * 17 + 0]) = c3l, (kc->u.narrow[2 * 17 + 1]) = c3h);
                            ( (kc->u.narrow[2 * 12 + 0]) = c4l, (kc->u.narrow[2 * 12 + 1]) = c4h);
                            (bnnl = ( (~(kc->u.narrow[2 * 24 + 0]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ), bnnh = ( (~(kc->u.narrow[2 * 24 + 1]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 9 + 0]) | (kc->u.narrow[2 * 4 + 0]), kth = (kc->u.narrow[2 * 9 + 1]) | (kc->u.narrow[2 * 4 + 1]) );
                                (c0l = (kc->u.narrow[2 * 14 + 0]) ^ ktl, c0h = (kc->u.narrow[2 * 14 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 4 + 0]) & (kc->u.narrow[2 * 24 + 0]), kth = (kc->u.narrow[2 * 4 + 1]) & (kc->u.narrow[2 * 24 + 1]) );
                                (c1l = (kc->u.narrow[2 * 9 + 0]) ^ ktl, c1h = (kc->u.narrow[2 * 9 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = bnnl & (kc->u.narrow[2 * 19 + 0]), kth = bnnh & (kc->u.narrow[2 * 19 + 1]) );
                                (c2l = (kc->u.narrow[2 * 4 + 0]) ^ ktl, c2h = (kc->u.narrow[2 * 4 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 19 + 0]) | (kc->u.narrow[2 * 14 + 0]), kth = (kc->u.narrow[2 * 19 + 1]) | (kc->u.narrow[2 * 14 + 1]) );
                                (c3l = bnnl ^ ktl, c3h = bnnh ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 14 + 0]) & (kc->u.narrow[2 * 9 + 0]), kth = (kc->u.narrow[2 * 14 + 1]) & (kc->u.narrow[2 * 9 + 1]) );
                                (c4l = (kc->u.narrow[2 * 19 + 0]) ^ ktl, c4h = (kc->u.narrow[2 * 19 + 1]) ^ kth);
                            }
                            while ( 0 );
                            ( (kc->u.narrow[2 * 14 + 0]) = c0l, (kc->u.narrow[2 * 14 + 1]) = c0h);
                            ( (kc->u.narrow[2 * 9 + 0]) = c1l, (kc->u.narrow[2 * 9 + 1]) = c1h);
                            ( (kc->u.narrow[2 * 4 + 0]) = c2l, (kc->u.narrow[2 * 4 + 1]) = c2h);
                            ( (kc->u.narrow[2 * 24 + 0]) = c3l, (kc->u.narrow[2 * 24 + 1]) = c3h);
                            ( (kc->u.narrow[2 * 19 + 0]) = c4l, (kc->u.narrow[2 * 19 + 1]) = c4h);
                            (bnnl = ( (~(kc->u.narrow[2 * 1 + 0]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ), bnnh = ( (~(kc->u.narrow[2 * 1 + 1]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 11 + 0]) & (kc->u.narrow[2 * 6 + 0]), kth = (kc->u.narrow[2 * 11 + 1]) & (kc->u.narrow[2 * 6 + 1]) );
                                (c0l = (kc->u.narrow[2 * 16 + 0]) ^ ktl, c0h = (kc->u.narrow[2 * 16 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 6 + 0]) | (kc->u.narrow[2 * 1 + 0]), kth = (kc->u.narrow[2 * 6 + 1]) | (kc->u.narrow[2 * 1 + 1]) );
                                (c1l = (kc->u.narrow[2 * 11 + 0]) ^ ktl, c1h = (kc->u.narrow[2 * 11 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = bnnl | (kc->u.narrow[2 * 21 + 0]), kth = bnnh | (kc->u.narrow[2 * 21 + 1]) );
                                (c2l = (kc->u.narrow[2 * 6 + 0]) ^ ktl, c2h = (kc->u.narrow[2 * 6 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 21 + 0]) & (kc->u.narrow[2 * 16 + 0]), kth = (kc->u.narrow[2 * 21 + 1]) & (kc->u.narrow[2 * 16 + 1]) );
                                (c3l = bnnl ^ ktl, c3h = bnnh ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 16 + 0]) | (kc->u.narrow[2 * 11 + 0]), kth = (kc->u.narrow[2 * 16 + 1]) | (kc->u.narrow[2 * 11 + 1]) );
                                (c4l = (kc->u.narrow[2 * 21 + 0]) ^ ktl, c4h = (kc->u.narrow[2 * 21 + 1]) ^ kth);
                            }
                            while ( 0 );
                            ( (kc->u.narrow[2 * 16 + 0]) = c0l, (kc->u.narrow[2 * 16 + 1]) = c0h);
                            ( (kc->u.narrow[2 * 11 + 0]) = c1l, (kc->u.narrow[2 * 11 + 1]) = c1h);
                            ( (kc->u.narrow[2 * 6 + 0]) = c2l, (kc->u.narrow[2 * 6 + 1]) = c2h);
                            ( (kc->u.narrow[2 * 1 + 0]) = c3l, (kc->u.narrow[2 * 1 + 1]) = c3h);
                            ( (kc->u.narrow[2 * 21 + 0]) = c4l, (kc->u.narrow[2 * 21 + 1]) = c4h);
                            (bnnl = ( (~(kc->u.narrow[2 * 18 + 0]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ), bnnh = ( (~(kc->u.narrow[2 * 18 + 1]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = bnnl & (kc->u.narrow[2 * 13 + 0]), kth = bnnh & (kc->u.narrow[2 * 13 + 1]) );
                                (c0l = (kc->u.narrow[2 * 23 + 0]) ^ ktl, c0h = (kc->u.narrow[2 * 23 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 13 + 0]) | (kc->u.narrow[2 * 8 + 0]), kth = (kc->u.narrow[2 * 13 + 1]) | (kc->u.narrow[2 * 8 + 1]) );
                                (c1l = bnnl ^ ktl, c1h = bnnh ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 8 + 0]) & (kc->u.narrow[2 * 3 + 0]), kth = (kc->u.narrow[2 * 8 + 1]) & (kc->u.narrow[2 * 3 + 1]) );
                                (c2l = (kc->u.narrow[2 * 13 + 0]) ^ ktl, c2h = (kc->u.narrow[2 * 13 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 3 + 0]) | (kc->u.narrow[2 * 23 + 0]), kth = (kc->u.narrow[2 * 3 + 1]) | (kc->u.narrow[2 * 23 + 1]) );
                                (c3l = (kc->u.narrow[2 * 8 + 0]) ^ ktl, c3h = (kc->u.narrow[2 * 8 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 23 + 0]) & (kc->u.narrow[2 * 18 + 0]), kth = (kc->u.narrow[2 * 23 + 1]) & (kc->u.narrow[2 * 18 + 1]) );
                                (c4l = (kc->u.narrow[2 * 3 + 0]) ^ ktl, c4h = (kc->u.narrow[2 * 3 + 1]) ^ kth);
                            }
                            while ( 0 );
                            ( (kc->u.narrow[2 * 23 + 0]) = c0l, (kc->u.narrow[2 * 23 + 1]) = c0h);
                            ( (kc->u.narrow[2 * 18 + 0]) = c1l, (kc->u.narrow[2 * 18 + 1]) = c1h);
                            ( (kc->u.narrow[2 * 13 + 0]) = c2l, (kc->u.narrow[2 * 13 + 1]) = c2h);
                            ( (kc->u.narrow[2 * 8 + 0]) = c3l, (kc->u.narrow[2 * 8 + 1]) = c3h);
                            ( (kc->u.narrow[2 * 3 + 0]) = c4l, (kc->u.narrow[2 * 3 + 1]) = c4h);
                        }
                        while ( 0 );
                        ( (kc->u.narrow[2 * 0 + 0]) = (kc->u.narrow[2 * 0 + 0]) ^ RC[j + 4].low, (kc->u.narrow[2 * 0 + 1]) = (kc->u.narrow[2 * 0 + 1]) ^ RC[j + 4].high);
                    }
                    while ( 0 );
                    if(out)
                    {
                        printf( "\n第%d轮中间状态：\n",k++ );
                        print_hex(kc->u.narrow, 1600);
                    }
                    do
                    {
                        do
                        {
                            sph_u64 t0l, t0h;
                            sph_u64 t1l, t1h;
                            sph_u64 t2l, t2h;
                            sph_u64 t3l, t3h;
                            sph_u64 t4l, t4h;
                            do
                            {
                                sph_u64 tt0l, tt0h;
                                sph_u64 tt1l, tt1h;
                                sph_u64 tt2l, tt2h;
                                sph_u64 tt3l, tt3h;
                                (tt0l = (kc->u.narrow[2 * 20 + 0]) ^ (kc->u.narrow[2 * 2 + 0]), tt0h = (kc->u.narrow[2 * 20 + 1]) ^ (kc->u.narrow[2 * 2 + 1]) );
                                (tt1l = (kc->u.narrow[2 * 9 + 0]) ^ (kc->u.narrow[2 * 11 + 0]), tt1h = (kc->u.narrow[2 * 9 + 1]) ^ (kc->u.narrow[2 * 11 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 18 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 18 + 1]) );
                                (tt0l = tt0l ^ tt1l, tt0h = tt0h ^ tt1h);
                                do
                                {
                                    sph_u32 tmp;
                                    tmp = tt0l;
                                    tt0l = ( (tt0h << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | (tt0h >> 31);
                                    tt0h = tmp;
                                }
                                while ( 0 );
                                (tt2l = (kc->u.narrow[2 * 5 + 0]) ^ (kc->u.narrow[2 * 12 + 0]), tt2h = (kc->u.narrow[2 * 5 + 1]) ^ (kc->u.narrow[2 * 12 + 1]) );
                                (tt3l = (kc->u.narrow[2 * 19 + 0]) ^ (kc->u.narrow[2 * 21 + 0]), tt3h = (kc->u.narrow[2 * 19 + 1]) ^ (kc->u.narrow[2 * 21 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 3 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 3 + 1]) );
                                (tt2l = tt2l ^ tt3l, tt2h = tt2h ^ tt3h);
                                (t0l = tt0l ^ tt2l, t0h = tt0h ^ tt2h);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 tt0l, tt0h;
                                sph_u64 tt1l, tt1h;
                                sph_u64 tt2l, tt2h;
                                sph_u64 tt3l, tt3h;
                                (tt0l = (kc->u.narrow[2 * 15 + 0]) ^ (kc->u.narrow[2 * 22 + 0]), tt0h = (kc->u.narrow[2 * 15 + 1]) ^ (kc->u.narrow[2 * 22 + 1]) );
                                (tt1l = (kc->u.narrow[2 * 4 + 0]) ^ (kc->u.narrow[2 * 6 + 0]), tt1h = (kc->u.narrow[2 * 4 + 1]) ^ (kc->u.narrow[2 * 6 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 13 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 13 + 1]) );
                                (tt0l = tt0l ^ tt1l, tt0h = tt0h ^ tt1h);
                                do
                                {
                                    sph_u32 tmp;
                                    tmp = tt0l;
                                    tt0l = ( (tt0h << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | (tt0h >> 31);
                                    tt0h = tmp;
                                }
                                while ( 0 );
                                (tt2l = (kc->u.narrow[2 * 0 + 0]) ^ (kc->u.narrow[2 * 7 + 0]), tt2h = (kc->u.narrow[2 * 0 + 1]) ^ (kc->u.narrow[2 * 7 + 1]) );
                                (tt3l = (kc->u.narrow[2 * 14 + 0]) ^ (kc->u.narrow[2 * 16 + 0]), tt3h = (kc->u.narrow[2 * 14 + 1]) ^ (kc->u.narrow[2 * 16 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 23 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 23 + 1]) );
                                (tt2l = tt2l ^ tt3l, tt2h = tt2h ^ tt3h);
                                (t1l = tt0l ^ tt2l, t1h = tt0h ^ tt2h);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 tt0l, tt0h;
                                sph_u64 tt1l, tt1h;
                                sph_u64 tt2l, tt2h;
                                sph_u64 tt3l, tt3h;
                                (tt0l = (kc->u.narrow[2 * 10 + 0]) ^ (kc->u.narrow[2 * 17 + 0]), tt0h = (kc->u.narrow[2 * 10 + 1]) ^ (kc->u.narrow[2 * 17 + 1]) );
                                (tt1l = (kc->u.narrow[2 * 24 + 0]) ^ (kc->u.narrow[2 * 1 + 0]), tt1h = (kc->u.narrow[2 * 24 + 1]) ^ (kc->u.narrow[2 * 1 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 8 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 8 + 1]) );
                                (tt0l = tt0l ^ tt1l, tt0h = tt0h ^ tt1h);
                                do
                                {
                                    sph_u32 tmp;
                                    tmp = tt0l;
                                    tt0l = ( (tt0h << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | (tt0h >> 31);
                                    tt0h = tmp;
                                }
                                while ( 0 );
                                (tt2l = (kc->u.narrow[2 * 20 + 0]) ^ (kc->u.narrow[2 * 2 + 0]), tt2h = (kc->u.narrow[2 * 20 + 1]) ^ (kc->u.narrow[2 * 2 + 1]) );
                                (tt3l = (kc->u.narrow[2 * 9 + 0]) ^ (kc->u.narrow[2 * 11 + 0]), tt3h = (kc->u.narrow[2 * 9 + 1]) ^ (kc->u.narrow[2 * 11 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 18 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 18 + 1]) );
                                (tt2l = tt2l ^ tt3l, tt2h = tt2h ^ tt3h);
                                (t2l = tt0l ^ tt2l, t2h = tt0h ^ tt2h);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 tt0l, tt0h;
                                sph_u64 tt1l, tt1h;
                                sph_u64 tt2l, tt2h;
                                sph_u64 tt3l, tt3h;
                                (tt0l = (kc->u.narrow[2 * 5 + 0]) ^ (kc->u.narrow[2 * 12 + 0]), tt0h = (kc->u.narrow[2 * 5 + 1]) ^ (kc->u.narrow[2 * 12 + 1]) );
                                (tt1l = (kc->u.narrow[2 * 19 + 0]) ^ (kc->u.narrow[2 * 21 + 0]), tt1h = (kc->u.narrow[2 * 19 + 1]) ^ (kc->u.narrow[2 * 21 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 3 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 3 + 1]) );
                                (tt0l = tt0l ^ tt1l, tt0h = tt0h ^ tt1h);
                                do
                                {
                                    sph_u32 tmp;
                                    tmp = tt0l;
                                    tt0l = ( (tt0h << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | (tt0h >> 31);
                                    tt0h = tmp;
                                }
                                while ( 0 );
                                (tt2l = (kc->u.narrow[2 * 15 + 0]) ^ (kc->u.narrow[2 * 22 + 0]), tt2h = (kc->u.narrow[2 * 15 + 1]) ^ (kc->u.narrow[2 * 22 + 1]) );
                                (tt3l = (kc->u.narrow[2 * 4 + 0]) ^ (kc->u.narrow[2 * 6 + 0]), tt3h = (kc->u.narrow[2 * 4 + 1]) ^ (kc->u.narrow[2 * 6 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 13 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 13 + 1]) );
                                (tt2l = tt2l ^ tt3l, tt2h = tt2h ^ tt3h);
                                (t3l = tt0l ^ tt2l, t3h = tt0h ^ tt2h);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 tt0l, tt0h;
                                sph_u64 tt1l, tt1h;
                                sph_u64 tt2l, tt2h;
                                sph_u64 tt3l, tt3h;
                                (tt0l = (kc->u.narrow[2 * 0 + 0]) ^ (kc->u.narrow[2 * 7 + 0]), tt0h = (kc->u.narrow[2 * 0 + 1]) ^ (kc->u.narrow[2 * 7 + 1]) );
                                (tt1l = (kc->u.narrow[2 * 14 + 0]) ^ (kc->u.narrow[2 * 16 + 0]), tt1h = (kc->u.narrow[2 * 14 + 1]) ^ (kc->u.narrow[2 * 16 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 23 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 23 + 1]) );
                                (tt0l = tt0l ^ tt1l, tt0h = tt0h ^ tt1h);
                                do
                                {
                                    sph_u32 tmp;
                                    tmp = tt0l;
                                    tt0l = ( (tt0h << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | (tt0h >> 31);
                                    tt0h = tmp;
                                }
                                while ( 0 );
                                (tt2l = (kc->u.narrow[2 * 10 + 0]) ^ (kc->u.narrow[2 * 17 + 0]), tt2h = (kc->u.narrow[2 * 10 + 1]) ^ (kc->u.narrow[2 * 17 + 1]) );
                                (tt3l = (kc->u.narrow[2 * 24 + 0]) ^ (kc->u.narrow[2 * 1 + 0]), tt3h = (kc->u.narrow[2 * 24 + 1]) ^ (kc->u.narrow[2 * 1 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 8 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 8 + 1]) );
                                (tt2l = tt2l ^ tt3l, tt2h = tt2h ^ tt3h);
                                (t4l = tt0l ^ tt2l, t4h = tt0h ^ tt2h);
                            }
                            while ( 0 );
                            ( (kc->u.narrow[2 * 0 + 0]) = (kc->u.narrow[2 * 0 + 0]) ^ t0l, (kc->u.narrow[2 * 0 + 1]) = (kc->u.narrow[2 * 0 + 1]) ^ t0h);
                            ( (kc->u.narrow[2 * 7 + 0]) = (kc->u.narrow[2 * 7 + 0]) ^ t0l, (kc->u.narrow[2 * 7 + 1]) = (kc->u.narrow[2 * 7 + 1]) ^ t0h);
                            ( (kc->u.narrow[2 * 14 + 0]) = (kc->u.narrow[2 * 14 + 0]) ^ t0l, (kc->u.narrow[2 * 14 + 1]) = (kc->u.narrow[2 * 14 + 1]) ^ t0h);
                            ( (kc->u.narrow[2 * 16 + 0]) = (kc->u.narrow[2 * 16 + 0]) ^ t0l, (kc->u.narrow[2 * 16 + 1]) = (kc->u.narrow[2 * 16 + 1]) ^ t0h);
                            ( (kc->u.narrow[2 * 23 + 0]) = (kc->u.narrow[2 * 23 + 0]) ^ t0l, (kc->u.narrow[2 * 23 + 1]) = (kc->u.narrow[2 * 23 + 1]) ^ t0h);
                            ( (kc->u.narrow[2 * 20 + 0]) = (kc->u.narrow[2 * 20 + 0]) ^ t1l, (kc->u.narrow[2 * 20 + 1]) = (kc->u.narrow[2 * 20 + 1]) ^ t1h);
                            ( (kc->u.narrow[2 * 2 + 0]) = (kc->u.narrow[2 * 2 + 0]) ^ t1l, (kc->u.narrow[2 * 2 + 1]) = (kc->u.narrow[2 * 2 + 1]) ^ t1h);
                            ( (kc->u.narrow[2 * 9 + 0]) = (kc->u.narrow[2 * 9 + 0]) ^ t1l, (kc->u.narrow[2 * 9 + 1]) = (kc->u.narrow[2 * 9 + 1]) ^ t1h);
                            ( (kc->u.narrow[2 * 11 + 0]) = (kc->u.narrow[2 * 11 + 0]) ^ t1l, (kc->u.narrow[2 * 11 + 1]) = (kc->u.narrow[2 * 11 + 1]) ^ t1h);
                            ( (kc->u.narrow[2 * 18 + 0]) = (kc->u.narrow[2 * 18 + 0]) ^ t1l, (kc->u.narrow[2 * 18 + 1]) = (kc->u.narrow[2 * 18 + 1]) ^ t1h);
                            ( (kc->u.narrow[2 * 15 + 0]) = (kc->u.narrow[2 * 15 + 0]) ^ t2l, (kc->u.narrow[2 * 15 + 1]) = (kc->u.narrow[2 * 15 + 1]) ^ t2h);
                            ( (kc->u.narrow[2 * 22 + 0]) = (kc->u.narrow[2 * 22 + 0]) ^ t2l, (kc->u.narrow[2 * 22 + 1]) = (kc->u.narrow[2 * 22 + 1]) ^ t2h);
                            ( (kc->u.narrow[2 * 4 + 0]) = (kc->u.narrow[2 * 4 + 0]) ^ t2l, (kc->u.narrow[2 * 4 + 1]) = (kc->u.narrow[2 * 4 + 1]) ^ t2h);
                            ( (kc->u.narrow[2 * 6 + 0]) = (kc->u.narrow[2 * 6 + 0]) ^ t2l, (kc->u.narrow[2 * 6 + 1]) = (kc->u.narrow[2 * 6 + 1]) ^ t2h);
                            ( (kc->u.narrow[2 * 13 + 0]) = (kc->u.narrow[2 * 13 + 0]) ^ t2l, (kc->u.narrow[2 * 13 + 1]) = (kc->u.narrow[2 * 13 + 1]) ^ t2h);
                            ( (kc->u.narrow[2 * 10 + 0]) = (kc->u.narrow[2 * 10 + 0]) ^ t3l, (kc->u.narrow[2 * 10 + 1]) = (kc->u.narrow[2 * 10 + 1]) ^ t3h);
                            ( (kc->u.narrow[2 * 17 + 0]) = (kc->u.narrow[2 * 17 + 0]) ^ t3l, (kc->u.narrow[2 * 17 + 1]) = (kc->u.narrow[2 * 17 + 1]) ^ t3h);
                            ( (kc->u.narrow[2 * 24 + 0]) = (kc->u.narrow[2 * 24 + 0]) ^ t3l, (kc->u.narrow[2 * 24 + 1]) = (kc->u.narrow[2 * 24 + 1]) ^ t3h);
                            ( (kc->u.narrow[2 * 1 + 0]) = (kc->u.narrow[2 * 1 + 0]) ^ t3l, (kc->u.narrow[2 * 1 + 1]) = (kc->u.narrow[2 * 1 + 1]) ^ t3h);
                            ( (kc->u.narrow[2 * 8 + 0]) = (kc->u.narrow[2 * 8 + 0]) ^ t3l, (kc->u.narrow[2 * 8 + 1]) = (kc->u.narrow[2 * 8 + 1]) ^ t3h);
                            ( (kc->u.narrow[2 * 5 + 0]) = (kc->u.narrow[2 * 5 + 0]) ^ t4l, (kc->u.narrow[2 * 5 + 1]) = (kc->u.narrow[2 * 5 + 1]) ^ t4h);
                            ( (kc->u.narrow[2 * 12 + 0]) = (kc->u.narrow[2 * 12 + 0]) ^ t4l, (kc->u.narrow[2 * 12 + 1]) = (kc->u.narrow[2 * 12 + 1]) ^ t4h);
                            ( (kc->u.narrow[2 * 19 + 0]) = (kc->u.narrow[2 * 19 + 0]) ^ t4l, (kc->u.narrow[2 * 19 + 1]) = (kc->u.narrow[2 * 19 + 1]) ^ t4h);
                            ( (kc->u.narrow[2 * 21 + 0]) = (kc->u.narrow[2 * 21 + 0]) ^ t4l, (kc->u.narrow[2 * 21 + 1]) = (kc->u.narrow[2 * 21 + 1]) ^ t4h);
                            ( (kc->u.narrow[2 * 3 + 0]) = (kc->u.narrow[2 * 3 + 0]) ^ t4l, (kc->u.narrow[2 * 3 + 1]) = (kc->u.narrow[2 * 3 + 1]) ^ t4h);
                        }
                        while ( 0 );
                        do
                        {
                            do
                            {
                                (kc->u.narrow[2 * 7 + 0]) = ( ( (kc->u.narrow[2 * 7 + 0]) << 18) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 7 + 0]) >> (32 - 18) );
                                (kc->u.narrow[2 * 7 + 1]) = ( ( (kc->u.narrow[2 * 7 + 1]) << 18) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 7 + 1]) >> (32 - 18) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 14 + 0]) << (2 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 14 + 0]) >> (33 - 2) );
                                (kc->u.narrow[2 * 14 + 0]) = ( ( (kc->u.narrow[2 * 14 + 1]) << 2) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 14 + 1]) >> (32 - 2) );
                                (kc->u.narrow[2 * 14 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 16 + 0]) << (21 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 16 + 0]) >> (33 - 21) );
                                (kc->u.narrow[2 * 16 + 0]) = ( ( (kc->u.narrow[2 * 16 + 1]) << 21) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 16 + 1]) >> (32 - 21) );
                                (kc->u.narrow[2 * 16 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 23 + 0]) = ( ( (kc->u.narrow[2 * 23 + 0]) << 9) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 23 + 0]) >> (32 - 9) );
                                (kc->u.narrow[2 * 23 + 1]) = ( ( (kc->u.narrow[2 * 23 + 1]) << 9) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 23 + 1]) >> (32 - 9) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = (kc->u.narrow[2 * 20 + 0]);
                                (kc->u.narrow[2 * 20 + 0]) = ( ( (kc->u.narrow[2 * 20 + 1]) << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 20 + 1]) >> 31);
                                (kc->u.narrow[2 * 20 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 2 + 0]) = ( ( (kc->u.narrow[2 * 2 + 0]) << 22) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 2 + 0]) >> (32 - 22) );
                                (kc->u.narrow[2 * 2 + 1]) = ( ( (kc->u.narrow[2 * 2 + 1]) << 22) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 2 + 1]) >> (32 - 22) );
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 9 + 0]) = ( ( (kc->u.narrow[2 * 9 + 0]) << 5) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 9 + 0]) >> (32 - 5) );
                                (kc->u.narrow[2 * 9 + 1]) = ( ( (kc->u.narrow[2 * 9 + 1]) << 5) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 9 + 1]) >> (32 - 5) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 11 + 0]) << (23 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 11 + 0]) >> (33 - 23) );
                                (kc->u.narrow[2 * 11 + 0]) = ( ( (kc->u.narrow[2 * 11 + 1]) << 23) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 11 + 1]) >> (32 - 23) );
                                (kc->u.narrow[2 * 11 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 18 + 0]) = ( ( (kc->u.narrow[2 * 18 + 0]) << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 18 + 0]) >> (32 - 1) );
                                (kc->u.narrow[2 * 18 + 1]) = ( ( (kc->u.narrow[2 * 18 + 1]) << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 18 + 1]) >> (32 - 1) );
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 15 + 0]) = ( ( (kc->u.narrow[2 * 15 + 0]) << 31) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 15 + 0]) >> (32 - 31) );
                                (kc->u.narrow[2 * 15 + 1]) = ( ( (kc->u.narrow[2 * 15 + 1]) << 31) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 15 + 1]) >> (32 - 31) );
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 22 + 0]) = ( ( (kc->u.narrow[2 * 22 + 0]) << 3) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 22 + 0]) >> (32 - 3) );
                                (kc->u.narrow[2 * 22 + 1]) = ( ( (kc->u.narrow[2 * 22 + 1]) << 3) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 22 + 1]) >> (32 - 3) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 4 + 0]) << (22 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 4 + 0]) >> (33 - 22) );
                                (kc->u.narrow[2 * 4 + 0]) = ( ( (kc->u.narrow[2 * 4 + 1]) << 22) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 4 + 1]) >> (32 - 22) );
                                (kc->u.narrow[2 * 4 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 6 + 0]) << (8 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 6 + 0]) >> (33 - 8) );
                                (kc->u.narrow[2 * 6 + 0]) = ( ( (kc->u.narrow[2 * 6 + 1]) << 8) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 6 + 1]) >> (32 - 8) );
                                (kc->u.narrow[2 * 6 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 13 + 0]) << (31 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 13 + 0]) >> (33 - 31) );
                                (kc->u.narrow[2 * 13 + 0]) = ( ( (kc->u.narrow[2 * 13 + 1]) << 31) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 13 + 1]) >> (32 - 31) );
                                (kc->u.narrow[2 * 13 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 10 + 0]) = ( ( (kc->u.narrow[2 * 10 + 0]) << 14) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 10 + 0]) >> (32 - 14) );
                                (kc->u.narrow[2 * 10 + 1]) = ( ( (kc->u.narrow[2 * 10 + 1]) << 14) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 10 + 1]) >> (32 - 14) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 17 + 0]) << (28 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 17 + 0]) >> (33 - 28) );
                                (kc->u.narrow[2 * 17 + 0]) = ( ( (kc->u.narrow[2 * 17 + 1]) << 28) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 17 + 1]) >> (32 - 28) );
                                (kc->u.narrow[2 * 17 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 24 + 0]) << (13 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 24 + 0]) >> (33 - 13) );
                                (kc->u.narrow[2 * 24 + 0]) = ( ( (kc->u.narrow[2 * 24 + 1]) << 13) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 24 + 1]) >> (32 - 13) );
                                (kc->u.narrow[2 * 24 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 1 + 0]) << (11 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 1 + 0]) >> (33 - 11) );
                                (kc->u.narrow[2 * 1 + 0]) = ( ( (kc->u.narrow[2 * 1 + 1]) << 11) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 1 + 1]) >> (32 - 11) );
                                (kc->u.narrow[2 * 1 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 8 + 0]) = ( ( (kc->u.narrow[2 * 8 + 0]) << 28) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 8 + 0]) >> (32 - 28) );
                                (kc->u.narrow[2 * 8 + 1]) = ( ( (kc->u.narrow[2 * 8 + 1]) << 28) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 8 + 1]) >> (32 - 28) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 5 + 0]) << (14 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 5 + 0]) >> (33 - 14) );
                                (kc->u.narrow[2 * 5 + 0]) = ( ( (kc->u.narrow[2 * 5 + 1]) << 14) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 5 + 1]) >> (32 - 14) );
                                (kc->u.narrow[2 * 5 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 12 + 0]) = ( ( (kc->u.narrow[2 * 12 + 0]) << 10) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 12 + 0]) >> (32 - 10) );
                                (kc->u.narrow[2 * 12 + 1]) = ( ( (kc->u.narrow[2 * 12 + 1]) << 10) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 12 + 1]) >> (32 - 10) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 19 + 0]) << (20 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 19 + 0]) >> (33 - 20) );
                                (kc->u.narrow[2 * 19 + 0]) = ( ( (kc->u.narrow[2 * 19 + 1]) << 20) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 19 + 1]) >> (32 - 20) );
                                (kc->u.narrow[2 * 19 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 21 + 0]) = ( ( (kc->u.narrow[2 * 21 + 0]) << 4) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 21 + 0]) >> (32 - 4) );
                                (kc->u.narrow[2 * 21 + 1]) = ( ( (kc->u.narrow[2 * 21 + 1]) << 4) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 21 + 1]) >> (32 - 4) );
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 3 + 0]) = ( ( (kc->u.narrow[2 * 3 + 0]) << 7) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 3 + 0]) >> (32 - 7) );
                                (kc->u.narrow[2 * 3 + 1]) = ( ( (kc->u.narrow[2 * 3 + 1]) << 7) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 3 + 1]) >> (32 - 7) );
                            }
                            while ( 0 );
                        }
                        while ( 0 );
                        do
                        {
                            sph_u64 c0l, c0h;
                            sph_u64 c1l, c1h;
                            sph_u64 c2l, c2h;
                            sph_u64 c3l, c3h;
                            sph_u64 c4l, c4h;
                            sph_u64 bnnl, bnnh;
                            (bnnl = ( (~(kc->u.narrow[2 * 4 + 0]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ), bnnh = ( (~(kc->u.narrow[2 * 4 + 1]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 2 + 0]) | (kc->u.narrow[2 * 4 + 0]), kth = (kc->u.narrow[2 * 2 + 1]) | (kc->u.narrow[2 * 4 + 1]) );
                                (c0l = (kc->u.narrow[2 * 0 + 0]) ^ ktl, c0h = (kc->u.narrow[2 * 0 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = bnnl | (kc->u.narrow[2 * 1 + 0]), kth = bnnh | (kc->u.narrow[2 * 1 + 1]) );
                                (c1l = (kc->u.narrow[2 * 2 + 0]) ^ ktl, c1h = (kc->u.narrow[2 * 2 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 1 + 0]) & (kc->u.narrow[2 * 3 + 0]), kth = (kc->u.narrow[2 * 1 + 1]) & (kc->u.narrow[2 * 3 + 1]) );
                                (c2l = (kc->u.narrow[2 * 4 + 0]) ^ ktl, c2h = (kc->u.narrow[2 * 4 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 3 + 0]) | (kc->u.narrow[2 * 0 + 0]), kth = (kc->u.narrow[2 * 3 + 1]) | (kc->u.narrow[2 * 0 + 1]) );
                                (c3l = (kc->u.narrow[2 * 1 + 0]) ^ ktl, c3h = (kc->u.narrow[2 * 1 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 0 + 0]) & (kc->u.narrow[2 * 2 + 0]), kth = (kc->u.narrow[2 * 0 + 1]) & (kc->u.narrow[2 * 2 + 1]) );
                                (c4l = (kc->u.narrow[2 * 3 + 0]) ^ ktl, c4h = (kc->u.narrow[2 * 3 + 1]) ^ kth);
                            }
                            while ( 0 );
                            ( (kc->u.narrow[2 * 0 + 0]) = c0l, (kc->u.narrow[2 * 0 + 1]) = c0h);
                            ( (kc->u.narrow[2 * 2 + 0]) = c1l, (kc->u.narrow[2 * 2 + 1]) = c1h);
                            ( (kc->u.narrow[2 * 4 + 0]) = c2l, (kc->u.narrow[2 * 4 + 1]) = c2h);
                            ( (kc->u.narrow[2 * 1 + 0]) = c3l, (kc->u.narrow[2 * 1 + 1]) = c3h);
                            ( (kc->u.narrow[2 * 3 + 0]) = c4l, (kc->u.narrow[2 * 3 + 1]) = c4h);
                            (bnnl = ( (~(kc->u.narrow[2 * 13 + 0]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ), bnnh = ( (~(kc->u.narrow[2 * 13 + 1]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 12 + 0]) | (kc->u.narrow[2 * 14 + 0]), kth = (kc->u.narrow[2 * 12 + 1]) | (kc->u.narrow[2 * 14 + 1]) );
                                (c0l = (kc->u.narrow[2 * 10 + 0]) ^ ktl, c0h = (kc->u.narrow[2 * 10 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 14 + 0]) & (kc->u.narrow[2 * 11 + 0]), kth = (kc->u.narrow[2 * 14 + 1]) & (kc->u.narrow[2 * 11 + 1]) );
                                (c1l = (kc->u.narrow[2 * 12 + 0]) ^ ktl, c1h = (kc->u.narrow[2 * 12 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 11 + 0]) | bnnl, kth = (kc->u.narrow[2 * 11 + 1]) | bnnh);
                                (c2l = (kc->u.narrow[2 * 14 + 0]) ^ ktl, c2h = (kc->u.narrow[2 * 14 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 13 + 0]) | (kc->u.narrow[2 * 10 + 0]), kth = (kc->u.narrow[2 * 13 + 1]) | (kc->u.narrow[2 * 10 + 1]) );
                                (c3l = (kc->u.narrow[2 * 11 + 0]) ^ ktl, c3h = (kc->u.narrow[2 * 11 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 10 + 0]) & (kc->u.narrow[2 * 12 + 0]), kth = (kc->u.narrow[2 * 10 + 1]) & (kc->u.narrow[2 * 12 + 1]) );
                                (c4l = (kc->u.narrow[2 * 13 + 0]) ^ ktl, c4h = (kc->u.narrow[2 * 13 + 1]) ^ kth);
                            }
                            while ( 0 );
                            ( (kc->u.narrow[2 * 10 + 0]) = c0l, (kc->u.narrow[2 * 10 + 1]) = c0h);
                            ( (kc->u.narrow[2 * 12 + 0]) = c1l, (kc->u.narrow[2 * 12 + 1]) = c1h);
                            ( (kc->u.narrow[2 * 14 + 0]) = c2l, (kc->u.narrow[2 * 14 + 1]) = c2h);
                            ( (kc->u.narrow[2 * 11 + 0]) = c3l, (kc->u.narrow[2 * 11 + 1]) = c3h);
                            ( (kc->u.narrow[2 * 13 + 0]) = c4l, (kc->u.narrow[2 * 13 + 1]) = c4h);
                            (bnnl = ( (~(kc->u.narrow[2 * 21 + 0]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ), bnnh = ( (~(kc->u.narrow[2 * 21 + 1]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 22 + 0]) | (kc->u.narrow[2 * 24 + 0]), kth = (kc->u.narrow[2 * 22 + 1]) | (kc->u.narrow[2 * 24 + 1]) );
                                (c0l = (kc->u.narrow[2 * 20 + 0]) ^ ktl, c0h = (kc->u.narrow[2 * 20 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 24 + 0]) & (kc->u.narrow[2 * 21 + 0]), kth = (kc->u.narrow[2 * 24 + 1]) & (kc->u.narrow[2 * 21 + 1]) );
                                (c1l = (kc->u.narrow[2 * 22 + 0]) ^ ktl, c1h = (kc->u.narrow[2 * 22 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = bnnl & (kc->u.narrow[2 * 23 + 0]), kth = bnnh & (kc->u.narrow[2 * 23 + 1]) );
                                (c2l = (kc->u.narrow[2 * 24 + 0]) ^ ktl, c2h = (kc->u.narrow[2 * 24 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 23 + 0]) | (kc->u.narrow[2 * 20 + 0]), kth = (kc->u.narrow[2 * 23 + 1]) | (kc->u.narrow[2 * 20 + 1]) );
                                (c3l = bnnl ^ ktl, c3h = bnnh ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 20 + 0]) & (kc->u.narrow[2 * 22 + 0]), kth = (kc->u.narrow[2 * 20 + 1]) & (kc->u.narrow[2 * 22 + 1]) );
                                (c4l = (kc->u.narrow[2 * 23 + 0]) ^ ktl, c4h = (kc->u.narrow[2 * 23 + 1]) ^ kth);
                            }
                            while ( 0 );
                            ( (kc->u.narrow[2 * 20 + 0]) = c0l, (kc->u.narrow[2 * 20 + 1]) = c0h);
                            ( (kc->u.narrow[2 * 22 + 0]) = c1l, (kc->u.narrow[2 * 22 + 1]) = c1h);
                            ( (kc->u.narrow[2 * 24 + 0]) = c2l, (kc->u.narrow[2 * 24 + 1]) = c2h);
                            ( (kc->u.narrow[2 * 21 + 0]) = c3l, (kc->u.narrow[2 * 21 + 1]) = c3h);
                            ( (kc->u.narrow[2 * 23 + 0]) = c4l, (kc->u.narrow[2 * 23 + 1]) = c4h);
                            (bnnl = ( (~(kc->u.narrow[2 * 6 + 0]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ), bnnh = ( (~(kc->u.narrow[2 * 6 + 1]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 7 + 0]) & (kc->u.narrow[2 * 9 + 0]), kth = (kc->u.narrow[2 * 7 + 1]) & (kc->u.narrow[2 * 9 + 1]) );
                                (c0l = (kc->u.narrow[2 * 5 + 0]) ^ ktl, c0h = (kc->u.narrow[2 * 5 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 9 + 0]) | (kc->u.narrow[2 * 6 + 0]), kth = (kc->u.narrow[2 * 9 + 1]) | (kc->u.narrow[2 * 6 + 1]) );
                                (c1l = (kc->u.narrow[2 * 7 + 0]) ^ ktl, c1h = (kc->u.narrow[2 * 7 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = bnnl | (kc->u.narrow[2 * 8 + 0]), kth = bnnh | (kc->u.narrow[2 * 8 + 1]) );
                                (c2l = (kc->u.narrow[2 * 9 + 0]) ^ ktl, c2h = (kc->u.narrow[2 * 9 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 8 + 0]) & (kc->u.narrow[2 * 5 + 0]), kth = (kc->u.narrow[2 * 8 + 1]) & (kc->u.narrow[2 * 5 + 1]) );
                                (c3l = bnnl ^ ktl, c3h = bnnh ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 5 + 0]) | (kc->u.narrow[2 * 7 + 0]), kth = (kc->u.narrow[2 * 5 + 1]) | (kc->u.narrow[2 * 7 + 1]) );
                                (c4l = (kc->u.narrow[2 * 8 + 0]) ^ ktl, c4h = (kc->u.narrow[2 * 8 + 1]) ^ kth);
                            }
                            while ( 0 );
                            ( (kc->u.narrow[2 * 5 + 0]) = c0l, (kc->u.narrow[2 * 5 + 1]) = c0h);
                            ( (kc->u.narrow[2 * 7 + 0]) = c1l, (kc->u.narrow[2 * 7 + 1]) = c1h);
                            ( (kc->u.narrow[2 * 9 + 0]) = c2l, (kc->u.narrow[2 * 9 + 1]) = c2h);
                            ( (kc->u.narrow[2 * 6 + 0]) = c3l, (kc->u.narrow[2 * 6 + 1]) = c3h);
                            ( (kc->u.narrow[2 * 8 + 0]) = c4l, (kc->u.narrow[2 * 8 + 1]) = c4h);
                            (bnnl = ( (~(kc->u.narrow[2 * 17 + 0]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ), bnnh = ( (~(kc->u.narrow[2 * 17 + 1]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = bnnl & (kc->u.narrow[2 * 19 + 0]), kth = bnnh & (kc->u.narrow[2 * 19 + 1]) );
                                (c0l = (kc->u.narrow[2 * 15 + 0]) ^ ktl, c0h = (kc->u.narrow[2 * 15 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 19 + 0]) | (kc->u.narrow[2 * 16 + 0]), kth = (kc->u.narrow[2 * 19 + 1]) | (kc->u.narrow[2 * 16 + 1]) );
                                (c1l = bnnl ^ ktl, c1h = bnnh ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 16 + 0]) & (kc->u.narrow[2 * 18 + 0]), kth = (kc->u.narrow[2 * 16 + 1]) & (kc->u.narrow[2 * 18 + 1]) );
                                (c2l = (kc->u.narrow[2 * 19 + 0]) ^ ktl, c2h = (kc->u.narrow[2 * 19 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 18 + 0]) | (kc->u.narrow[2 * 15 + 0]), kth = (kc->u.narrow[2 * 18 + 1]) | (kc->u.narrow[2 * 15 + 1]) );
                                (c3l = (kc->u.narrow[2 * 16 + 0]) ^ ktl, c3h = (kc->u.narrow[2 * 16 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 15 + 0]) & (kc->u.narrow[2 * 17 + 0]), kth = (kc->u.narrow[2 * 15 + 1]) & (kc->u.narrow[2 * 17 + 1]) );
                                (c4l = (kc->u.narrow[2 * 18 + 0]) ^ ktl, c4h = (kc->u.narrow[2 * 18 + 1]) ^ kth);
                            }
                            while ( 0 );
                            ( (kc->u.narrow[2 * 15 + 0]) = c0l, (kc->u.narrow[2 * 15 + 1]) = c0h);
                            ( (kc->u.narrow[2 * 17 + 0]) = c1l, (kc->u.narrow[2 * 17 + 1]) = c1h);
                            ( (kc->u.narrow[2 * 19 + 0]) = c2l, (kc->u.narrow[2 * 19 + 1]) = c2h);
                            ( (kc->u.narrow[2 * 16 + 0]) = c3l, (kc->u.narrow[2 * 16 + 1]) = c3h);
                            ( (kc->u.narrow[2 * 18 + 0]) = c4l, (kc->u.narrow[2 * 18 + 1]) = c4h);
                        }
                        while ( 0 );
                        ( (kc->u.narrow[2 * 0 + 0]) = (kc->u.narrow[2 * 0 + 0]) ^ RC[j + 5].low, (kc->u.narrow[2 * 0 + 1]) = (kc->u.narrow[2 * 0 + 1]) ^ RC[j + 5].high);
                    }
                    while ( 0 );
                    if(out)
                    {
                        printf( "\n第%d轮中间状态：\n",k++ );
                        print_hex(kc->u.narrow, 1600);
                    }
                    do
                    {
                        do
                        {
                            sph_u64 t0l, t0h;
                            sph_u64 t1l, t1h;
                            sph_u64 t2l, t2h;
                            sph_u64 t3l, t3h;
                            sph_u64 t4l, t4h;
                            do
                            {
                                sph_u64 tt0l, tt0h;
                                sph_u64 tt1l, tt1h;
                                sph_u64 tt2l, tt2h;
                                sph_u64 tt3l, tt3h;
                                (tt0l = (kc->u.narrow[2 * 2 + 0]) ^ (kc->u.narrow[2 * 12 + 0]), tt0h = (kc->u.narrow[2 * 2 + 1]) ^ (kc->u.narrow[2 * 12 + 1]) );
                                (tt1l = (kc->u.narrow[2 * 22 + 0]) ^ (kc->u.narrow[2 * 7 + 0]), tt1h = (kc->u.narrow[2 * 22 + 1]) ^ (kc->u.narrow[2 * 7 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 17 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 17 + 1]) );
                                (tt0l = tt0l ^ tt1l, tt0h = tt0h ^ tt1h);
                                do
                                {
                                    sph_u32 tmp;
                                    tmp = tt0l;
                                    tt0l = ( (tt0h << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | (tt0h >> 31);
                                    tt0h = tmp;
                                }
                                while ( 0 );
                                (tt2l = (kc->u.narrow[2 * 3 + 0]) ^ (kc->u.narrow[2 * 13 + 0]), tt2h = (kc->u.narrow[2 * 3 + 1]) ^ (kc->u.narrow[2 * 13 + 1]) );
                                (tt3l = (kc->u.narrow[2 * 23 + 0]) ^ (kc->u.narrow[2 * 8 + 0]), tt3h = (kc->u.narrow[2 * 23 + 1]) ^ (kc->u.narrow[2 * 8 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 18 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 18 + 1]) );
                                (tt2l = tt2l ^ tt3l, tt2h = tt2h ^ tt3h);
                                (t0l = tt0l ^ tt2l, t0h = tt0h ^ tt2h);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 tt0l, tt0h;
                                sph_u64 tt1l, tt1h;
                                sph_u64 tt2l, tt2h;
                                sph_u64 tt3l, tt3h;
                                (tt0l = (kc->u.narrow[2 * 4 + 0]) ^ (kc->u.narrow[2 * 14 + 0]), tt0h = (kc->u.narrow[2 * 4 + 1]) ^ (kc->u.narrow[2 * 14 + 1]) );
                                (tt1l = (kc->u.narrow[2 * 24 + 0]) ^ (kc->u.narrow[2 * 9 + 0]), tt1h = (kc->u.narrow[2 * 24 + 1]) ^ (kc->u.narrow[2 * 9 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 19 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 19 + 1]) );
                                (tt0l = tt0l ^ tt1l, tt0h = tt0h ^ tt1h);
                                do
                                {
                                    sph_u32 tmp;
                                    tmp = tt0l;
                                    tt0l = ( (tt0h << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | (tt0h >> 31);
                                    tt0h = tmp;
                                }
                                while ( 0 );
                                (tt2l = (kc->u.narrow[2 * 0 + 0]) ^ (kc->u.narrow[2 * 10 + 0]), tt2h = (kc->u.narrow[2 * 0 + 1]) ^ (kc->u.narrow[2 * 10 + 1]) );
                                (tt3l = (kc->u.narrow[2 * 20 + 0]) ^ (kc->u.narrow[2 * 5 + 0]), tt3h = (kc->u.narrow[2 * 20 + 1]) ^ (kc->u.narrow[2 * 5 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 15 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 15 + 1]) );
                                (tt2l = tt2l ^ tt3l, tt2h = tt2h ^ tt3h);
                                (t1l = tt0l ^ tt2l, t1h = tt0h ^ tt2h);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 tt0l, tt0h;
                                sph_u64 tt1l, tt1h;
                                sph_u64 tt2l, tt2h;
                                sph_u64 tt3l, tt3h;
                                (tt0l = (kc->u.narrow[2 * 1 + 0]) ^ (kc->u.narrow[2 * 11 + 0]), tt0h = (kc->u.narrow[2 * 1 + 1]) ^ (kc->u.narrow[2 * 11 + 1]) );
                                (tt1l = (kc->u.narrow[2 * 21 + 0]) ^ (kc->u.narrow[2 * 6 + 0]), tt1h = (kc->u.narrow[2 * 21 + 1]) ^ (kc->u.narrow[2 * 6 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 16 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 16 + 1]) );
                                (tt0l = tt0l ^ tt1l, tt0h = tt0h ^ tt1h);
                                do
                                {
                                    sph_u32 tmp;
                                    tmp = tt0l;
                                    tt0l = ( (tt0h << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | (tt0h >> 31);
                                    tt0h = tmp;
                                }
                                while ( 0 );
                                (tt2l = (kc->u.narrow[2 * 2 + 0]) ^ (kc->u.narrow[2 * 12 + 0]), tt2h = (kc->u.narrow[2 * 2 + 1]) ^ (kc->u.narrow[2 * 12 + 1]) );
                                (tt3l = (kc->u.narrow[2 * 22 + 0]) ^ (kc->u.narrow[2 * 7 + 0]), tt3h = (kc->u.narrow[2 * 22 + 1]) ^ (kc->u.narrow[2 * 7 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 17 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 17 + 1]) );
                                (tt2l = tt2l ^ tt3l, tt2h = tt2h ^ tt3h);
                                (t2l = tt0l ^ tt2l, t2h = tt0h ^ tt2h);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 tt0l, tt0h;
                                sph_u64 tt1l, tt1h;
                                sph_u64 tt2l, tt2h;
                                sph_u64 tt3l, tt3h;
                                (tt0l = (kc->u.narrow[2 * 3 + 0]) ^ (kc->u.narrow[2 * 13 + 0]), tt0h = (kc->u.narrow[2 * 3 + 1]) ^ (kc->u.narrow[2 * 13 + 1]) );
                                (tt1l = (kc->u.narrow[2 * 23 + 0]) ^ (kc->u.narrow[2 * 8 + 0]), tt1h = (kc->u.narrow[2 * 23 + 1]) ^ (kc->u.narrow[2 * 8 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 18 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 18 + 1]) );
                                (tt0l = tt0l ^ tt1l, tt0h = tt0h ^ tt1h);
                                do
                                {
                                    sph_u32 tmp;
                                    tmp = tt0l;
                                    tt0l = ( (tt0h << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | (tt0h >> 31);
                                    tt0h = tmp;
                                }
                                while ( 0 );
                                (tt2l = (kc->u.narrow[2 * 4 + 0]) ^ (kc->u.narrow[2 * 14 + 0]), tt2h = (kc->u.narrow[2 * 4 + 1]) ^ (kc->u.narrow[2 * 14 + 1]) );
                                (tt3l = (kc->u.narrow[2 * 24 + 0]) ^ (kc->u.narrow[2 * 9 + 0]), tt3h = (kc->u.narrow[2 * 24 + 1]) ^ (kc->u.narrow[2 * 9 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 19 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 19 + 1]) );
                                (tt2l = tt2l ^ tt3l, tt2h = tt2h ^ tt3h);
                                (t3l = tt0l ^ tt2l, t3h = tt0h ^ tt2h);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 tt0l, tt0h;
                                sph_u64 tt1l, tt1h;
                                sph_u64 tt2l, tt2h;
                                sph_u64 tt3l, tt3h;
                                (tt0l = (kc->u.narrow[2 * 0 + 0]) ^ (kc->u.narrow[2 * 10 + 0]), tt0h = (kc->u.narrow[2 * 0 + 1]) ^ (kc->u.narrow[2 * 10 + 1]) );
                                (tt1l = (kc->u.narrow[2 * 20 + 0]) ^ (kc->u.narrow[2 * 5 + 0]), tt1h = (kc->u.narrow[2 * 20 + 1]) ^ (kc->u.narrow[2 * 5 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 15 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 15 + 1]) );
                                (tt0l = tt0l ^ tt1l, tt0h = tt0h ^ tt1h);
                                do
                                {
                                    sph_u32 tmp;
                                    tmp = tt0l;
                                    tt0l = ( (tt0h << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | (tt0h >> 31);
                                    tt0h = tmp;
                                }
                                while ( 0 );
                                (tt2l = (kc->u.narrow[2 * 1 + 0]) ^ (kc->u.narrow[2 * 11 + 0]), tt2h = (kc->u.narrow[2 * 1 + 1]) ^ (kc->u.narrow[2 * 11 + 1]) );
                                (tt3l = (kc->u.narrow[2 * 21 + 0]) ^ (kc->u.narrow[2 * 6 + 0]), tt3h = (kc->u.narrow[2 * 21 + 1]) ^ (kc->u.narrow[2 * 6 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 16 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 16 + 1]) );
                                (tt2l = tt2l ^ tt3l, tt2h = tt2h ^ tt3h);
                                (t4l = tt0l ^ tt2l, t4h = tt0h ^ tt2h);
                            }
                            while ( 0 );
                            ( (kc->u.narrow[2 * 0 + 0]) = (kc->u.narrow[2 * 0 + 0]) ^ t0l, (kc->u.narrow[2 * 0 + 1]) = (kc->u.narrow[2 * 0 + 1]) ^ t0h);
                            ( (kc->u.narrow[2 * 10 + 0]) = (kc->u.narrow[2 * 10 + 0]) ^ t0l, (kc->u.narrow[2 * 10 + 1]) = (kc->u.narrow[2 * 10 + 1]) ^ t0h);
                            ( (kc->u.narrow[2 * 20 + 0]) = (kc->u.narrow[2 * 20 + 0]) ^ t0l, (kc->u.narrow[2 * 20 + 1]) = (kc->u.narrow[2 * 20 + 1]) ^ t0h);
                            ( (kc->u.narrow[2 * 5 + 0]) = (kc->u.narrow[2 * 5 + 0]) ^ t0l, (kc->u.narrow[2 * 5 + 1]) = (kc->u.narrow[2 * 5 + 1]) ^ t0h);
                            ( (kc->u.narrow[2 * 15 + 0]) = (kc->u.narrow[2 * 15 + 0]) ^ t0l, (kc->u.narrow[2 * 15 + 1]) = (kc->u.narrow[2 * 15 + 1]) ^ t0h);
                            ( (kc->u.narrow[2 * 2 + 0]) = (kc->u.narrow[2 * 2 + 0]) ^ t1l, (kc->u.narrow[2 * 2 + 1]) = (kc->u.narrow[2 * 2 + 1]) ^ t1h);
                            ( (kc->u.narrow[2 * 12 + 0]) = (kc->u.narrow[2 * 12 + 0]) ^ t1l, (kc->u.narrow[2 * 12 + 1]) = (kc->u.narrow[2 * 12 + 1]) ^ t1h);
                            ( (kc->u.narrow[2 * 22 + 0]) = (kc->u.narrow[2 * 22 + 0]) ^ t1l, (kc->u.narrow[2 * 22 + 1]) = (kc->u.narrow[2 * 22 + 1]) ^ t1h);
                            ( (kc->u.narrow[2 * 7 + 0]) = (kc->u.narrow[2 * 7 + 0]) ^ t1l, (kc->u.narrow[2 * 7 + 1]) = (kc->u.narrow[2 * 7 + 1]) ^ t1h);
                            ( (kc->u.narrow[2 * 17 + 0]) = (kc->u.narrow[2 * 17 + 0]) ^ t1l, (kc->u.narrow[2 * 17 + 1]) = (kc->u.narrow[2 * 17 + 1]) ^ t1h);
                            ( (kc->u.narrow[2 * 4 + 0]) = (kc->u.narrow[2 * 4 + 0]) ^ t2l, (kc->u.narrow[2 * 4 + 1]) = (kc->u.narrow[2 * 4 + 1]) ^ t2h);
                            ( (kc->u.narrow[2 * 14 + 0]) = (kc->u.narrow[2 * 14 + 0]) ^ t2l, (kc->u.narrow[2 * 14 + 1]) = (kc->u.narrow[2 * 14 + 1]) ^ t2h);
                            ( (kc->u.narrow[2 * 24 + 0]) = (kc->u.narrow[2 * 24 + 0]) ^ t2l, (kc->u.narrow[2 * 24 + 1]) = (kc->u.narrow[2 * 24 + 1]) ^ t2h);
                            ( (kc->u.narrow[2 * 9 + 0]) = (kc->u.narrow[2 * 9 + 0]) ^ t2l, (kc->u.narrow[2 * 9 + 1]) = (kc->u.narrow[2 * 9 + 1]) ^ t2h);
                            ( (kc->u.narrow[2 * 19 + 0]) = (kc->u.narrow[2 * 19 + 0]) ^ t2l, (kc->u.narrow[2 * 19 + 1]) = (kc->u.narrow[2 * 19 + 1]) ^ t2h);
                            ( (kc->u.narrow[2 * 1 + 0]) = (kc->u.narrow[2 * 1 + 0]) ^ t3l, (kc->u.narrow[2 * 1 + 1]) = (kc->u.narrow[2 * 1 + 1]) ^ t3h);
                            ( (kc->u.narrow[2 * 11 + 0]) = (kc->u.narrow[2 * 11 + 0]) ^ t3l, (kc->u.narrow[2 * 11 + 1]) = (kc->u.narrow[2 * 11 + 1]) ^ t3h);
                            ( (kc->u.narrow[2 * 21 + 0]) = (kc->u.narrow[2 * 21 + 0]) ^ t3l, (kc->u.narrow[2 * 21 + 1]) = (kc->u.narrow[2 * 21 + 1]) ^ t3h);
                            ( (kc->u.narrow[2 * 6 + 0]) = (kc->u.narrow[2 * 6 + 0]) ^ t3l, (kc->u.narrow[2 * 6 + 1]) = (kc->u.narrow[2 * 6 + 1]) ^ t3h);
                            ( (kc->u.narrow[2 * 16 + 0]) = (kc->u.narrow[2 * 16 + 0]) ^ t3l, (kc->u.narrow[2 * 16 + 1]) = (kc->u.narrow[2 * 16 + 1]) ^ t3h);
                            ( (kc->u.narrow[2 * 3 + 0]) = (kc->u.narrow[2 * 3 + 0]) ^ t4l, (kc->u.narrow[2 * 3 + 1]) = (kc->u.narrow[2 * 3 + 1]) ^ t4h);
                            ( (kc->u.narrow[2 * 13 + 0]) = (kc->u.narrow[2 * 13 + 0]) ^ t4l, (kc->u.narrow[2 * 13 + 1]) = (kc->u.narrow[2 * 13 + 1]) ^ t4h);
                            ( (kc->u.narrow[2 * 23 + 0]) = (kc->u.narrow[2 * 23 + 0]) ^ t4l, (kc->u.narrow[2 * 23 + 1]) = (kc->u.narrow[2 * 23 + 1]) ^ t4h);
                            ( (kc->u.narrow[2 * 8 + 0]) = (kc->u.narrow[2 * 8 + 0]) ^ t4l, (kc->u.narrow[2 * 8 + 1]) = (kc->u.narrow[2 * 8 + 1]) ^ t4h);
                            ( (kc->u.narrow[2 * 18 + 0]) = (kc->u.narrow[2 * 18 + 0]) ^ t4l, (kc->u.narrow[2 * 18 + 1]) = (kc->u.narrow[2 * 18 + 1]) ^ t4h);
                        }
                        while ( 0 );
                        do
                        {
                            do
                            {
                                (kc->u.narrow[2 * 10 + 0]) = ( ( (kc->u.narrow[2 * 10 + 0]) << 18) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 10 + 0]) >> (32 - 18) );
                                (kc->u.narrow[2 * 10 + 1]) = ( ( (kc->u.narrow[2 * 10 + 1]) << 18) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 10 + 1]) >> (32 - 18) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 20 + 0]) << (2 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 20 + 0]) >> (33 - 2) );
                                (kc->u.narrow[2 * 20 + 0]) = ( ( (kc->u.narrow[2 * 20 + 1]) << 2) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 20 + 1]) >> (32 - 2) );
                                (kc->u.narrow[2 * 20 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 5 + 0]) << (21 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 5 + 0]) >> (33 - 21) );
                                (kc->u.narrow[2 * 5 + 0]) = ( ( (kc->u.narrow[2 * 5 + 1]) << 21) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 5 + 1]) >> (32 - 21) );
                                (kc->u.narrow[2 * 5 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 15 + 0]) = ( ( (kc->u.narrow[2 * 15 + 0]) << 9) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 15 + 0]) >> (32 - 9) );
                                (kc->u.narrow[2 * 15 + 1]) = ( ( (kc->u.narrow[2 * 15 + 1]) << 9) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 15 + 1]) >> (32 - 9) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = (kc->u.narrow[2 * 2 + 0]);
                                (kc->u.narrow[2 * 2 + 0]) = ( ( (kc->u.narrow[2 * 2 + 1]) << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 2 + 1]) >> 31);
                                (kc->u.narrow[2 * 2 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 12 + 0]) = ( ( (kc->u.narrow[2 * 12 + 0]) << 22) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 12 + 0]) >> (32 - 22) );
                                (kc->u.narrow[2 * 12 + 1]) = ( ( (kc->u.narrow[2 * 12 + 1]) << 22) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 12 + 1]) >> (32 - 22) );
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 22 + 0]) = ( ( (kc->u.narrow[2 * 22 + 0]) << 5) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 22 + 0]) >> (32 - 5) );
                                (kc->u.narrow[2 * 22 + 1]) = ( ( (kc->u.narrow[2 * 22 + 1]) << 5) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 22 + 1]) >> (32 - 5) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 7 + 0]) << (23 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 7 + 0]) >> (33 - 23) );
                                (kc->u.narrow[2 * 7 + 0]) = ( ( (kc->u.narrow[2 * 7 + 1]) << 23) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 7 + 1]) >> (32 - 23) );
                                (kc->u.narrow[2 * 7 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 17 + 0]) = ( ( (kc->u.narrow[2 * 17 + 0]) << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 17 + 0]) >> (32 - 1) );
                                (kc->u.narrow[2 * 17 + 1]) = ( ( (kc->u.narrow[2 * 17 + 1]) << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 17 + 1]) >> (32 - 1) );
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 4 + 0]) = ( ( (kc->u.narrow[2 * 4 + 0]) << 31) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 4 + 0]) >> (32 - 31) );
                                (kc->u.narrow[2 * 4 + 1]) = ( ( (kc->u.narrow[2 * 4 + 1]) << 31) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 4 + 1]) >> (32 - 31) );
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 14 + 0]) = ( ( (kc->u.narrow[2 * 14 + 0]) << 3) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 14 + 0]) >> (32 - 3) );
                                (kc->u.narrow[2 * 14 + 1]) = ( ( (kc->u.narrow[2 * 14 + 1]) << 3) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 14 + 1]) >> (32 - 3) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 24 + 0]) << (22 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 24 + 0]) >> (33 - 22) );
                                (kc->u.narrow[2 * 24 + 0]) = ( ( (kc->u.narrow[2 * 24 + 1]) << 22) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 24 + 1]) >> (32 - 22) );
                                (kc->u.narrow[2 * 24 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 9 + 0]) << (8 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 9 + 0]) >> (33 - 8) );
                                (kc->u.narrow[2 * 9 + 0]) = ( ( (kc->u.narrow[2 * 9 + 1]) << 8) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 9 + 1]) >> (32 - 8) );
                                (kc->u.narrow[2 * 9 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 19 + 0]) << (31 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 19 + 0]) >> (33 - 31) );
                                (kc->u.narrow[2 * 19 + 0]) = ( ( (kc->u.narrow[2 * 19 + 1]) << 31) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 19 + 1]) >> (32 - 31) );
                                (kc->u.narrow[2 * 19 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 1 + 0]) = ( ( (kc->u.narrow[2 * 1 + 0]) << 14) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 1 + 0]) >> (32 - 14) );
                                (kc->u.narrow[2 * 1 + 1]) = ( ( (kc->u.narrow[2 * 1 + 1]) << 14) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 1 + 1]) >> (32 - 14) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 11 + 0]) << (28 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 11 + 0]) >> (33 - 28) );
                                (kc->u.narrow[2 * 11 + 0]) = ( ( (kc->u.narrow[2 * 11 + 1]) << 28) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 11 + 1]) >> (32 - 28) );
                                (kc->u.narrow[2 * 11 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 21 + 0]) << (13 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 21 + 0]) >> (33 - 13) );
                                (kc->u.narrow[2 * 21 + 0]) = ( ( (kc->u.narrow[2 * 21 + 1]) << 13) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 21 + 1]) >> (32 - 13) );
                                (kc->u.narrow[2 * 21 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 6 + 0]) << (11 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 6 + 0]) >> (33 - 11) );
                                (kc->u.narrow[2 * 6 + 0]) = ( ( (kc->u.narrow[2 * 6 + 1]) << 11) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 6 + 1]) >> (32 - 11) );
                                (kc->u.narrow[2 * 6 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 16 + 0]) = ( ( (kc->u.narrow[2 * 16 + 0]) << 28) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 16 + 0]) >> (32 - 28) );
                                (kc->u.narrow[2 * 16 + 1]) = ( ( (kc->u.narrow[2 * 16 + 1]) << 28) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 16 + 1]) >> (32 - 28) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 3 + 0]) << (14 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 3 + 0]) >> (33 - 14) );
                                (kc->u.narrow[2 * 3 + 0]) = ( ( (kc->u.narrow[2 * 3 + 1]) << 14) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 3 + 1]) >> (32 - 14) );
                                (kc->u.narrow[2 * 3 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 13 + 0]) = ( ( (kc->u.narrow[2 * 13 + 0]) << 10) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 13 + 0]) >> (32 - 10) );
                                (kc->u.narrow[2 * 13 + 1]) = ( ( (kc->u.narrow[2 * 13 + 1]) << 10) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 13 + 1]) >> (32 - 10) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 23 + 0]) << (20 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 23 + 0]) >> (33 - 20) );
                                (kc->u.narrow[2 * 23 + 0]) = ( ( (kc->u.narrow[2 * 23 + 1]) << 20) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 23 + 1]) >> (32 - 20) );
                                (kc->u.narrow[2 * 23 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 8 + 0]) = ( ( (kc->u.narrow[2 * 8 + 0]) << 4) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 8 + 0]) >> (32 - 4) );
                                (kc->u.narrow[2 * 8 + 1]) = ( ( (kc->u.narrow[2 * 8 + 1]) << 4) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 8 + 1]) >> (32 - 4) );
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 18 + 0]) = ( ( (kc->u.narrow[2 * 18 + 0]) << 7) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 18 + 0]) >> (32 - 7) );
                                (kc->u.narrow[2 * 18 + 1]) = ( ( (kc->u.narrow[2 * 18 + 1]) << 7) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 18 + 1]) >> (32 - 7) );
                            }
                            while ( 0 );
                        }
                        while ( 0 );
                        do
                        {
                            sph_u64 c0l, c0h;
                            sph_u64 c1l, c1h;
                            sph_u64 c2l, c2h;
                            sph_u64 c3l, c3h;
                            sph_u64 c4l, c4h;
                            sph_u64 bnnl, bnnh;
                            (bnnl = ( (~(kc->u.narrow[2 * 24 + 0]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ), bnnh = ( (~(kc->u.narrow[2 * 24 + 1]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 12 + 0]) | (kc->u.narrow[2 * 24 + 0]), kth = (kc->u.narrow[2 * 12 + 1]) | (kc->u.narrow[2 * 24 + 1]) );
                                (c0l = (kc->u.narrow[2 * 0 + 0]) ^ ktl, c0h = (kc->u.narrow[2 * 0 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = bnnl | (kc->u.narrow[2 * 6 + 0]), kth = bnnh | (kc->u.narrow[2 * 6 + 1]) );
                                (c1l = (kc->u.narrow[2 * 12 + 0]) ^ ktl, c1h = (kc->u.narrow[2 * 12 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 6 + 0]) & (kc->u.narrow[2 * 18 + 0]), kth = (kc->u.narrow[2 * 6 + 1]) & (kc->u.narrow[2 * 18 + 1]) );
                                (c2l = (kc->u.narrow[2 * 24 + 0]) ^ ktl, c2h = (kc->u.narrow[2 * 24 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 18 + 0]) | (kc->u.narrow[2 * 0 + 0]), kth = (kc->u.narrow[2 * 18 + 1]) | (kc->u.narrow[2 * 0 + 1]) );
                                (c3l = (kc->u.narrow[2 * 6 + 0]) ^ ktl, c3h = (kc->u.narrow[2 * 6 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 0 + 0]) & (kc->u.narrow[2 * 12 + 0]), kth = (kc->u.narrow[2 * 0 + 1]) & (kc->u.narrow[2 * 12 + 1]) );
                                (c4l = (kc->u.narrow[2 * 18 + 0]) ^ ktl, c4h = (kc->u.narrow[2 * 18 + 1]) ^ kth);
                            }
                            while ( 0 );
                            ( (kc->u.narrow[2 * 0 + 0]) = c0l, (kc->u.narrow[2 * 0 + 1]) = c0h);
                            ( (kc->u.narrow[2 * 12 + 0]) = c1l, (kc->u.narrow[2 * 12 + 1]) = c1h);
                            ( (kc->u.narrow[2 * 24 + 0]) = c2l, (kc->u.narrow[2 * 24 + 1]) = c2h);
                            ( (kc->u.narrow[2 * 6 + 0]) = c3l, (kc->u.narrow[2 * 6 + 1]) = c3h);
                            ( (kc->u.narrow[2 * 18 + 0]) = c4l, (kc->u.narrow[2 * 18 + 1]) = c4h);
                            (bnnl = ( (~(kc->u.narrow[2 * 19 + 0]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ), bnnh = ( (~(kc->u.narrow[2 * 19 + 1]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 13 + 0]) | (kc->u.narrow[2 * 20 + 0]), kth = (kc->u.narrow[2 * 13 + 1]) | (kc->u.narrow[2 * 20 + 1]) );
                                (c0l = (kc->u.narrow[2 * 1 + 0]) ^ ktl, c0h = (kc->u.narrow[2 * 1 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 20 + 0]) & (kc->u.narrow[2 * 7 + 0]), kth = (kc->u.narrow[2 * 20 + 1]) & (kc->u.narrow[2 * 7 + 1]) );
                                (c1l = (kc->u.narrow[2 * 13 + 0]) ^ ktl, c1h = (kc->u.narrow[2 * 13 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 7 + 0]) | bnnl, kth = (kc->u.narrow[2 * 7 + 1]) | bnnh);
                                (c2l = (kc->u.narrow[2 * 20 + 0]) ^ ktl, c2h = (kc->u.narrow[2 * 20 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 19 + 0]) | (kc->u.narrow[2 * 1 + 0]), kth = (kc->u.narrow[2 * 19 + 1]) | (kc->u.narrow[2 * 1 + 1]) );
                                (c3l = (kc->u.narrow[2 * 7 + 0]) ^ ktl, c3h = (kc->u.narrow[2 * 7 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 1 + 0]) & (kc->u.narrow[2 * 13 + 0]), kth = (kc->u.narrow[2 * 1 + 1]) & (kc->u.narrow[2 * 13 + 1]) );
                                (c4l = (kc->u.narrow[2 * 19 + 0]) ^ ktl, c4h = (kc->u.narrow[2 * 19 + 1]) ^ kth);
                            }
                            while ( 0 );
                            ( (kc->u.narrow[2 * 1 + 0]) = c0l, (kc->u.narrow[2 * 1 + 1]) = c0h);
                            ( (kc->u.narrow[2 * 13 + 0]) = c1l, (kc->u.narrow[2 * 13 + 1]) = c1h);
                            ( (kc->u.narrow[2 * 20 + 0]) = c2l, (kc->u.narrow[2 * 20 + 1]) = c2h);
                            ( (kc->u.narrow[2 * 7 + 0]) = c3l, (kc->u.narrow[2 * 7 + 1]) = c3h);
                            ( (kc->u.narrow[2 * 19 + 0]) = c4l, (kc->u.narrow[2 * 19 + 1]) = c4h);
                            (bnnl = ( (~(kc->u.narrow[2 * 8 + 0]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ), bnnh = ( (~(kc->u.narrow[2 * 8 + 1]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 14 + 0]) | (kc->u.narrow[2 * 21 + 0]), kth = (kc->u.narrow[2 * 14 + 1]) | (kc->u.narrow[2 * 21 + 1]) );
                                (c0l = (kc->u.narrow[2 * 2 + 0]) ^ ktl, c0h = (kc->u.narrow[2 * 2 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 21 + 0]) & (kc->u.narrow[2 * 8 + 0]), kth = (kc->u.narrow[2 * 21 + 1]) & (kc->u.narrow[2 * 8 + 1]) );
                                (c1l = (kc->u.narrow[2 * 14 + 0]) ^ ktl, c1h = (kc->u.narrow[2 * 14 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = bnnl & (kc->u.narrow[2 * 15 + 0]), kth = bnnh & (kc->u.narrow[2 * 15 + 1]) );
                                (c2l = (kc->u.narrow[2 * 21 + 0]) ^ ktl, c2h = (kc->u.narrow[2 * 21 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 15 + 0]) | (kc->u.narrow[2 * 2 + 0]), kth = (kc->u.narrow[2 * 15 + 1]) | (kc->u.narrow[2 * 2 + 1]) );
                                (c3l = bnnl ^ ktl, c3h = bnnh ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 2 + 0]) & (kc->u.narrow[2 * 14 + 0]), kth = (kc->u.narrow[2 * 2 + 1]) & (kc->u.narrow[2 * 14 + 1]) );
                                (c4l = (kc->u.narrow[2 * 15 + 0]) ^ ktl, c4h = (kc->u.narrow[2 * 15 + 1]) ^ kth);
                            }
                            while ( 0 );
                            ( (kc->u.narrow[2 * 2 + 0]) = c0l, (kc->u.narrow[2 * 2 + 1]) = c0h);
                            ( (kc->u.narrow[2 * 14 + 0]) = c1l, (kc->u.narrow[2 * 14 + 1]) = c1h);
                            ( (kc->u.narrow[2 * 21 + 0]) = c2l, (kc->u.narrow[2 * 21 + 1]) = c2h);
                            ( (kc->u.narrow[2 * 8 + 0]) = c3l, (kc->u.narrow[2 * 8 + 1]) = c3h);
                            ( (kc->u.narrow[2 * 15 + 0]) = c4l, (kc->u.narrow[2 * 15 + 1]) = c4h);
                            (bnnl = ( (~(kc->u.narrow[2 * 9 + 0]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ), bnnh = ( (~(kc->u.narrow[2 * 9 + 1]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 10 + 0]) & (kc->u.narrow[2 * 22 + 0]), kth = (kc->u.narrow[2 * 10 + 1]) & (kc->u.narrow[2 * 22 + 1]) );
                                (c0l = (kc->u.narrow[2 * 3 + 0]) ^ ktl, c0h = (kc->u.narrow[2 * 3 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 22 + 0]) | (kc->u.narrow[2 * 9 + 0]), kth = (kc->u.narrow[2 * 22 + 1]) | (kc->u.narrow[2 * 9 + 1]) );
                                (c1l = (kc->u.narrow[2 * 10 + 0]) ^ ktl, c1h = (kc->u.narrow[2 * 10 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = bnnl | (kc->u.narrow[2 * 16 + 0]), kth = bnnh | (kc->u.narrow[2 * 16 + 1]) );
                                (c2l = (kc->u.narrow[2 * 22 + 0]) ^ ktl, c2h = (kc->u.narrow[2 * 22 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 16 + 0]) & (kc->u.narrow[2 * 3 + 0]), kth = (kc->u.narrow[2 * 16 + 1]) & (kc->u.narrow[2 * 3 + 1]) );
                                (c3l = bnnl ^ ktl, c3h = bnnh ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 3 + 0]) | (kc->u.narrow[2 * 10 + 0]), kth = (kc->u.narrow[2 * 3 + 1]) | (kc->u.narrow[2 * 10 + 1]) );
                                (c4l = (kc->u.narrow[2 * 16 + 0]) ^ ktl, c4h = (kc->u.narrow[2 * 16 + 1]) ^ kth);
                            }
                            while ( 0 );
                            ( (kc->u.narrow[2 * 3 + 0]) = c0l, (kc->u.narrow[2 * 3 + 1]) = c0h);
                            ( (kc->u.narrow[2 * 10 + 0]) = c1l, (kc->u.narrow[2 * 10 + 1]) = c1h);
                            ( (kc->u.narrow[2 * 22 + 0]) = c2l, (kc->u.narrow[2 * 22 + 1]) = c2h);
                            ( (kc->u.narrow[2 * 9 + 0]) = c3l, (kc->u.narrow[2 * 9 + 1]) = c3h);
                            ( (kc->u.narrow[2 * 16 + 0]) = c4l, (kc->u.narrow[2 * 16 + 1]) = c4h);
                            (bnnl = ( (~(kc->u.narrow[2 * 11 + 0]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ), bnnh = ( (~(kc->u.narrow[2 * 11 + 1]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = bnnl & (kc->u.narrow[2 * 23 + 0]), kth = bnnh & (kc->u.narrow[2 * 23 + 1]) );
                                (c0l = (kc->u.narrow[2 * 4 + 0]) ^ ktl, c0h = (kc->u.narrow[2 * 4 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 23 + 0]) | (kc->u.narrow[2 * 5 + 0]), kth = (kc->u.narrow[2 * 23 + 1]) | (kc->u.narrow[2 * 5 + 1]) );
                                (c1l = bnnl ^ ktl, c1h = bnnh ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 5 + 0]) & (kc->u.narrow[2 * 17 + 0]), kth = (kc->u.narrow[2 * 5 + 1]) & (kc->u.narrow[2 * 17 + 1]) );
                                (c2l = (kc->u.narrow[2 * 23 + 0]) ^ ktl, c2h = (kc->u.narrow[2 * 23 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 17 + 0]) | (kc->u.narrow[2 * 4 + 0]), kth = (kc->u.narrow[2 * 17 + 1]) | (kc->u.narrow[2 * 4 + 1]) );
                                (c3l = (kc->u.narrow[2 * 5 + 0]) ^ ktl, c3h = (kc->u.narrow[2 * 5 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 4 + 0]) & (kc->u.narrow[2 * 11 + 0]), kth = (kc->u.narrow[2 * 4 + 1]) & (kc->u.narrow[2 * 11 + 1]) );
                                (c4l = (kc->u.narrow[2 * 17 + 0]) ^ ktl, c4h = (kc->u.narrow[2 * 17 + 1]) ^ kth);
                            }
                            while ( 0 );
                            ( (kc->u.narrow[2 * 4 + 0]) = c0l, (kc->u.narrow[2 * 4 + 1]) = c0h);
                            ( (kc->u.narrow[2 * 11 + 0]) = c1l, (kc->u.narrow[2 * 11 + 1]) = c1h);
                            ( (kc->u.narrow[2 * 23 + 0]) = c2l, (kc->u.narrow[2 * 23 + 1]) = c2h);
                            ( (kc->u.narrow[2 * 5 + 0]) = c3l, (kc->u.narrow[2 * 5 + 1]) = c3h);
                            ( (kc->u.narrow[2 * 17 + 0]) = c4l, (kc->u.narrow[2 * 17 + 1]) = c4h);
                        }
                        while ( 0 );
                        ( (kc->u.narrow[2 * 0 + 0]) = (kc->u.narrow[2 * 0 + 0]) ^ RC[j + 6].low, (kc->u.narrow[2 * 0 + 1]) = (kc->u.narrow[2 * 0 + 1]) ^ RC[j + 6].high);
                    }
                    while ( 0 );
                    if(out)
                    {
                        printf( "\n第%d轮中间状态：\n",k++ );
                        print_hex(kc->u.narrow, 1600);
                    }
                    do
                    {
                        do
                        {
                            sph_u64 t0l, t0h;
                            sph_u64 t1l, t1h;
                            sph_u64 t2l, t2h;
                            sph_u64 t3l, t3h;
                            sph_u64 t4l, t4h;
                            do
                            {
                                sph_u64 tt0l, tt0h;
                                sph_u64 tt1l, tt1h;
                                sph_u64 tt2l, tt2h;
                                sph_u64 tt3l, tt3h;
                                (tt0l = (kc->u.narrow[2 * 12 + 0]) ^ (kc->u.narrow[2 * 13 + 0]), tt0h = (kc->u.narrow[2 * 12 + 1]) ^ (kc->u.narrow[2 * 13 + 1]) );
                                (tt1l = (kc->u.narrow[2 * 14 + 0]) ^ (kc->u.narrow[2 * 10 + 0]), tt1h = (kc->u.narrow[2 * 14 + 1]) ^ (kc->u.narrow[2 * 10 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 11 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 11 + 1]) );
                                (tt0l = tt0l ^ tt1l, tt0h = tt0h ^ tt1h);
                                do
                                {
                                    sph_u32 tmp;
                                    tmp = tt0l;
                                    tt0l = ( (tt0h << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | (tt0h >> 31);
                                    tt0h = tmp;
                                }
                                while ( 0 );
                                (tt2l = (kc->u.narrow[2 * 18 + 0]) ^ (kc->u.narrow[2 * 19 + 0]), tt2h = (kc->u.narrow[2 * 18 + 1]) ^ (kc->u.narrow[2 * 19 + 1]) );
                                (tt3l = (kc->u.narrow[2 * 15 + 0]) ^ (kc->u.narrow[2 * 16 + 0]), tt3h = (kc->u.narrow[2 * 15 + 1]) ^ (kc->u.narrow[2 * 16 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 17 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 17 + 1]) );
                                (tt2l = tt2l ^ tt3l, tt2h = tt2h ^ tt3h);
                                (t0l = tt0l ^ tt2l, t0h = tt0h ^ tt2h);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 tt0l, tt0h;
                                sph_u64 tt1l, tt1h;
                                sph_u64 tt2l, tt2h;
                                sph_u64 tt3l, tt3h;
                                (tt0l = (kc->u.narrow[2 * 24 + 0]) ^ (kc->u.narrow[2 * 20 + 0]), tt0h = (kc->u.narrow[2 * 24 + 1]) ^ (kc->u.narrow[2 * 20 + 1]) );
                                (tt1l = (kc->u.narrow[2 * 21 + 0]) ^ (kc->u.narrow[2 * 22 + 0]), tt1h = (kc->u.narrow[2 * 21 + 1]) ^ (kc->u.narrow[2 * 22 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 23 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 23 + 1]) );
                                (tt0l = tt0l ^ tt1l, tt0h = tt0h ^ tt1h);
                                do
                                {
                                    sph_u32 tmp;
                                    tmp = tt0l;
                                    tt0l = ( (tt0h << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | (tt0h >> 31);
                                    tt0h = tmp;
                                }
                                while ( 0 );
                                (tt2l = (kc->u.narrow[2 * 0 + 0]) ^ (kc->u.narrow[2 * 1 + 0]), tt2h = (kc->u.narrow[2 * 0 + 1]) ^ (kc->u.narrow[2 * 1 + 1]) );
                                (tt3l = (kc->u.narrow[2 * 2 + 0]) ^ (kc->u.narrow[2 * 3 + 0]), tt3h = (kc->u.narrow[2 * 2 + 1]) ^ (kc->u.narrow[2 * 3 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 4 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 4 + 1]) );
                                (tt2l = tt2l ^ tt3l, tt2h = tt2h ^ tt3h);
                                (t1l = tt0l ^ tt2l, t1h = tt0h ^ tt2h);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 tt0l, tt0h;
                                sph_u64 tt1l, tt1h;
                                sph_u64 tt2l, tt2h;
                                sph_u64 tt3l, tt3h;
                                (tt0l = (kc->u.narrow[2 * 6 + 0]) ^ (kc->u.narrow[2 * 7 + 0]), tt0h = (kc->u.narrow[2 * 6 + 1]) ^ (kc->u.narrow[2 * 7 + 1]) );
                                (tt1l = (kc->u.narrow[2 * 8 + 0]) ^ (kc->u.narrow[2 * 9 + 0]), tt1h = (kc->u.narrow[2 * 8 + 1]) ^ (kc->u.narrow[2 * 9 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 5 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 5 + 1]) );
                                (tt0l = tt0l ^ tt1l, tt0h = tt0h ^ tt1h);
                                do
                                {
                                    sph_u32 tmp;
                                    tmp = tt0l;
                                    tt0l = ( (tt0h << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | (tt0h >> 31);
                                    tt0h = tmp;
                                }
                                while ( 0 );
                                (tt2l = (kc->u.narrow[2 * 12 + 0]) ^ (kc->u.narrow[2 * 13 + 0]), tt2h = (kc->u.narrow[2 * 12 + 1]) ^ (kc->u.narrow[2 * 13 + 1]) );
                                (tt3l = (kc->u.narrow[2 * 14 + 0]) ^ (kc->u.narrow[2 * 10 + 0]), tt3h = (kc->u.narrow[2 * 14 + 1]) ^ (kc->u.narrow[2 * 10 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 11 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 11 + 1]) );
                                (tt2l = tt2l ^ tt3l, tt2h = tt2h ^ tt3h);
                                (t2l = tt0l ^ tt2l, t2h = tt0h ^ tt2h);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 tt0l, tt0h;
                                sph_u64 tt1l, tt1h;
                                sph_u64 tt2l, tt2h;
                                sph_u64 tt3l, tt3h;
                                (tt0l = (kc->u.narrow[2 * 18 + 0]) ^ (kc->u.narrow[2 * 19 + 0]), tt0h = (kc->u.narrow[2 * 18 + 1]) ^ (kc->u.narrow[2 * 19 + 1]) );
                                (tt1l = (kc->u.narrow[2 * 15 + 0]) ^ (kc->u.narrow[2 * 16 + 0]), tt1h = (kc->u.narrow[2 * 15 + 1]) ^ (kc->u.narrow[2 * 16 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 17 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 17 + 1]) );
                                (tt0l = tt0l ^ tt1l, tt0h = tt0h ^ tt1h);
                                do
                                {
                                    sph_u32 tmp;
                                    tmp = tt0l;
                                    tt0l = ( (tt0h << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | (tt0h >> 31);
                                    tt0h = tmp;
                                }
                                while ( 0 );
                                (tt2l = (kc->u.narrow[2 * 24 + 0]) ^ (kc->u.narrow[2 * 20 + 0]), tt2h = (kc->u.narrow[2 * 24 + 1]) ^ (kc->u.narrow[2 * 20 + 1]) );
                                (tt3l = (kc->u.narrow[2 * 21 + 0]) ^ (kc->u.narrow[2 * 22 + 0]), tt3h = (kc->u.narrow[2 * 21 + 1]) ^ (kc->u.narrow[2 * 22 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 23 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 23 + 1]) );
                                (tt2l = tt2l ^ tt3l, tt2h = tt2h ^ tt3h);
                                (t3l = tt0l ^ tt2l, t3h = tt0h ^ tt2h);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 tt0l, tt0h;
                                sph_u64 tt1l, tt1h;
                                sph_u64 tt2l, tt2h;
                                sph_u64 tt3l, tt3h;
                                (tt0l = (kc->u.narrow[2 * 0 + 0]) ^ (kc->u.narrow[2 * 1 + 0]), tt0h = (kc->u.narrow[2 * 0 + 1]) ^ (kc->u.narrow[2 * 1 + 1]) );
                                (tt1l = (kc->u.narrow[2 * 2 + 0]) ^ (kc->u.narrow[2 * 3 + 0]), tt1h = (kc->u.narrow[2 * 2 + 1]) ^ (kc->u.narrow[2 * 3 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 4 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 4 + 1]) );
                                (tt0l = tt0l ^ tt1l, tt0h = tt0h ^ tt1h);
                                do
                                {
                                    sph_u32 tmp;
                                    tmp = tt0l;
                                    tt0l = ( (tt0h << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | (tt0h >> 31);
                                    tt0h = tmp;
                                }
                                while ( 0 );
                                (tt2l = (kc->u.narrow[2 * 6 + 0]) ^ (kc->u.narrow[2 * 7 + 0]), tt2h = (kc->u.narrow[2 * 6 + 1]) ^ (kc->u.narrow[2 * 7 + 1]) );
                                (tt3l = (kc->u.narrow[2 * 8 + 0]) ^ (kc->u.narrow[2 * 9 + 0]), tt3h = (kc->u.narrow[2 * 8 + 1]) ^ (kc->u.narrow[2 * 9 + 1]) );
                                (tt0l = tt0l ^ (kc->u.narrow[2 * 5 + 0]), tt0h = tt0h ^ (kc->u.narrow[2 * 5 + 1]) );
                                (tt2l = tt2l ^ tt3l, tt2h = tt2h ^ tt3h);
                                (t4l = tt0l ^ tt2l, t4h = tt0h ^ tt2h);
                            }
                            while ( 0 );
                            ( (kc->u.narrow[2 * 0 + 0]) = (kc->u.narrow[2 * 0 + 0]) ^ t0l, (kc->u.narrow[2 * 0 + 1]) = (kc->u.narrow[2 * 0 + 1]) ^ t0h);
                            ( (kc->u.narrow[2 * 1 + 0]) = (kc->u.narrow[2 * 1 + 0]) ^ t0l, (kc->u.narrow[2 * 1 + 1]) = (kc->u.narrow[2 * 1 + 1]) ^ t0h);
                            ( (kc->u.narrow[2 * 2 + 0]) = (kc->u.narrow[2 * 2 + 0]) ^ t0l, (kc->u.narrow[2 * 2 + 1]) = (kc->u.narrow[2 * 2 + 1]) ^ t0h);
                            ( (kc->u.narrow[2 * 3 + 0]) = (kc->u.narrow[2 * 3 + 0]) ^ t0l, (kc->u.narrow[2 * 3 + 1]) = (kc->u.narrow[2 * 3 + 1]) ^ t0h);
                            ( (kc->u.narrow[2 * 4 + 0]) = (kc->u.narrow[2 * 4 + 0]) ^ t0l, (kc->u.narrow[2 * 4 + 1]) = (kc->u.narrow[2 * 4 + 1]) ^ t0h);
                            ( (kc->u.narrow[2 * 12 + 0]) = (kc->u.narrow[2 * 12 + 0]) ^ t1l, (kc->u.narrow[2 * 12 + 1]) = (kc->u.narrow[2 * 12 + 1]) ^ t1h);
                            ( (kc->u.narrow[2 * 13 + 0]) = (kc->u.narrow[2 * 13 + 0]) ^ t1l, (kc->u.narrow[2 * 13 + 1]) = (kc->u.narrow[2 * 13 + 1]) ^ t1h);
                            ( (kc->u.narrow[2 * 14 + 0]) = (kc->u.narrow[2 * 14 + 0]) ^ t1l, (kc->u.narrow[2 * 14 + 1]) = (kc->u.narrow[2 * 14 + 1]) ^ t1h);
                            ( (kc->u.narrow[2 * 10 + 0]) = (kc->u.narrow[2 * 10 + 0]) ^ t1l, (kc->u.narrow[2 * 10 + 1]) = (kc->u.narrow[2 * 10 + 1]) ^ t1h);
                            ( (kc->u.narrow[2 * 11 + 0]) = (kc->u.narrow[2 * 11 + 0]) ^ t1l, (kc->u.narrow[2 * 11 + 1]) = (kc->u.narrow[2 * 11 + 1]) ^ t1h);
                            ( (kc->u.narrow[2 * 24 + 0]) = (kc->u.narrow[2 * 24 + 0]) ^ t2l, (kc->u.narrow[2 * 24 + 1]) = (kc->u.narrow[2 * 24 + 1]) ^ t2h);
                            ( (kc->u.narrow[2 * 20 + 0]) = (kc->u.narrow[2 * 20 + 0]) ^ t2l, (kc->u.narrow[2 * 20 + 1]) = (kc->u.narrow[2 * 20 + 1]) ^ t2h);
                            ( (kc->u.narrow[2 * 21 + 0]) = (kc->u.narrow[2 * 21 + 0]) ^ t2l, (kc->u.narrow[2 * 21 + 1]) = (kc->u.narrow[2 * 21 + 1]) ^ t2h);
                            ( (kc->u.narrow[2 * 22 + 0]) = (kc->u.narrow[2 * 22 + 0]) ^ t2l, (kc->u.narrow[2 * 22 + 1]) = (kc->u.narrow[2 * 22 + 1]) ^ t2h);
                            ( (kc->u.narrow[2 * 23 + 0]) = (kc->u.narrow[2 * 23 + 0]) ^ t2l, (kc->u.narrow[2 * 23 + 1]) = (kc->u.narrow[2 * 23 + 1]) ^ t2h);
                            ( (kc->u.narrow[2 * 6 + 0]) = (kc->u.narrow[2 * 6 + 0]) ^ t3l, (kc->u.narrow[2 * 6 + 1]) = (kc->u.narrow[2 * 6 + 1]) ^ t3h);
                            ( (kc->u.narrow[2 * 7 + 0]) = (kc->u.narrow[2 * 7 + 0]) ^ t3l, (kc->u.narrow[2 * 7 + 1]) = (kc->u.narrow[2 * 7 + 1]) ^ t3h);
                            ( (kc->u.narrow[2 * 8 + 0]) = (kc->u.narrow[2 * 8 + 0]) ^ t3l, (kc->u.narrow[2 * 8 + 1]) = (kc->u.narrow[2 * 8 + 1]) ^ t3h);
                            ( (kc->u.narrow[2 * 9 + 0]) = (kc->u.narrow[2 * 9 + 0]) ^ t3l, (kc->u.narrow[2 * 9 + 1]) = (kc->u.narrow[2 * 9 + 1]) ^ t3h);
                            ( (kc->u.narrow[2 * 5 + 0]) = (kc->u.narrow[2 * 5 + 0]) ^ t3l, (kc->u.narrow[2 * 5 + 1]) = (kc->u.narrow[2 * 5 + 1]) ^ t3h);
                            ( (kc->u.narrow[2 * 18 + 0]) = (kc->u.narrow[2 * 18 + 0]) ^ t4l, (kc->u.narrow[2 * 18 + 1]) = (kc->u.narrow[2 * 18 + 1]) ^ t4h);
                            ( (kc->u.narrow[2 * 19 + 0]) = (kc->u.narrow[2 * 19 + 0]) ^ t4l, (kc->u.narrow[2 * 19 + 1]) = (kc->u.narrow[2 * 19 + 1]) ^ t4h);
                            ( (kc->u.narrow[2 * 15 + 0]) = (kc->u.narrow[2 * 15 + 0]) ^ t4l, (kc->u.narrow[2 * 15 + 1]) = (kc->u.narrow[2 * 15 + 1]) ^ t4h);
                            ( (kc->u.narrow[2 * 16 + 0]) = (kc->u.narrow[2 * 16 + 0]) ^ t4l, (kc->u.narrow[2 * 16 + 1]) = (kc->u.narrow[2 * 16 + 1]) ^ t4h);
                            ( (kc->u.narrow[2 * 17 + 0]) = (kc->u.narrow[2 * 17 + 0]) ^ t4l, (kc->u.narrow[2 * 17 + 1]) = (kc->u.narrow[2 * 17 + 1]) ^ t4h);
                        }
                        while ( 0 );
                        do
                        {
                            do
                            {
                                (kc->u.narrow[2 * 1 + 0]) = ( ( (kc->u.narrow[2 * 1 + 0]) << 18) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 1 + 0]) >> (32 - 18) );
                                (kc->u.narrow[2 * 1 + 1]) = ( ( (kc->u.narrow[2 * 1 + 1]) << 18) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 1 + 1]) >> (32 - 18) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 2 + 0]) << (2 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 2 + 0]) >> (33 - 2) );
                                (kc->u.narrow[2 * 2 + 0]) = ( ( (kc->u.narrow[2 * 2 + 1]) << 2) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 2 + 1]) >> (32 - 2) );
                                (kc->u.narrow[2 * 2 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 3 + 0]) << (21 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 3 + 0]) >> (33 - 21) );
                                (kc->u.narrow[2 * 3 + 0]) = ( ( (kc->u.narrow[2 * 3 + 1]) << 21) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 3 + 1]) >> (32 - 21) );
                                (kc->u.narrow[2 * 3 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 4 + 0]) = ( ( (kc->u.narrow[2 * 4 + 0]) << 9) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 4 + 0]) >> (32 - 9) );
                                (kc->u.narrow[2 * 4 + 1]) = ( ( (kc->u.narrow[2 * 4 + 1]) << 9) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 4 + 1]) >> (32 - 9) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = (kc->u.narrow[2 * 12 + 0]);
                                (kc->u.narrow[2 * 12 + 0]) = ( ( (kc->u.narrow[2 * 12 + 1]) << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 12 + 1]) >> 31);
                                (kc->u.narrow[2 * 12 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 13 + 0]) = ( ( (kc->u.narrow[2 * 13 + 0]) << 22) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 13 + 0]) >> (32 - 22) );
                                (kc->u.narrow[2 * 13 + 1]) = ( ( (kc->u.narrow[2 * 13 + 1]) << 22) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 13 + 1]) >> (32 - 22) );
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 14 + 0]) = ( ( (kc->u.narrow[2 * 14 + 0]) << 5) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 14 + 0]) >> (32 - 5) );
                                (kc->u.narrow[2 * 14 + 1]) = ( ( (kc->u.narrow[2 * 14 + 1]) << 5) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 14 + 1]) >> (32 - 5) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 10 + 0]) << (23 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 10 + 0]) >> (33 - 23) );
                                (kc->u.narrow[2 * 10 + 0]) = ( ( (kc->u.narrow[2 * 10 + 1]) << 23) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 10 + 1]) >> (32 - 23) );
                                (kc->u.narrow[2 * 10 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 11 + 0]) = ( ( (kc->u.narrow[2 * 11 + 0]) << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 11 + 0]) >> (32 - 1) );
                                (kc->u.narrow[2 * 11 + 1]) = ( ( (kc->u.narrow[2 * 11 + 1]) << 1) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 11 + 1]) >> (32 - 1) );
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 24 + 0]) = ( ( (kc->u.narrow[2 * 24 + 0]) << 31) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 24 + 0]) >> (32 - 31) );
                                (kc->u.narrow[2 * 24 + 1]) = ( ( (kc->u.narrow[2 * 24 + 1]) << 31) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 24 + 1]) >> (32 - 31) );
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 20 + 0]) = ( ( (kc->u.narrow[2 * 20 + 0]) << 3) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 20 + 0]) >> (32 - 3) );
                                (kc->u.narrow[2 * 20 + 1]) = ( ( (kc->u.narrow[2 * 20 + 1]) << 3) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 20 + 1]) >> (32 - 3) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 21 + 0]) << (22 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 21 + 0]) >> (33 - 22) );
                                (kc->u.narrow[2 * 21 + 0]) = ( ( (kc->u.narrow[2 * 21 + 1]) << 22) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 21 + 1]) >> (32 - 22) );
                                (kc->u.narrow[2 * 21 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 22 + 0]) << (8 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 22 + 0]) >> (33 - 8) );
                                (kc->u.narrow[2 * 22 + 0]) = ( ( (kc->u.narrow[2 * 22 + 1]) << 8) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 22 + 1]) >> (32 - 8) );
                                (kc->u.narrow[2 * 22 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 23 + 0]) << (31 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 23 + 0]) >> (33 - 31) );
                                (kc->u.narrow[2 * 23 + 0]) = ( ( (kc->u.narrow[2 * 23 + 1]) << 31) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 23 + 1]) >> (32 - 31) );
                                (kc->u.narrow[2 * 23 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 6 + 0]) = ( ( (kc->u.narrow[2 * 6 + 0]) << 14) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 6 + 0]) >> (32 - 14) );
                                (kc->u.narrow[2 * 6 + 1]) = ( ( (kc->u.narrow[2 * 6 + 1]) << 14) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 6 + 1]) >> (32 - 14) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 7 + 0]) << (28 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 7 + 0]) >> (33 - 28) );
                                (kc->u.narrow[2 * 7 + 0]) = ( ( (kc->u.narrow[2 * 7 + 1]) << 28) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 7 + 1]) >> (32 - 28) );
                                (kc->u.narrow[2 * 7 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 8 + 0]) << (13 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 8 + 0]) >> (33 - 13) );
                                (kc->u.narrow[2 * 8 + 0]) = ( ( (kc->u.narrow[2 * 8 + 1]) << 13) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 8 + 1]) >> (32 - 13) );
                                (kc->u.narrow[2 * 8 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 9 + 0]) << (11 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 9 + 0]) >> (33 - 11) );
                                (kc->u.narrow[2 * 9 + 0]) = ( ( (kc->u.narrow[2 * 9 + 1]) << 11) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 9 + 1]) >> (32 - 11) );
                                (kc->u.narrow[2 * 9 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 5 + 0]) = ( ( (kc->u.narrow[2 * 5 + 0]) << 28) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 5 + 0]) >> (32 - 28) );
                                (kc->u.narrow[2 * 5 + 1]) = ( ( (kc->u.narrow[2 * 5 + 1]) << 28) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 5 + 1]) >> (32 - 28) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 18 + 0]) << (14 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 18 + 0]) >> (33 - 14) );
                                (kc->u.narrow[2 * 18 + 0]) = ( ( (kc->u.narrow[2 * 18 + 1]) << 14) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 18 + 1]) >> (32 - 14) );
                                (kc->u.narrow[2 * 18 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 19 + 0]) = ( ( (kc->u.narrow[2 * 19 + 0]) << 10) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 19 + 0]) >> (32 - 10) );
                                (kc->u.narrow[2 * 19 + 1]) = ( ( (kc->u.narrow[2 * 19 + 1]) << 10) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 19 + 1]) >> (32 - 10) );
                            }
                            while ( 0 );
                            do
                            {
                                sph_u32 tmp;
                                tmp = ( ( (kc->u.narrow[2 * 15 + 0]) << (20 - 1) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 15 + 0]) >> (33 - 20) );
                                (kc->u.narrow[2 * 15 + 0]) = ( ( (kc->u.narrow[2 * 15 + 1]) << 20) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 15 + 1]) >> (32 - 20) );
                                (kc->u.narrow[2 * 15 + 1]) = tmp;
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 16 + 0]) = ( ( (kc->u.narrow[2 * 16 + 0]) << 4) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 16 + 0]) >> (32 - 4) );
                                (kc->u.narrow[2 * 16 + 1]) = ( ( (kc->u.narrow[2 * 16 + 1]) << 4) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 16 + 1]) >> (32 - 4) );
                            }
                            while ( 0 );
                            do
                            {
                                (kc->u.narrow[2 * 17 + 0]) = ( ( (kc->u.narrow[2 * 17 + 0]) << 7) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 17 + 0]) >> (32 - 7) );
                                (kc->u.narrow[2 * 17 + 1]) = ( ( (kc->u.narrow[2 * 17 + 1]) << 7) & ( (sph_u32) (0xFFFFFFFFU) ) ) | ( (kc->u.narrow[2 * 17 + 1]) >> (32 - 7) );
                            }
                            while ( 0 );
                        }
                        while ( 0 );
                        do
                        {
                            sph_u64 c0l, c0h;
                            sph_u64 c1l, c1h;
                            sph_u64 c2l, c2h;
                            sph_u64 c3l, c3h;
                            sph_u64 c4l, c4h;
                            sph_u64 bnnl, bnnh;
                            (bnnl = ( (~(kc->u.narrow[2 * 21 + 0]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ), bnnh = ( (~(kc->u.narrow[2 * 21 + 1]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 13 + 0]) | (kc->u.narrow[2 * 21 + 0]), kth = (kc->u.narrow[2 * 13 + 1]) | (kc->u.narrow[2 * 21 + 1]) );
                                (c0l = (kc->u.narrow[2 * 0 + 0]) ^ ktl, c0h = (kc->u.narrow[2 * 0 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = bnnl | (kc->u.narrow[2 * 9 + 0]), kth = bnnh | (kc->u.narrow[2 * 9 + 1]) );
                                (c1l = (kc->u.narrow[2 * 13 + 0]) ^ ktl, c1h = (kc->u.narrow[2 * 13 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 9 + 0]) & (kc->u.narrow[2 * 17 + 0]), kth = (kc->u.narrow[2 * 9 + 1]) & (kc->u.narrow[2 * 17 + 1]) );
                                (c2l = (kc->u.narrow[2 * 21 + 0]) ^ ktl, c2h = (kc->u.narrow[2 * 21 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 17 + 0]) | (kc->u.narrow[2 * 0 + 0]), kth = (kc->u.narrow[2 * 17 + 1]) | (kc->u.narrow[2 * 0 + 1]) );
                                (c3l = (kc->u.narrow[2 * 9 + 0]) ^ ktl, c3h = (kc->u.narrow[2 * 9 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 0 + 0]) & (kc->u.narrow[2 * 13 + 0]), kth = (kc->u.narrow[2 * 0 + 1]) & (kc->u.narrow[2 * 13 + 1]) );
                                (c4l = (kc->u.narrow[2 * 17 + 0]) ^ ktl, c4h = (kc->u.narrow[2 * 17 + 1]) ^ kth);
                            }
                            while ( 0 );
                            ( (kc->u.narrow[2 * 0 + 0]) = c0l, (kc->u.narrow[2 * 0 + 1]) = c0h);
                            ( (kc->u.narrow[2 * 13 + 0]) = c1l, (kc->u.narrow[2 * 13 + 1]) = c1h);
                            ( (kc->u.narrow[2 * 21 + 0]) = c2l, (kc->u.narrow[2 * 21 + 1]) = c2h);
                            ( (kc->u.narrow[2 * 9 + 0]) = c3l, (kc->u.narrow[2 * 9 + 1]) = c3h);
                            ( (kc->u.narrow[2 * 17 + 0]) = c4l, (kc->u.narrow[2 * 17 + 1]) = c4h);
                            (bnnl = ( (~(kc->u.narrow[2 * 23 + 0]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ), bnnh = ( (~(kc->u.narrow[2 * 23 + 1]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 19 + 0]) | (kc->u.narrow[2 * 2 + 0]), kth = (kc->u.narrow[2 * 19 + 1]) | (kc->u.narrow[2 * 2 + 1]) );
                                (c0l = (kc->u.narrow[2 * 6 + 0]) ^ ktl, c0h = (kc->u.narrow[2 * 6 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 2 + 0]) & (kc->u.narrow[2 * 10 + 0]), kth = (kc->u.narrow[2 * 2 + 1]) & (kc->u.narrow[2 * 10 + 1]) );
                                (c1l = (kc->u.narrow[2 * 19 + 0]) ^ ktl, c1h = (kc->u.narrow[2 * 19 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 10 + 0]) | bnnl, kth = (kc->u.narrow[2 * 10 + 1]) | bnnh);
                                (c2l = (kc->u.narrow[2 * 2 + 0]) ^ ktl, c2h = (kc->u.narrow[2 * 2 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 23 + 0]) | (kc->u.narrow[2 * 6 + 0]), kth = (kc->u.narrow[2 * 23 + 1]) | (kc->u.narrow[2 * 6 + 1]) );
                                (c3l = (kc->u.narrow[2 * 10 + 0]) ^ ktl, c3h = (kc->u.narrow[2 * 10 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 6 + 0]) & (kc->u.narrow[2 * 19 + 0]), kth = (kc->u.narrow[2 * 6 + 1]) & (kc->u.narrow[2 * 19 + 1]) );
                                (c4l = (kc->u.narrow[2 * 23 + 0]) ^ ktl, c4h = (kc->u.narrow[2 * 23 + 1]) ^ kth);
                            }
                            while ( 0 );
                            ( (kc->u.narrow[2 * 6 + 0]) = c0l, (kc->u.narrow[2 * 6 + 1]) = c0h);
                            ( (kc->u.narrow[2 * 19 + 0]) = c1l, (kc->u.narrow[2 * 19 + 1]) = c1h);
                            ( (kc->u.narrow[2 * 2 + 0]) = c2l, (kc->u.narrow[2 * 2 + 1]) = c2h);
                            ( (kc->u.narrow[2 * 10 + 0]) = c3l, (kc->u.narrow[2 * 10 + 1]) = c3h);
                            ( (kc->u.narrow[2 * 23 + 0]) = c4l, (kc->u.narrow[2 * 23 + 1]) = c4h);
                            (bnnl = ( (~(kc->u.narrow[2 * 16 + 0]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ), bnnh = ( (~(kc->u.narrow[2 * 16 + 1]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 20 + 0]) | (kc->u.narrow[2 * 8 + 0]), kth = (kc->u.narrow[2 * 20 + 1]) | (kc->u.narrow[2 * 8 + 1]) );
                                (c0l = (kc->u.narrow[2 * 12 + 0]) ^ ktl, c0h = (kc->u.narrow[2 * 12 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 8 + 0]) & (kc->u.narrow[2 * 16 + 0]), kth = (kc->u.narrow[2 * 8 + 1]) & (kc->u.narrow[2 * 16 + 1]) );
                                (c1l = (kc->u.narrow[2 * 20 + 0]) ^ ktl, c1h = (kc->u.narrow[2 * 20 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = bnnl & (kc->u.narrow[2 * 4 + 0]), kth = bnnh & (kc->u.narrow[2 * 4 + 1]) );
                                (c2l = (kc->u.narrow[2 * 8 + 0]) ^ ktl, c2h = (kc->u.narrow[2 * 8 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 4 + 0]) | (kc->u.narrow[2 * 12 + 0]), kth = (kc->u.narrow[2 * 4 + 1]) | (kc->u.narrow[2 * 12 + 1]) );
                                (c3l = bnnl ^ ktl, c3h = bnnh ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 12 + 0]) & (kc->u.narrow[2 * 20 + 0]), kth = (kc->u.narrow[2 * 12 + 1]) & (kc->u.narrow[2 * 20 + 1]) );
                                (c4l = (kc->u.narrow[2 * 4 + 0]) ^ ktl, c4h = (kc->u.narrow[2 * 4 + 1]) ^ kth);
                            }
                            while ( 0 );
                            ( (kc->u.narrow[2 * 12 + 0]) = c0l, (kc->u.narrow[2 * 12 + 1]) = c0h);
                            ( (kc->u.narrow[2 * 20 + 0]) = c1l, (kc->u.narrow[2 * 20 + 1]) = c1h);
                            ( (kc->u.narrow[2 * 8 + 0]) = c2l, (kc->u.narrow[2 * 8 + 1]) = c2h);
                            ( (kc->u.narrow[2 * 16 + 0]) = c3l, (kc->u.narrow[2 * 16 + 1]) = c3h);
                            ( (kc->u.narrow[2 * 4 + 0]) = c4l, (kc->u.narrow[2 * 4 + 1]) = c4h);
                            (bnnl = ( (~(kc->u.narrow[2 * 22 + 0]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ), bnnh = ( (~(kc->u.narrow[2 * 22 + 1]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 1 + 0]) & (kc->u.narrow[2 * 14 + 0]), kth = (kc->u.narrow[2 * 1 + 1]) & (kc->u.narrow[2 * 14 + 1]) );
                                (c0l = (kc->u.narrow[2 * 18 + 0]) ^ ktl, c0h = (kc->u.narrow[2 * 18 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 14 + 0]) | (kc->u.narrow[2 * 22 + 0]), kth = (kc->u.narrow[2 * 14 + 1]) | (kc->u.narrow[2 * 22 + 1]) );
                                (c1l = (kc->u.narrow[2 * 1 + 0]) ^ ktl, c1h = (kc->u.narrow[2 * 1 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = bnnl | (kc->u.narrow[2 * 5 + 0]), kth = bnnh | (kc->u.narrow[2 * 5 + 1]) );
                                (c2l = (kc->u.narrow[2 * 14 + 0]) ^ ktl, c2h = (kc->u.narrow[2 * 14 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 5 + 0]) & (kc->u.narrow[2 * 18 + 0]), kth = (kc->u.narrow[2 * 5 + 1]) & (kc->u.narrow[2 * 18 + 1]) );
                                (c3l = bnnl ^ ktl, c3h = bnnh ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 18 + 0]) | (kc->u.narrow[2 * 1 + 0]), kth = (kc->u.narrow[2 * 18 + 1]) | (kc->u.narrow[2 * 1 + 1]) );
                                (c4l = (kc->u.narrow[2 * 5 + 0]) ^ ktl, c4h = (kc->u.narrow[2 * 5 + 1]) ^ kth);
                            }
                            while ( 0 );
                            ( (kc->u.narrow[2 * 18 + 0]) = c0l, (kc->u.narrow[2 * 18 + 1]) = c0h);
                            ( (kc->u.narrow[2 * 1 + 0]) = c1l, (kc->u.narrow[2 * 1 + 1]) = c1h);
                            ( (kc->u.narrow[2 * 14 + 0]) = c2l, (kc->u.narrow[2 * 14 + 1]) = c2h);
                            ( (kc->u.narrow[2 * 22 + 0]) = c3l, (kc->u.narrow[2 * 22 + 1]) = c3h);
                            ( (kc->u.narrow[2 * 5 + 0]) = c4l, (kc->u.narrow[2 * 5 + 1]) = c4h);
                            (bnnl = ( (~(kc->u.narrow[2 * 7 + 0]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ), bnnh = ( (~(kc->u.narrow[2 * 7 + 1]) ) & ( (sph_u32) (0xFFFFFFFFU) ) ) );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = bnnl & (kc->u.narrow[2 * 15 + 0]), kth = bnnh & (kc->u.narrow[2 * 15 + 1]) );
                                (c0l = (kc->u.narrow[2 * 24 + 0]) ^ ktl, c0h = (kc->u.narrow[2 * 24 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 15 + 0]) | (kc->u.narrow[2 * 3 + 0]), kth = (kc->u.narrow[2 * 15 + 1]) | (kc->u.narrow[2 * 3 + 1]) );
                                (c1l = bnnl ^ ktl, c1h = bnnh ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 3 + 0]) & (kc->u.narrow[2 * 11 + 0]), kth = (kc->u.narrow[2 * 3 + 1]) & (kc->u.narrow[2 * 11 + 1]) );
                                (c2l = (kc->u.narrow[2 * 15 + 0]) ^ ktl, c2h = (kc->u.narrow[2 * 15 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 11 + 0]) | (kc->u.narrow[2 * 24 + 0]), kth = (kc->u.narrow[2 * 11 + 1]) | (kc->u.narrow[2 * 24 + 1]) );
                                (c3l = (kc->u.narrow[2 * 3 + 0]) ^ ktl, c3h = (kc->u.narrow[2 * 3 + 1]) ^ kth);
                            }
                            while ( 0 );
                            do
                            {
                                sph_u64 ktl, kth;
                                (ktl = (kc->u.narrow[2 * 24 + 0]) & (kc->u.narrow[2 * 7 + 0]), kth = (kc->u.narrow[2 * 24 + 1]) & (kc->u.narrow[2 * 7 + 1]) );
                                (c4l = (kc->u.narrow[2 * 11 + 0]) ^ ktl, c4h = (kc->u.narrow[2 * 11 + 1]) ^ kth);
                            }
                            while ( 0 );
                            ( (kc->u.narrow[2 * 24 + 0]) = c0l, (kc->u.narrow[2 * 24 + 1]) = c0h);
                            ( (kc->u.narrow[2 * 7 + 0]) = c1l, (kc->u.narrow[2 * 7 + 1]) = c1h);
                            ( (kc->u.narrow[2 * 15 + 0]) = c2l, (kc->u.narrow[2 * 15 + 1]) = c2h);
                            ( (kc->u.narrow[2 * 3 + 0]) = c3l, (kc->u.narrow[2 * 3 + 1]) = c3h);
                            ( (kc->u.narrow[2 * 11 + 0]) = c4l, (kc->u.narrow[2 * 11 + 1]) = c4h);
                        }
                        while ( 0 );
                        ( (kc->u.narrow[2 * 0 + 0]) = (kc->u.narrow[2 * 0 + 0]) ^ RC[j + 7].low, (kc->u.narrow[2 * 0 + 1]) = (kc->u.narrow[2 * 0 + 1]) ^ RC[j + 7].high);
                    }
                    while ( 0 );
                    if(out)
                    {
                        printf( "\n第%d轮中间状态：\n",k++ );
                        print_hex(kc->u.narrow, 1600);
                    }
                    do
                    {
                        sph_u64 tl, th;
                        (tl = (kc->u.narrow[2 * 5 + 0]), th = (kc->u.narrow[2 * 5 + 1]) );
                        ( (kc->u.narrow[2 * 5 + 0]) = (kc->u.narrow[2 * 6 + 0]), (kc->u.narrow[2 * 5 + 1]) = (kc->u.narrow[2 * 6 + 1]) );
                        ( (kc->u.narrow[2 * 6 + 0]) = (kc->u.narrow[2 * 19 + 0]), (kc->u.narrow[2 * 6 + 1]) = (kc->u.narrow[2 * 19 + 1]) );
                        ( (kc->u.narrow[2 * 19 + 0]) = tl, (kc->u.narrow[2 * 19 + 1]) = th);
                        (tl = (kc->u.narrow[2 * 10 + 0]), th = (kc->u.narrow[2 * 10 + 1]) );
                        ( (kc->u.narrow[2 * 10 + 0]) = (kc->u.narrow[2 * 12 + 0]), (kc->u.narrow[2 * 10 + 1]) = (kc->u.narrow[2 * 12 + 1]) );
                        ( (kc->u.narrow[2 * 12 + 0]) = (kc->u.narrow[2 * 8 + 0]), (kc->u.narrow[2 * 12 + 1]) = (kc->u.narrow[2 * 8 + 1]) );
                        ( (kc->u.narrow[2 * 8 + 0]) = tl, (kc->u.narrow[2 * 8 + 1]) = th);
                        (tl = (kc->u.narrow[2 * 15 + 0]), th = (kc->u.narrow[2 * 15 + 1]) );
                        ( (kc->u.narrow[2 * 15 + 0]) = (kc->u.narrow[2 * 18 + 0]), (kc->u.narrow[2 * 15 + 1]) = (kc->u.narrow[2 * 18 + 1]) );
                        ( (kc->u.narrow[2 * 18 + 0]) = (kc->u.narrow[2 * 22 + 0]), (kc->u.narrow[2 * 18 + 1]) = (kc->u.narrow[2 * 22 + 1]) );
                        ( (kc->u.narrow[2 * 22 + 0]) = tl, (kc->u.narrow[2 * 22 + 1]) = th);
                        (tl = (kc->u.narrow[2 * 20 + 0]), th = (kc->u.narrow[2 * 20 + 1]) );
                        ( (kc->u.narrow[2 * 20 + 0]) = (kc->u.narrow[2 * 24 + 0]), (kc->u.narrow[2 * 20 + 1]) = (kc->u.narrow[2 * 24 + 1]) );
                        ( (kc->u.narrow[2 * 24 + 0]) = (kc->u.narrow[2 * 11 + 0]), (kc->u.narrow[2 * 24 + 1]) = (kc->u.narrow[2 * 11 + 1]) );
                        ( (kc->u.narrow[2 * 11 + 0]) = tl, (kc->u.narrow[2 * 11 + 1]) = th);
                        (tl = (kc->u.narrow[2 * 1 + 0]), th = (kc->u.narrow[2 * 1 + 1]) );
                        ( (kc->u.narrow[2 * 1 + 0]) = (kc->u.narrow[2 * 13 + 0]), (kc->u.narrow[2 * 1 + 1]) = (kc->u.narrow[2 * 13 + 1]) );
                        ( (kc->u.narrow[2 * 13 + 0]) = (kc->u.narrow[2 * 16 + 0]), (kc->u.narrow[2 * 13 + 1]) = (kc->u.narrow[2 * 16 + 1]) );
                        ( (kc->u.narrow[2 * 16 + 0]) = tl, (kc->u.narrow[2 * 16 + 1]) = th);
                        (tl = (kc->u.narrow[2 * 21 + 0]), th = (kc->u.narrow[2 * 21 + 1]) );
                        ( (kc->u.narrow[2 * 21 + 0]) = (kc->u.narrow[2 * 7 + 0]), (kc->u.narrow[2 * 21 + 1]) = (kc->u.narrow[2 * 7 + 1]) );
                        ( (kc->u.narrow[2 * 7 + 0]) = (kc->u.narrow[2 * 2 + 0]), (kc->u.narrow[2 * 7 + 1]) = (kc->u.narrow[2 * 2 + 1]) );
                        ( (kc->u.narrow[2 * 2 + 0]) = tl, (kc->u.narrow[2 * 2 + 1]) = th);
                        (tl = (kc->u.narrow[2 * 17 + 0]), th = (kc->u.narrow[2 * 17 + 1]) );
                        ( (kc->u.narrow[2 * 17 + 0]) = (kc->u.narrow[2 * 14 + 0]), (kc->u.narrow[2 * 17 + 1]) = (kc->u.narrow[2 * 14 + 1]) );
                        ( (kc->u.narrow[2 * 14 + 0]) = (kc->u.narrow[2 * 4 + 0]), (kc->u.narrow[2 * 14 + 1]) = (kc->u.narrow[2 * 4 + 1]) );
                        ( (kc->u.narrow[2 * 4 + 0]) = tl, (kc->u.narrow[2 * 4 + 1]) = th);
                        (tl = (kc->u.narrow[2 * 3 + 0]), th = (kc->u.narrow[2 * 3 + 1]) );
                        ( (kc->u.narrow[2 * 3 + 0]) = (kc->u.narrow[2 * 9 + 0]), (kc->u.narrow[2 * 3 + 1]) = (kc->u.narrow[2 * 9 + 1]) );
                        ( (kc->u.narrow[2 * 9 + 0]) = (kc->u.narrow[2 * 23 + 0]), (kc->u.narrow[2 * 9 + 1]) = (kc->u.narrow[2 * 23 + 1]) );
                        ( (kc->u.narrow[2 * 23 + 0]) = tl, (kc->u.narrow[2 * 23 + 1]) = th);
                    }
                    while ( 0 );



                }
            }
            while ( 0 );
            ptr = 0;
        }
    }
    ;
    kc->ptr = ptr;
}



static void keccak_close32( sph_keccak_context *kc, unsigned ub, unsigned n, void *dst )
{
    unsigned eb;
    union
    {
        unsigned char tmp[136 + 1];
        sph_u64 dummy;
    } u;
    size_t j;
    eb = (0x100 | (ub & 0xFF) ) >> (8 - n);
    if ( kc->ptr == (136 - 1) )
    {
        if ( n == 7 )
        {
            u.tmp[0] = eb;
            memset( u.tmp + 1, 0, 136 - 1 );
            u.tmp[136] = 0x80;
            j = 1 + 136;
        }
        else
        {
            u.tmp[0] = eb | 0x80;
            j = 1;
        }
    }
    else
    {
        j = 136 - kc->ptr;
        u.tmp[0] = eb;
        memset( u.tmp + 1, 0, j - 2 );
        u.tmp[j - 1] = 0x80;
    }
    keccak_core( kc, u.tmp, j, 136 );
    kc->u.narrow[2] = ~kc->u.narrow[2];
    kc->u.narrow[3] = ~kc->u.narrow[3];
    kc->u.narrow[4] = ~kc->u.narrow[4];
    kc->u.narrow[5] = ~kc->u.narrow[5];
    kc->u.narrow[16] = ~kc->u.narrow[16];
    kc->u.narrow[17] = ~kc->u.narrow[17];
    kc->u.narrow[24] = ~kc->u.narrow[24];
    kc->u.narrow[25] = ~kc->u.narrow[25];
    kc->u.narrow[34] = ~kc->u.narrow[34];
    kc->u.narrow[35] = ~kc->u.narrow[35];
    kc->u.narrow[40] = ~kc->u.narrow[40];
    kc->u.narrow[41] = ~kc->u.narrow[41];
    for ( j = 0; j < 50; j += 2 )
        do
        {
            sph_u32 l, h, t;
            l = (kc->u.narrow[j]);
            h = (kc->u.narrow[j + 1]);
            t = (l ^ ( (h << 16) & ( (sph_u32) (0xFFFFFFFFU) ) ) ) & ( (sph_u32) (0xFFFF0000U) );
            l ^= t;
            h ^= t >> 16;
            t = (l ^ (l >> 8) ) & ( (sph_u32) (0x0000FF00U) );
            l ^= t ^ (t << 8);
            t = (h ^ (h >> 8) ) & ( (sph_u32) (0x0000FF00U) );
            h ^= t ^ (t << 8);
            t = (l ^ (l >> 4) ) & ( (sph_u32) (0x00F000F0U) );
            l ^= t ^ (t << 4);
            t = (h ^ (h >> 4) ) & ( (sph_u32) (0x00F000F0U) );
            h ^= t ^ (t << 4);
            t = (l ^ (l >> 2) ) & ( (sph_u32) (0x0C0C0C0CU) );
            l ^= t ^ (t << 2);
            t = (h ^ (h >> 2) ) & ( (sph_u32) (0x0C0C0C0CU) );
            h ^= t ^ (t << 2);
            t = (l ^ (l >> 1) ) & ( (sph_u32) (0x22222222U) );
            l ^= t ^ (t << 1);
            t = (h ^ (h >> 1) ) & ( (sph_u32) (0x22222222U) );
            h ^= t ^ (t << 1);
            (kc->u.narrow[j]) = l;
            (kc->u.narrow[j + 1]) = h;
        }
        while ( 0 );
    for ( j = 0; j < 32; j += 4 )
        sph_enc32le_aligned( u.tmp + j, kc->u.narrow[j >> 2] );
    memcpy( dst, u.tmp, 32 );
    keccak_init( kc, (unsigned) 32 << 3 );
}



static void keccak_close64( sph_keccak_context *kc, unsigned ub, unsigned n, void *dst )
{
    unsigned eb;
    union
    {
        unsigned char tmp[72 + 1];
        sph_u64 dummy;
    } u;
    size_t j;
    eb = (0x100 | (ub & 0xFF) ) >> (8 - n);
    if ( kc->ptr == (72 - 1) )
    {
        if ( n == 7 )
        {
            u.tmp[0] = eb;
            memset( u.tmp + 1, 0, 72 - 1 );
            u.tmp[72] = 0x80;
            j = 1 + 72;
        }
        else
        {
            u.tmp[0] = eb | 0x80;
            j = 1;
        }
    }
    else
    {
        j = 72 - kc->ptr;
        u.tmp[0] = eb;
        memset( u.tmp + 1, 0, j - 2 );
        u.tmp[j - 1] = 0x80;
    }
    keccak_core( kc, u.tmp, j, 72 );
    kc->u.narrow[2] = ~kc->u.narrow[2];
    kc->u.narrow[3] = ~kc->u.narrow[3];
    kc->u.narrow[4] = ~kc->u.narrow[4];
    kc->u.narrow[5] = ~kc->u.narrow[5];
    kc->u.narrow[16] = ~kc->u.narrow[16];
    kc->u.narrow[17] = ~kc->u.narrow[17];
    kc->u.narrow[24] = ~kc->u.narrow[24];
    kc->u.narrow[25] = ~kc->u.narrow[25];
    kc->u.narrow[34] = ~kc->u.narrow[34];
    kc->u.narrow[35] = ~kc->u.narrow[35];
    kc->u.narrow[40] = ~kc->u.narrow[40];
    kc->u.narrow[41] = ~kc->u.narrow[41];
    for ( j = 0; j < 50; j += 2 )
        do
        {
            sph_u32 l, h, t;
            l = (kc->u.narrow[j]);
            h = (kc->u.narrow[j + 1]);
            t = (l ^ ( (h << 16) & ( (sph_u32) (0xFFFFFFFFU) ) ) ) & ( (sph_u32) (0xFFFF0000U) );
            l ^= t;
            h ^= t >> 16;
            t = (l ^ (l >> 8) ) & ( (sph_u32) (0x0000FF00U) );
            l ^= t ^ (t << 8);
            t = (h ^ (h >> 8) ) & ( (sph_u32) (0x0000FF00U) );
            h ^= t ^ (t << 8);
            t = (l ^ (l >> 4) ) & ( (sph_u32) (0x00F000F0U) );
            l ^= t ^ (t << 4);
            t = (h ^ (h >> 4) ) & ( (sph_u32) (0x00F000F0U) );
            h ^= t ^ (t << 4);
            t = (l ^ (l >> 2) ) & ( (sph_u32) (0x0C0C0C0CU) );
            l ^= t ^ (t << 2);
            t = (h ^ (h >> 2) ) & ( (sph_u32) (0x0C0C0C0CU) );
            h ^= t ^ (t << 2);
            t = (l ^ (l >> 1) ) & ( (sph_u32) (0x22222222U) );
            l ^= t ^ (t << 1);
            t = (h ^ (h >> 1) ) & ( (sph_u32) (0x22222222U) );
            h ^= t ^ (t << 1);
            (kc->u.narrow[j]) = l;
            (kc->u.narrow[j + 1]) = h;
        }
        while ( 0 );
    for ( j = 0; j < 64; j += 4 )
        sph_enc32le_aligned( u.tmp + j, kc->u.narrow[j >> 2] );
    memcpy( dst, u.tmp, 64 );
    keccak_init( kc, (unsigned) 64 << 3 );
}


void
sph_keccak256_init( void *cc )
{
    keccak_init( cc, 256 );
}


void
sph_keccak256( void *cc, const void *data, size_t len )
{
    keccak_core( cc, data, len, 136 );
}


void
sph_keccak256_close( void *cc, void *dst )
{
    sph_keccak256_addbits_and_close( cc, 0, 0, dst );
}


void
sph_keccak256_addbits_and_close( void *cc, unsigned ub, unsigned n, void *dst )
{
    keccak_close32( cc, ub, n, dst );
}


void
sph_keccak512_init( void *cc )
{
    keccak_init( cc, 512 );
}


void
sph_keccak512( void *cc, const void *data, size_t len )
{
    keccak_core( cc, data, len, 72 );
}


void
sph_keccak512_close( void *cc, void *dst )
{
    sph_keccak512_addbits_and_close( cc, 0, 0, dst );
}


void
sph_keccak512_addbits_and_close( void *cc, unsigned ub, unsigned n, void *dst )
{
    keccak_close64( cc, ub, n, dst );
}


static int
compute_output( hashState *state, unsigned ub, unsigned n )
{
    if ( state->output_computed )
        return(0);
    switch ( state->hashbitlen )
    {
    case 256:
        sph_keccak256_addbits_and_close(
            &state->u.ctx256, ub, n, state->output );
        break;
    case 512:
        sph_keccak512_addbits_and_close(
            &state->u.ctx512, ub, n, state->output );
        break;
    default:
        return(-1);
    }
    state->output_computed = 1;
    return(0);
}


HashReturn
Init( hashState *state, int hashbitlen )
{
    switch ( hashbitlen )
    {
    case 256:
        sph_keccak256_init( &state->u.ctx256 );
        break;

    case 512:
        sph_keccak512_init( &state->u.ctx512 );
        break;
    default:
        return(BAD_HASHBITLEN);
    }
    state->hashbitlen	= hashbitlen;
    state->output_computed	= 0;
    return(SUCCESS);
}


HashReturn
Update( hashState *state, const BitSequence *data, DataLength databitlen )
{
    unsigned	extra;
    size_t		len;

    if ( state->output_computed )
        return(FAIL);
    extra	= (unsigned) databitlen & 0x7U;
    len	= (size_t) (databitlen >> 3);
    switch ( state->hashbitlen )
    {
    case 256:
        sph_keccak256(
            &state->u.ctx256, data, len );
        break;

    case 512:
        sph_keccak512(
            &state->u.ctx512, data, len );
        break;
    default:
        return(FAIL);
    }
    if ( extra > 0 )
        compute_output( state, data[len], extra );
    return(SUCCESS);
}


HashReturn
Final( hashState *state, BitSequence *hashval )
{
    size_t len;

    if ( compute_output( state, 0, 0 ) < 0 )
        return(FAIL);
    len = state->hashbitlen >> 3;
    if ( len > 64 )
        return(FAIL);
    memcpy( hashval, state->output, len );
    return(SUCCESS);
}


HashReturn
Hash( int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval )
{
    hashState	st;
    HashReturn	r;

    r = Init( &st, hashbitlen );
    if ( r != SUCCESS )
        return(r);
    r = Update( &st, data, databitlen );
    if ( r != SUCCESS )
        return(r);
    return(Final( &st, hashval ) );
}

int main()
{
    int	hashBitL=256;
    unsigned char text1[] = {""};
    unsigned char text2[] = {"abc"};
    unsigned char text3[] = {"huangyukai2014210916"};
    unsigned char text4[] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"};
    unsigned char text5[] = {"a"};

    unsigned char dist[0x40]= { 0 };
    hashState	st;

    int i;

    printf("\n================================================================\n");
    printf( "明文：%s\n" ,text1);
    Init( &st, hashBitL );
    Update( &st, text1, strlen((const char *)text1 )* 8 );
    Final( &st, dist );

    printf( "\n最终hash为: \n" );
    print_hex( dist, 32 );
    printf("\n================================================================\n");
    printf( "\n\n" );

    printf("\n================================================================\n");
    printf( "明文：%s\n" ,text2);
    Init( &st, hashBitL );
    Update( &st, text2, strlen((const char *)text2 )* 8 );
    Final( &st, dist );
    printf( "\n最终hash为: \n" );
    print_hex( dist, 32 );
    printf("\n================================================================\n");
    printf( "\n\n" );

    printf("\n================================================================\n");
    printf( "明文：%s\n" ,text3);
    Init( &st, hashBitL );
    Update( &st, text3, strlen((const char *)text3 )* 8 );
    Final( &st, dist );
    printf( "\n最终hash为: \n" );
    print_hex( dist, 32 );
    printf("\n================================================================\n");
    printf( "\n\n" );

    out=0;
    printf("\n================================================================\n");
    printf( "明文：%s\n" ,text4);
    Init( &st, hashBitL );
    Update( &st, text4, strlen((const char *)text4)* 8 );
    Final( &st, dist );
    printf( "\n最终hash为: \n" );
    print_hex( dist, 32 );
    printf("\n================================================================\n");
    printf( "\n\n" );

    printf("\n================================================================\n");
    printf( "明文：aaaa....a\n" );

    Init( &st, hashBitL );

    for (i = 0; i < 1000000; i++)
        Update( &st, text5, strlen((const char *)text5 )* 8 );
    Final( &st, dist );
    printf( "\n最终hash为: \n" );
    print_hex( dist, 32 );
    printf("\n================================================================\n");
    printf( "\n\n" );

    return(0);
}

