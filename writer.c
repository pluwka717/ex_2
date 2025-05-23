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
    fd = open (FIFO_NAME,O_WRONLY);
    scanf("%s",buffer);
    ret = write (fd,buffer,strlen(buffer)+1);
    close (fd);
    ret=unlink(FIFO_NAME);
    printf("program complete\n");
    return 0;
}
