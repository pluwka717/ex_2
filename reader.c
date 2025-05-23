#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#define FIFO_NAME "tmp-fifo"
#define BUF_SIZE 256

int main(){
    int fd;
    char buffer [BUF_SIZE];
    int bytes_read;

    fd = open (FIFO_NAME,O_RDONLY);
    printf("open descriptor for reading\n");
    bytes_read = read (fd, buffer,BUF_SIZE -1);
    if (bytes_read == -1){
        perror("read");
        close (fd);
        exit (EXIT_FAILURE);
    } else if (bytes_read==0){
        printf("EOF (FIFO closed).\n");
    } else {
        buffer [bytes_read] = '\0';
        printf("%s\n",buffer);
    }

    close (fd);
    printf("end\n");
    return 0;
}