#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/un.h>
#include<sys/select.h>

#define BUF_SIZE 256
#define SOCKET_PATH "/tmp/chat"

void chat(int socket_fd, const char *a)
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
                printf("[%s] User left from the chat.\n",a);
                break;
            }
        }
    }
}

void process_for_chatting(const char *mode)
{
    struct sockaddr_un server_addr;
    int socket_fd;
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sun_family=AF_UNIX;
    strncpy (server_addr.sun_path, SOCKET_PATH,sizeof(server_addr.sun_path)-1);
    server_addr.sun_path[sizeof(server_addr.sun_path)-1]='\0';

    if(strcmp(mode, "server")==0){
        printf("Server mode.\n");
        int server_fd,client_fd;
        struct sockaddr_un client_addr_info; 
        int client_len=sizeof(client_addr_info);
        
        if((server_fd = socket(AF_UNIX,SOCK_STREAM,0))==-1)
        {
            perror("socket");
            EXIT_FAILURE;
        }
        unlink(SOCKET_PATH);
        if (bind(server_fd,(struct sockaddr*)&server_addr,sizeof(server_addr))==-1)
        {
            close(server_fd);
            unlink(SOCKET_PATH);
            perror("bind");
            EXIT_FAILURE;
        }
        if (listen(server_fd,1)==-1)
        {
            close (server_fd);
            unlink(SOCKET_PATH);
            perror("listen");
            EXIT_FAILURE;
        }
        printf("ready for listen!\n");
        if ((socket_fd=accept(server_fd,(struct sockaddr*)&client_addr_info,&client_len))==-1)
        {
            close(server_fd);
            unlink(SOCKET_PATH);
            perror("client");
            EXIT_FAILURE;
        }
        printf("client on the server\n");
        chat(socket_fd,"client");
        
    } else if (strcmp(mode,"client")==0){
        printf("client mode\n");
        int client_sockfd;
        if((client_sockfd=socket(AF_UNIX,SOCK_STREAM,0))==-1)
        {
            perror("socket (client)\n");
            EXIT_FAILURE;
        }
        printf("connecting to the server...\n");
        if(connect(client_sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr))==-1){
            close(client_sockfd);
            perror("connect (client\n");
            EXIT_FAILURE;
        }
        printf("complete!\n");
        chat(client_sockfd,"server");
    }

}

int main(int argc ,char *argv[])
{

}