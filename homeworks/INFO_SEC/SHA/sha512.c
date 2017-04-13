#include <stdio.h>
#include <string.h>
#if defined(DJGPP) || defined(__CYGWIN__)
typedef unsigned long long UINT64;
#else
#include <basetsd.h>
#endif

typedef struct
{
    UINT64 state [8], length, curlen;
    unsigned char buf [128];
} SHA512_CTX;

static const UINT64 K [80] =
{
    0x428A2F98D728AE22, 0x7137449123EF65CD, 0xB5C0FBCFEC4D3B2F, 0xE9B5DBA58189DBBC,
    0x3956C25BF348B538, 0x59F111F1B605D019, 0x923F82A4AF194F9B, 0xAB1C5ED5DA6D8118,
    0xD807AA98A3030242, 0x12835B0145706FBE, 0x243185BE4EE4B28C, 0x550C7DC3D5FFB4E2,
    0x72BE5D74F27B896F, 0x80DEB1FE3B1696B1, 0x9BDC06A725C71235, 0xC19BF174CF692694,
    0xE49B69C19EF14AD2, 0xEFBE4786384F25E3, 0x0FC19DC68B8CD5B5, 0x240CA1CC77AC9C65,
    0x2DE92C6F592B0275, 0x4A7484AA6EA6E483, 0x5CB0A9DCBD41FBD4, 0x76F988DA831153B5,
    0x983E5152EE66DFAB, 0xA831C66D2DB43210, 0xB00327C898FB213F, 0xBF597FC7BEEF0EE4,
    0xC6E00BF33DA88FC2, 0xD5A79147930AA725, 0x06CA6351E003826F, 0x142929670A0E6E70,
    0x27B70A8546D22FFC, 0x2E1B21385C26C926, 0x4D2C6DFC5AC42AED, 0x53380D139D95B3DF,
    0x650A73548BAF63DE, 0x766A0ABB3C77B2A8, 0x81C2C92E47EDAEE6, 0x92722C851482353B,
    0xA2BFE8A14CF10364, 0xA81A664BBC423001, 0xC24B8B70D0F89791, 0xC76C51A30654BE30,
    0xD192E819D6EF5218, 0xD69906245565A910, 0xF40E35855771202A, 0x106AA07032BBD1B8,
    0x19A4C116B8D2D0C8, 0x1E376C085141AB53, 0x2748774CDF8EEB99, 0x34B0BCB5E19B48A8,
    0x391C0CB3C5C95A63, 0x4ED8AA4AE3418ACB, 0x5B9CCA4F7763E373, 0x682E6FF3D6B2B8A3,
    0x748F82EE5DEFB2FC, 0x78A5636F43172F60, 0x84C87814A1F0AB72, 0x8CC702081A6439EC,
    0x90BEFFFA23631E28, 0xA4506CEBDE82BDE9, 0xBEF9A3F7B2C67915, 0xC67178F2E372532B,
    0xCA273ECEEA26619C, 0xD186B8C721C0C207, 0xEADA7DD6CDE0EB1E, 0xF57D4F7FEE6ED178,
    0x06F067AA72176FBA, 0x0A637DC5A2C898A6, 0x113F9804BEF90DAE, 0x1B710B35131C471B,
    0x28DB77F523047D84, 0x32CAAB7B40C72493, 0x3C9EBE0A15C9BEBC, 0x431D67C49C100D4C,
    0x4CC5D4BECB3E42B6, 0x597F299CFC657E2A, 0x5FCB6FAB3AD6FAEC, 0x6C44198C4A475817,
};
typedef unsigned char BYTE;
typedef unsigned int  WORD;

int out=1;
void print_hex( BYTE str[], int len )
{
    int idx;
    for ( idx = 0; idx < len; idx++ )
        printf( "%02x", str[idx] );
}

void print_state( UINT64 S [8])
{
    int i;
    BYTE hash[64];

    for (i = 0; i < 64; i++)
        hash[i] = S [i>>3] >> (((7-i)&7)<<3);


    print_hex(hash, 64);
}

#define USE_DEFINE
#ifdef USE_DEFINE
#define Ch(x,y,z)	((x & y) ^ (~x & z))
#define Maj(x,y,z)	((x & y) ^ (x & z) ^ (y & z))
#define S(x, n)		(((x)>>(n))|((x)<<(64-n)))
#define R(x, n)		((x)>>(n))
#define Sigma0(x)	(S(x, 28) ^ S(x, 34) ^ S(x, 39))
#define Sigma1(x)	(S(x, 14) ^ S(x, 18) ^ S(x, 41))
#define Gamma0(x)	(S(x, 1 ) ^ S(x, 8 ) ^ R(x, 7 ))
#define Gamma1(x)	(S(x, 19) ^ S(x, 61) ^ R(x, 6 ))
#endif

#ifdef USE_INLINE
USE_INLINE UINT64 Ch (UINT64 x, UINT64 y, UINT64 z)
{
    return (x & y) ^ (~x & z);
}
USE_INLINE UINT64 Maj (UINT64 x, UINT64 y, UINT64 z)
{
    return (x & y) ^ (x & z) ^ (y & z);
}

USE_INLINE UINT64 rotateRight (UINT64 x, UINT64 n)
{
#ifdef __LCC__
    return (x >> n) | (x << (n=64 - n));
#else
    return (x >> n) | (x << (64 - n));
#endif
}

USE_INLINE UINT64 shiftRight (UINT64 x, UINT64 n)
{
    return x >> n;
}

USE_INLINE UINT64 Sigma0 (UINT64 x)
{
    return (rotateRight (x, 28) ^ rotateRight (x, 34) ^ rotateRight (x, 39));
}

USE_INLINE UINT64 Sigma1 (UINT64 x)
{
    return rotateRight (x, 14) ^ rotateRight (x, 18) ^ rotateRight (x, 41);
}

USE_INLINE UINT64 Gamma0 (UINT64 x)
{
    return rotateRight (x, 1) ^ rotateRight (x, 8) ^ shiftRight (x, 7);
}

USE_INLINE UINT64 Gamma1 (UINT64 x)
{
    return rotateRight (x, 19) ^ rotateRight (x, 61) ^ shiftRight (x, 6);
}
#endif

static void sha512_transform(SHA512_CTX *ctx)
{
    UINT64 S [8], W [80], t0, t1;
    int i;

    memcpy (S, ctx->state, 64);
    if(out)
    {
        printf( "\n分组起始状态：\n" );
        print_state(S);
    }

    for (i = 0; i < 128; i+=8)
        W [i>>3] =
            ((UINT64)ctx->buf [i + 0]) << 56 |
            ((UINT64)ctx->buf [i + 1]) << 48 |
            ((UINT64)ctx->buf [i + 2]) << 40 |
            ((UINT64)ctx->buf [i + 3]) << 32 |
            ((UINT64)ctx->buf [i + 4]) << 24 |
            ((UINT64)ctx->buf [i + 5]) << 16 |
            ((UINT64)ctx->buf [i + 6]) <<  8 |
            ((UINT64)ctx->buf [i + 7]);

    for (i = 16; i < 80; i++)
    {
        W [i] = Gamma1 (W [i - 2]) + W [i - 7];
        W [i] += Gamma0 (W [i - 15]) + W [i - 16];
    }

    for (i = 0; i < 80; i++)
    {
        t0 = S[7] + Sigma1 (S [4]) + Ch (S [4], S [5], S [6]) + K [i] + W [i];
        t1 = Sigma0 (S [0]) + Maj (S [0], S [1], S [2]);
        S [7] = S [6];
        S [6] = S [5];
        S [5] = S [4];
        S [4] = S [3] + t0;
        S [3] = S [2];
        S [2] = S [1];
        S [1] = S [0];
        S [0] = t0 + t1;

        if(out)
        {
            printf( "\n第%d轮中间状态：\n",i+1 );
            print_state(S);
        }
    }


    for (i = 0; i < 8; i++)
        ctx->state[i] += S[i];
    if(out)
    {
        printf( "\n该分组输出hash：\n");
        print_state(ctx->state);
        printf( "\n");
    }

}

void sha512_init(SHA512_CTX *ctx)
{
    ctx->curlen = ctx->length = 0;
    ctx->state [0] = 0x6A09E667F3BCC908;
    ctx->state [1] = 0xBB67AE8584CAA73B;
    ctx->state [2] = 0x3C6EF372FE94F82B;
    ctx->state [3] = 0xA54FF53A5F1D36F1;
    ctx->state [4] = 0x510E527FADE682D1;
    ctx->state [5] = 0x9B05688C2B3E6C1F;
    ctx->state [6] = 0x1F83D9ABFB41BD6B;
    ctx->state [7] = 0x5BE0CD19137E2179;
}

void sha512_update(SHA512_CTX *ctx, unsigned char *buf, int len)
{
    while (len--)
    {
        ctx->buf[ctx->curlen++] = *buf++;

        if (ctx->curlen == 128)
        {
            sha512_transform(ctx);
            ctx->length += 1024;
            ctx->curlen = 0;
        }
    }
}

void sha512_final(SHA512_CTX *ctx, unsigned char *hash)
{
    int i;
    ctx->length += ctx->curlen << 3;
    ctx->buf[ctx->curlen++] = 0x80;


    if (ctx->curlen >= 112)
    {
        for (; ctx->curlen < 128; )
            ctx->buf[ctx->curlen++] = 0;
        sha512_transform(ctx);
        ctx->curlen = 0;
    }


    for (; ctx->curlen < 112; )
        ctx->buf[ctx->curlen++] = 0;
    for (i = 112; i < 120; i++)
        ctx->buf[i] = 0;
    for (i = 120; i < 128; i++)
        ctx->buf [i] = (ctx->length >> ((127-i)<<3)) & 255;
    sha512_transform(ctx);
    for (i = 0; i < 64; i++)
        hash[i] = ctx->state [i>>3] >> (((7-i)&7)<<3);
}


int sha_file(unsigned char *filename, unsigned char *hash)
{
    unsigned char buf[1024];
    int i;
    FILE *in;
    SHA512_CTX ctx;

    sha512_init(&ctx);
    in = fopen(filename, "rb");
    if (!in) return 0;
    do
    {
        i = fread(buf, 1, 1024, in);
        sha512_update(&ctx, buf, i);
    }
    while (i == 1024);
    sha512_final(&ctx, hash);
    fclose(in);
    return 1;
}

int main()
{
    BYTE text1[] = {""};
    BYTE text2[] = {"abc"};
    BYTE text3[] = {"huangyukai2014210916"};
    BYTE text4[] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"};
    BYTE text5[] = {"a"};

    BYTE buf[64];
    SHA512_CTX ctx;
    int i;

    printf("\n================================================================\n");
    printf( "明文：%s\n" ,text1);
    sha512_init(&ctx);
    sha512_update(&ctx, text1, strlen(text1));
    sha512_final(&ctx, buf);
    printf( "\n最终hash为: \n" );
    print_hex( buf, 64 );
    printf("\n================================================================\n");
    printf( "\n\n" );

    printf("\n================================================================\n");
    printf( "明文：%s\n" ,text2);
    sha512_init(&ctx);
    sha512_update(&ctx, text2, strlen(text2));
    sha512_final(&ctx, buf);
    printf( "\n最终hash为: \n" );
    print_hex( buf, 64 );
    printf("\n================================================================\n");
    printf( "\n\n" );

    printf("\n================================================================\n");
    printf( "明文：%s\n" ,text3);
    sha512_init(&ctx);
    sha512_update(&ctx, text3, strlen(text3));
    sha512_final(&ctx, buf);
    printf( "\n最终hash为: \n" );
    print_hex( buf, 64 );
    printf("\n================================================================\n");
    printf( "\n\n" );

    out=0;
    printf("\n================================================================\n");
    printf( "明文：%s\n" ,text4);
    sha512_init(&ctx);
    sha512_update(&ctx, text4, strlen(text4));
    sha512_final(&ctx, buf);
    printf( "\n最终hash为: \n" );
    print_hex( buf, 64 );
    printf("\n================================================================\n");
    printf( "\n\n" );

    printf("\n================================================================\n");
    printf( "明文：aaaa....a\n" );
    sha512_init(&ctx);
    for (i = 0; i < 1000000; i++)
        sha512_update(&ctx, text5, strlen(text5));
    sha512_final(&ctx, buf);
    printf( "\n最终hash为: \n" );
    print_hex( buf, 64 );
    printf("\n================================================================\n");
    printf( "\n\n" );

    return(0);
}
