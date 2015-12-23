/*************************************************************************
	> File Name: test.c
	> Author: 
	> Mail: 
	> Created Time: 2015年12月16日 星期三 14时59分53秒
 ************************************************************************/

#include<stdio.h>
#include<libexif/exif-data.h>

int main(int argc, char *argv[])
{
    ExifData * pt=exif_data_new_from_file(argv[1]);

    if(argc<2)
    {
        printf("error in input file \n");
        return 0 ;
    }
    printf("display files\n");
    if(!pt)
        printf("empty files\n");
    exif_data_dump(pt);
    printf("done\n");
    return 0;
}
