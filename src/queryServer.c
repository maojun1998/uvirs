#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAXLINE 100

int main(int argc,char **argv)
{
    char *servInetAddr = "59.110.225.100";
    int socketfd;
    struct sockaddr_in sockaddr;
    char recvline[MAXLINE];
    int n;
 

 
    socketfd = socket(AF_INET,SOCK_STREAM,0);
    memset(&sockaddr,0,sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(6000);
    inet_pton(AF_INET,servInetAddr,&sockaddr.sin_addr);
    
    if((connect(socketfd,(struct sockaddr*)&sockaddr,
                                        sizeof(sockaddr))) < 0) {
        printf("connect error %s errno: %d\n", strerror(errno), errno);
        return 0;
    }


    if((send(socketfd, "q" ,strlen("q") + 1 ,0)) < 0) {
        printf("send mes error: %s errno : %d\n", strerror(errno), errno);
        return 0;
    }
    recv(socketfd, recvline, MAXLINE, 0);
    
    printf("%s\n", recvline);
    
    close(socketfd);
    exit(0);
 }