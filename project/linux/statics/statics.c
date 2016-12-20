#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
typedef struct Word
{
    char w[200];
    int k;
    struct Word *next;
} pWord;

int main(int argc, char *argv[])
{
    FILE *fp = fopen(argv[1],"rb");
    struct Word *Head = NULL;

    if(argc<2)
    {
        printf("error in input format\n");
        return 0;
    }

    while (!feof(fp))
    {
        char *p = (char *)malloc(200*sizeof(char));
        memset(p,0,200);
        fscanf(fp, "%s", p);
        if(Head == NULL)
        {
            struct Word *temp = (struct Word *)malloc(sizeof(struct Word));
            strcpy(temp->w, p);
            temp->k = 1;
            temp->next = NULL;
            Head = temp;
        }
        else
        {
            struct Word *pp = Head;
            while (pp != NULL)
            {
                if (strcasecmp( pp->w, p) == 0)
                {
                    ++pp->k;
                    break;
                }
                else
                {
                    pp = pp->next;
                }
            }
            if (pp == NULL)
            {
                struct Word *temp = (struct Word *)malloc(sizeof(struct Word));
                strcpy(temp->w, p);
                temp->k = 1;
                temp->next = Head;
                Head = temp;
            }
        }
    }
    struct Word *q = Head;
    while (q != NULL)
    {
        printf("%s ", q->w);
        printf("%d\n", q->k);
        q = q->next;
    }
    return 0;
}
