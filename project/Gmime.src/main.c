#include "all.h"
#include <stdio.h>

int main(int argc, char const *argv[])
{
    /* code */
    GMimeMessage* pMessage=AllInits(argc,(char**)argv);
    if(!pMessage)
    {
        printf("error in ints \n");
        return 0;
    }

    /*analyze header message*/
    //GetHeader(pMessage);
    GetBasicInfo(pMessage);

    count_parts_in_message(pMessage);
    //GetBody( pMessage);
    //Analyze(pMessage);

    printf("[log]:\tunref the pMessage object.\n");
    g_object_unref(GMIME_OBJECT(pMessage));

    printf("[log]:\tshutdown for gmime\n");
    printf("local charset %s\n", g_mime_charset_locale_name());
    g_mime_shutdown();

    return 0;
}
