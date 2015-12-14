#include <stdio.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <stdlib.h>  
#include <memory.h>  
#include <string.h>  
  
#define BUFFER_SIZE                1024    
#define FILE_NAME_MAX_SIZE         512    
  
int main(int argc,char *argv[])  
{  
    int socketfd;  
    struct sockaddr_in s_add,c_add;  
    unsigned short portnum = 0x22B8;  
    int len;  
    char buffer[BUFFER_SIZE];    
    FILE *fp ;  
    int file_block_length = 0;    
      
      
      
    /* 
     *Create the socket 
     */  
    if((socketfd=socket(AF_INET,SOCK_STREAM,0))<0)  
    {  
        printf("Socket create error! \n");  
        exit(1);  
    }  
      
    /* 
     *set the address format 
     */   
    bzero(&s_add,sizeof(struct sockaddr_in));  
    s_add.sin_family = AF_INET;  
    /* 
     *change the VM address 
     */  
    s_add.sin_addr.s_addr = htons(INADDR_ANY);//change the string to 32-bit internet byte.  
    s_add.sin_port=htons(portnum);  
      
    if (connect(socketfd,(struct sockaddr *)(&s_add),sizeof(struct sockaddr))<0)  
    {  
        printf("Connect failure!\n");  
        return -1;  
    }  
    else  
        printf("Connect Success!\n");  
      
    /* 
     *Send the file 
     */       
    fp = fopen("send.txt", "r");    
    if (fp == NULL)    
    {    
        printf("File:  Not Found!\n");    
    }    
    else    
    {    
        bzero(buffer, BUFFER_SIZE);           
        while( (file_block_length = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) > 0)    
        {    
            printf("file_block_length = %d\n", file_block_length);    
  
            /* 
             *Send the string to the buffer   
             */  
            if (send(socketfd, buffer, file_block_length, 0) < 0)    
            {    
                printf("Send File:test Failed!\n");    
                break;    
            }    
  
            bzero(buffer, sizeof(buffer));    
        }    
        fclose(fp);    
        printf("File:test Transfer Finished!\n");    
    }    
    close(socketfd);    
      
    return 0;  
}  