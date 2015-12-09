#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

int len = 0;
int trave_dir(char* path)
{
    DIR *d;
    struct dirent *file;
    struct stat buf;

    if(!(d = opendir(path)))
    {
        printf("error opendir %s!!!\\n",path);
        return -1;
    }
    //Add this, so that it can scan the children dir(please look at main() function)
    chdir(path);
    while((file = readdir(d)) != NULL)
    {

        if(strncmp(file->d_name, ".", 1) == 0)
            continue;
        if(stat(file->d_name, &buf) >= 0 && !S_ISDIR(buf.st_mode) )
        {
            struct tm *timeinfo;
            {
                time_t nowtime;
                time( &nowtime );
                timeinfo = localtime( &nowtime );
            }
            time_t t = buf.st_mtime;
            struct tm lt;
            char timbuf[80];
            localtime_r(&t, &lt);
            if( timeinfo->tm_year!=lt.tm_year
                    ||timeinfo->tm_mon!=lt.tm_mon
                    ||timeinfo->tm_mday!=lt.tm_mday)
            {
                continue;
            }
            strftime(timbuf, sizeof(timbuf), "%c", &lt);

            printf("==============================\n");
            printf("file name:\t%s\n",file->d_name);
            printf("file size:\t%d\n",(int)(buf.st_size));
            printf("last modified time:\t%s\n", timbuf);

        }

    }
    closedir(d);
    return 0;
}
int main() 
{
    int i;
    printf("this file is about to display any file modified today\n");
    printf("this demo is copyright@LIU YING\n");
    trave_dir(".");//trave_dir("./ChildrenDir");
    return 0;
}
