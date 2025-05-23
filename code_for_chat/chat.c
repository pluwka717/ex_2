#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/select.h>

#define BUF_SIZE 256

void startchat(int socket_fd, const char *a)
{
    int max_fd;
    char buffer[BUF_SIZE];
    fd_set readfds;
    ssize_t bytes_rec;
    printf("chat is started:\n");
    while (1){
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO,&readfds);
        FD_SET(socket_fd,&readfds);

        if (STDIN_FILENO>socket_fd) {
            max_fd=STDIN_FILENO;
        } else max_fd=socket_fd;

        if (select(max_fd+1,&readfds,NULL,NULL,NULL)==-1){
            perror("select");
            break;
        }

        if (FD_ISSET(STDIN_FILENO,&readfds)){
            memset(buffer,0,BUF_SIZE);
            if(fgets(buffer,BUF_SIZE,stdin)==NULL){
                printf("[%s] EOF or error",a);
                strcpy(buffer,"exit\n");
                send(socket_fd,buffer,strlen(buffer),0);
                break;
            }
            if(send(socket_fd,buffer,strlen(buffer),0)==-1){
                perror("send");
                break;
            }
            if(strncmp(buffer,"exit",4)==0){
                printf("[%s] Exit from chat\n",a);
                break;
            }
        }
        if (FD_ISSET(socket_fd,&readfds)){
            memset(buffer,0,BUF_SIZE);
            bytes_rec = recv (socket_fd,buffer,BUF_SIZE-1,0);
            if (bytes_rec==-1){
                perror("recv");
                break;
            }
            if (bytes_rec==0){
                printf("chat has ended\n");
                break;
            }
            buffer[bytes_rec]='\0';
            printf(">%s",buffer);
            if (strncmp(buffer,"exit",4)==0){
                printf("[%s] User left from the chat.\n");
                break;
            }
        }
    }
}

int main(int argc ,char *argv[])
{

}