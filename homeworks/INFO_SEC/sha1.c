#include <stdlib.h>
#include <memory.h>
#include <stddef.h>

#define ROTLEFT(a, b) ((a << b) | (a >> (32 - b)))
#define SHA1_BLOCK_SIZE 20
typedef unsigned char BYTE;
typedef unsigned int  WORD;

typedef struct
{
    BYTE data[64];
    WORD datalen;
    unsigned long long bitlen;
    WORD state[5];
    WORD k[4];
} SHA1_CTX;

int out=1;
void print_hex( BYTE str[], int len )
{
    int idx;
    for ( idx = 0; idx < len; idx++ )
        printf( "%02x", str[idx] );
}

void print_state( WORD a, WORD b,WORD c,WORD d,WORD e)
{
    int i;
    BYTE hash[SHA1_BLOCK_SIZE];
    for (i = 0; i < 4; ++i)
    {
        hash[i]      = (a >> (24 - i * 8)) & 0x000000ff;
        hash[i + 4]  = (b >> (24 - i * 8)) & 0x000000ff;
        hash[i + 8]  = (c >> (24 - i * 8)) & 0x000000ff;
        hash[i + 12] = (d >> (24 - i * 8)) & 0x000000ff;
        hash[i + 16] = (e >> (24 - i * 8)) & 0x000000ff;
    }

    print_hex(hash, 20);
}

void sha1_transform(SHA1_CTX *ctx, const BYTE data[])
{
    WORD a, b, c, d, e, i, j, t, m[80];

    for (i = 0, j = 0; i < 16; ++i, j += 4)
        m[i] = (data[j] << 24) + (data[j + 1] << 16) + (data[j + 2] << 8) + (data[j + 3]);
    for ( ; i < 80; ++i)
    {
        m[i] = (m[i - 3] ^ m[i - 8] ^ m[i - 14] ^ m[i - 16]);
        m[i] = (m[i] << 1) | (m[i] >> 31);
    }

    a = ctx->state[0];
    b = ctx->state[1];
    c = ctx->state[2];
    d = ctx->state[3];
    e = ctx->state[4];
    if(out)
    {
        printf( "\n分组起始状态：\n" );
        print_state(a,b,c,d,e);
    }

    for (i = 0; i < 20; ++i)
    {
        t = ROTLEFT(a, 5) + ((b & c) ^ (~b & d)) + e + ctx->k[0] + m[i];
        e = d;
        d = c;
        c = ROTLEFT(b, 30);
        b = a;
        a = t;
        if(out)
        {
            printf( "\n第%d轮中间状态：\n",i+1 );
            print_state(a,b,c,d,e);
        }

    }
    for ( ; i < 40; ++i)
    {
        t = ROTLEFT(a, 5) + (b ^ c ^ d) + e + ctx->k[1] + m[i];
        e = d;
        d = c;
        c = ROTLEFT(b, 30);
        b = a;
        a = t;
        if(out)
        {
            printf( "\n第%d轮中间状态：\n",i+1 );
            print_state(a,b,c,d,e);
        }
    }
    for ( ; i < 60; ++i)
    {
        t = ROTLEFT(a, 5) + ((b & c) ^ (b & d) ^ (c & d))  + e + ctx->k[2] + m[i];
        e = d;
        d = c;
        c = ROTLEFT(b, 30);
        b = a;
        a = t;
        if(out)
        {
            printf( "\n第%d轮中间状态：\n",i+1 );
            print_state(a,b,c,d,e);
        }
    }
    for ( ; i < 80; ++i)
    {
        t = ROTLEFT(a, 5) + (b ^ c ^ d) + e + ctx->k[3] + m[i];
        e = d;
        d = c;
        c = ROTLEFT(b, 30);
        b = a;
        a = t;
        if(out)
        {
            printf( "\n第%d轮中间状态：\n",i+1 );
            print_state(a,b,c,d,e);
        }
    }

    ctx->state[0] += a;
    ctx->state[1] += b;
    ctx->state[2] += c;
    ctx->state[3] += d;
    ctx->state[4] += e;

    if(out)
    {
        printf( "\n该分组输出hash：\n");
        print_state(ctx->state[0],ctx->state[1],ctx->state[2],ctx->state[3],ctx->state[4]);
        printf( "\n");
    }

}

void sha1_init(SHA1_CTX *ctx)
{
    ctx->datalen = 0;
    ctx->bitlen = 0;
    ctx->state[0] = 0x67452301;
    ctx->state[1] = 0xEFCDAB89;
    ctx->state[2] = 0x98BADCFE;
    ctx->state[3] = 0x10325476;
    ctx->state[4] = 0xc3d2e1f0;
    ctx->k[0] = 0x5a827999;
    ctx->k[1] = 0x6ed9eba1;
    ctx->k[2] = 0x8f1bbcdc;
    ctx->k[3] = 0xca62c1d6;
}

void sha1_update(SHA1_CTX *ctx, const BYTE data[], size_t len)
{
    size_t i;

    for (i = 0; i < len; ++i)
    {
        ctx->data[ctx->datalen] = data[i];
        ctx->datalen++;
        if (ctx->datalen == 64)
        {
            sha1_transform(ctx, ctx->data);
            ctx->bitlen += 512;
            ctx->datalen = 0;
        }
    }
}

void sha1_final(SHA1_CTX *ctx, BYTE hash[])
{
    WORD i;
    i = ctx->datalen;

    if (ctx->datalen < 56)
    {
        ctx->data[i++] = 0x80;
        while (i < 56)
            ctx->data[i++] = 0x00;
    }
    else
    {
        ctx->data[i++] = 0x80;
        while (i < 64)
            ctx->data[i++] = 0x00;
        sha1_transform(ctx, ctx->data);
        memset(ctx->data, 0, 56);
    }

    ctx->bitlen += ctx->datalen * 8;
    ctx->data[63] = ctx->bitlen;
    ctx->data[62] = ctx->bitlen >> 8;
    ctx->data[61] = ctx->bitlen >> 16;
    ctx->data[60] = ctx->bitlen >> 24;
    ctx->data[59] = ctx->bitlen >> 32;
    ctx->data[58] = ctx->bitlen >> 40;
    ctx->data[57] = ctx->bitlen >> 48;
    ctx->data[56] = ctx->bitlen >> 56;
    sha1_transform(ctx, ctx->data);

    for (i = 0; i < 4; i++)
    {
        hash[i]      = (ctx->state[0] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 4]  = (ctx->state[1] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 8]  = (ctx->state[2] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 12] = (ctx->state[3] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 16] = (ctx->state[4] >> (24 - i * 8)) & 0x000000ff;
    }
}


int main()
{
    BYTE text1[] = {""};
    BYTE text2[] = {"abc"};
    BYTE text3[] = {"huangyukai2014210916"};
    BYTE text4[] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"};
    BYTE text5[] = {"a"};

    BYTE buf[SHA1_BLOCK_SIZE];
    int i;
    SHA1_CTX ctx;

    printf("\n========================================\n");
    printf( "明文：%s\n" ,text1);
    sha1_init(&ctx);
    sha1_update(&ctx, text1, strlen(text1));
    sha1_final(&ctx, buf);
    printf( "\n最终hash为: \n" );
    print_hex( buf, 20 );
    printf("\n========================================\n");
    printf( "\n\n" );

    printf("\n========================================\n");
    printf( "明文：%s\n" ,text2);
    sha1_init(&ctx);
    sha1_update(&ctx, text2, strlen(text2));
    sha1_final(&ctx, buf);
    printf( "\n最终hash为: \n" );
    print_hex( buf, 20 );
    printf("\n========================================\n");
    printf( "\n\n" );

    printf("\n========================================\n");
    printf( "明文：%s\n" ,text3);
    sha1_init(&ctx);
    sha1_update(&ctx, text3, strlen(text3));
    sha1_final(&ctx, buf);
    printf( "\n最终hash为: \n" );
    print_hex( buf, 20 );
    printf("\n========================================\n");
    printf( "\n\n" );

    out=0;
    printf("\n========================================\n");
    printf( "明文：%s\n" ,text4);
    sha1_init(&ctx);
    sha1_update(&ctx, text4, strlen(text4));
    sha1_final(&ctx, buf);
    printf( "\n最终hash为: \n" );
    print_hex( buf, 20 );
    printf("\n========================================\n");
    printf( "\n\n" );

    printf("\n========================================\n");
    printf( "明文：aaaa....a\n" );
    sha1_init(&ctx);
    for (i = 0; i < 1000000; i++)
        sha1_update(&ctx, text5, strlen(text5));
    sha1_final(&ctx, buf);
    printf( "\n最终hash为: \n" );
    print_hex( buf, 20 );
    printf("\n========================================\n");
    printf( "\n\n" );

    return(0);
}
