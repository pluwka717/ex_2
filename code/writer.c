#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#define FIFO_NAME "tmp-fifo"


int main()
{
    int fd;
    int ret;
    char buffer[256];

    printf("program start\n");
    ret = mkfifo(FIFO_NAME,0666);
    if (ret==-1){
        perror("mkfifo");
        EXIT_FAILURE;
    } else printf ("FIFO created!\n");

    fd = open (FIFO_NAME,O_WRONLY);
    if (fd==-1){
        perror("open - write");
        EXIT_FAILURE;
    } else printf("FIFO opend for writting!\n");

    printf("Write message:");
    scanf("%s",buffer);
    ret = write (fd,buffer,strlen(buffer)+1);
    if (ret==-1){
        perror("writting");
        EXIT_FAILURE;
    } else printf("FIFO is recorded");
    
    close (fd);
    ret=unlink(FIFO_NAME);
    if (ret==-1){
        perror("unlink");
        EXIT_FAILURE;
    } else printf("program complete\n");
 
    return 0;
}
