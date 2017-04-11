#include <sys/types.h>
#include <stdint.h>
#include <string.h>

#define cpu_to_be32(v) (((v)>>24) | (((v)>>8)&0xff00) | (((v)<<8)&0xff0000) | ((v)<<24))
#define ROTATELEFT(X,n)  (((X)<<(n)) | ((X)>>(32-(n))))
#define P0(x) ((x) ^  ROTATELEFT((x),9)  ^ ROTATELEFT((x),17))
#define P1(x) ((x) ^  ROTATELEFT((x),15) ^ ROTATELEFT((x),23))
#define FF0(x,y,z) ( (x) ^ (y) ^ (z))
#define FF1(x,y,z) (((x) & (y)) | ( (x) & (z)) | ( (y) & (z)))
#define GG0(x,y,z) ( (x) ^ (y) ^ (z))
#define GG1(x,y,z) (((x) & (y)) | ( (~(x)) & (z)) )
#define SM3_DIGEST_LENGTH	32
#define SM3_BLOCK_SIZE		64
int out=1;

typedef unsigned char BYTE;
typedef unsigned int  WORD;

typedef struct
{
    uint32_t digest[8];
    int nblocks;
    unsigned char block[64];
    int num;
} SM3_CTX;

void print_hex( BYTE str[], int len )
{
    int idx;
    for ( idx = 0; idx < len; idx++ )
        printf( "%02x", str[idx] );
}

void print_state( WORD a, WORD b,WORD c,WORD d,WORD e,WORD f,WORD g,WORD h)
{
    int i;
    BYTE hash[SM3_DIGEST_LENGTH];

    for (i = 0; i < 4; i++)
    {
        hash[i]      = (a >> (24 - i * 8)) & 0x000000ff;
        hash[i + 4]  = (b >> (24 - i * 8)) & 0x000000ff;
        hash[i + 8]  = (c >> (24 - i * 8)) & 0x000000ff;
        hash[i + 12] = (d >> (24 - i * 8)) & 0x000000ff;
        hash[i + 16] = (e >> (24 - i * 8)) & 0x000000ff;
        hash[i + 20] = (f >> (24 - i * 8)) & 0x000000ff;
        hash[i + 24] = (g >> (24 - i * 8)) & 0x000000ff;
        hash[i + 28] = (h >> (24 - i * 8)) & 0x000000ff;
    }

    print_hex(hash, SM3_DIGEST_LENGTH);
}
void sm3_compress(uint32_t digest[8], const unsigned char block[64])
{
    int j;
    uint32_t W[68], W1[64];
    const uint32_t *pblock = (const uint32_t *)block;

    uint32_t A = digest[0];
    uint32_t B = digest[1];
    uint32_t C = digest[2];
    uint32_t D = digest[3];
    uint32_t E = digest[4];
    uint32_t F = digest[5];
    uint32_t G = digest[6];
    uint32_t H = digest[7];
    uint32_t SS1,SS2,TT1,TT2,T[64];
    if(out)
    {
        printf( "\n分组起始状态：\n" );
        print_state(A,B,C,D,E,F,G,H);
    }
    for (j = 0; j < 16; j++)
    {
        W[j] = cpu_to_be32(pblock[j]);
    }
    for (j = 16; j < 68; j++)
    {
        W[j] = P1( W[j-16] ^ W[j-9] ^ ROTATELEFT(W[j-3],15)) ^ ROTATELEFT(W[j - 13],7 ) ^ W[j-6];;
    }
    for( j = 0; j < 64; j++)
    {
        W1[j] = W[j] ^ W[j+4];
    }

    for(j =0; j < 16; j++)
    {

        T[j] = 0x79CC4519;
        SS1 = ROTATELEFT((ROTATELEFT(A,12) + E + ROTATELEFT(T[j],j)), 7);
        SS2 = SS1 ^ ROTATELEFT(A,12);
        TT1 = FF0(A,B,C) + D + SS2 + W1[j];
        TT2 = GG0(E,F,G) + H + SS1 + W[j];
        D = C;
        C = ROTATELEFT(B,9);
        B = A;
        A = TT1;
        H = G;
        G = ROTATELEFT(F,19);
        F = E;
        E = P0(TT2);

        if(out)
        {
            printf( "\n第%d轮中间状态：\n",j+1 );
            print_state(A,B,C,D,E,F,G,H);
        }
    }

    for(j =16; j < 64; j++)
    {

        T[j] = 0x7A879D8A;
        SS1 = ROTATELEFT((ROTATELEFT(A,12) + E + ROTATELEFT(T[j],j)), 7);
        SS2 = SS1 ^ ROTATELEFT(A,12);
        TT1 = FF1(A,B,C) + D + SS2 + W1[j];
        TT2 = GG1(E,F,G) + H + SS1 + W[j];
        D = C;
        C = ROTATELEFT(B,9);
        B = A;
        A = TT1;
        H = G;
        G = ROTATELEFT(F,19);
        F = E;
        E = P0(TT2);
        if(out)
        {
            printf( "\n第%d轮中间状态：\n",j+1 );
            print_state(A,B,C,D,E,F,G,H);
        }
    }

    digest[0] ^= A;
    digest[1] ^= B;
    digest[2] ^= C;
    digest[3] ^= D;
    digest[4] ^= E;
    digest[5] ^= F;
    digest[6] ^= G;
    digest[7] ^= H;

    if(out)
    {
        printf( "\n该分组输出hash：\n");
        print_state(digest[0],digest[1],digest[2],digest[3],digest[4],digest[5],digest[6],digest[7]);
        printf( "\n");
    }
}
void sm3_init(SM3_CTX *ctx)
{
    ctx->digest[0] = 0x7380166F;
    ctx->digest[1] = 0x4914B2B9;
    ctx->digest[2] = 0x172442D7;
    ctx->digest[3] = 0xDA8A0600;
    ctx->digest[4] = 0xA96F30BC;
    ctx->digest[5] = 0x163138AA;
    ctx->digest[6] = 0xE38DEE4D;
    ctx->digest[7] = 0xB0FB0E4E;

    ctx->nblocks = 0;
    ctx->num = 0;
}

void sm3_update(SM3_CTX *ctx, const unsigned char* data, size_t data_len)
{
    if (ctx->num)
    {
        unsigned int left = SM3_BLOCK_SIZE - ctx->num;
        if (data_len < left)
        {
            memcpy(ctx->block + ctx->num, data, data_len);
            ctx->num += data_len;
            return;
        }
        else
        {
            memcpy(ctx->block + ctx->num, data, left);
            sm3_compress(ctx->digest, ctx->block);
            ctx->nblocks++;
            data += left;
            data_len -= left;
        }
    }
    while (data_len >= SM3_BLOCK_SIZE)
    {
        sm3_compress(ctx->digest, data);
        ctx->nblocks++;
        data += SM3_BLOCK_SIZE;
        data_len -= SM3_BLOCK_SIZE;
    }
    ctx->num = data_len;
    if (data_len)
    {
        memcpy(ctx->block, data, data_len);
    }
}

void sm3_final(SM3_CTX *ctx, unsigned char *digest)
{
    int i;
    uint32_t *pdigest = (uint32_t *)digest;
    uint32_t *count = (uint32_t *)(ctx->block + SM3_BLOCK_SIZE - 8);

    ctx->block[ctx->num] = 0x80;

    if (ctx->num + 9 <= SM3_BLOCK_SIZE)
    {
        memset(ctx->block + ctx->num + 1, 0, SM3_BLOCK_SIZE - ctx->num - 9);
    }
    else
    {
        memset(ctx->block + ctx->num + 1, 0, SM3_BLOCK_SIZE - ctx->num - 1);
        sm3_compress(ctx->digest, ctx->block);
        memset(ctx->block, 0, SM3_BLOCK_SIZE - 8);
    }

    count[0] = cpu_to_be32((ctx->nblocks) >> 23);
    count[1] = cpu_to_be32((ctx->nblocks << 9) + (ctx->num << 3));

    sm3_compress(ctx->digest, ctx->block);
    for (i = 0; i < sizeof(ctx->digest)/sizeof(ctx->digest[0]); i++)
    {
        pdigest[i] = cpu_to_be32(ctx->digest[i]);
    }
}

void sm3(const unsigned char *msg, size_t  msglen, unsigned char dgst[SM3_DIGEST_LENGTH])
{
    SM3_CTX ctx;

    sm3_init(&ctx);
    sm3_update(&ctx, msg, msglen);
    sm3_final(&ctx, dgst);

    memset(&ctx, 0, sizeof(SM3_CTX));
}
int main()
{
    BYTE text1[] = {""};
    BYTE text2[] = {"abc"};
    BYTE text3[] = {"huangyukai2014210916"};
    BYTE text4[] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"};
    BYTE text5[] = {"a"};

    BYTE buf[SM3_DIGEST_LENGTH];
    SM3_CTX ctx;
    int i;

    printf("\n================================================================\n");
    printf( "明文：%s\n" ,text1);
    sm3_init(&ctx);
    sm3_update(&ctx, text1, strlen(text1));
    sm3_final(&ctx, buf);
    printf( "\n最终hash为: \n" );
    print_hex( buf, SM3_DIGEST_LENGTH );
    printf("\n================================================================\n");
    printf( "\n\n" );

    printf("\n================================================================\n");
    printf( "明文：%s\n" ,text2);
    sm3_init(&ctx);
    sm3_update(&ctx, text2, strlen(text2));
    sm3_final(&ctx, buf);
    printf( "\n最终hash为: \n" );
    print_hex( buf, SM3_DIGEST_LENGTH );
    printf("\n================================================================\n");
    printf( "\n\n" );

    printf("\n================================================================\n");
    printf( "明文：%s\n" ,text3);
    sm3_init(&ctx);
    sm3_update(&ctx, text3, strlen(text3));
    sm3_final(&ctx, buf);
    printf( "\n最终hash为: \n" );
    print_hex( buf, SM3_DIGEST_LENGTH );
    printf("\n================================================================\n");
    printf( "\n\n" );

    out=0;
    printf("\n================================================================\n");
    printf( "明文：%s\n" ,text4);
    sm3_init(&ctx);
    sm3_update(&ctx, text4, strlen(text4));
    sm3_final(&ctx, buf);
    printf( "\n最终hash为: \n" );
    print_hex( buf, SM3_DIGEST_LENGTH );
    printf("\n================================================================\n");
    printf( "\n\n" );

    printf("\n================================================================\n");
    printf( "明文：aaaa....a\n" );
    sm3_init(&ctx);
    for (i = 0; i < 1000000; i++)
        sm3_update(&ctx, text5, strlen(text5));
    sm3_final(&ctx, buf);
    printf( "\n最终hash为: \n" );
    print_hex( buf, SM3_DIGEST_LENGTH );
    printf("\n================================================================\n");
    printf( "\n\n" );

    return(0);
}
