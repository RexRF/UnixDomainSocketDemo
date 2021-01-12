#include <stdlib.h>  
#include <stdio.h>  
#include <stddef.h>  
#include <sys/socket.h>  
#include <sys/un.h>  
#include <errno.h>  
#include <string.h>  
#include <unistd.h>
#include <pthread.h>  
 
#define MAXLINE 80  
 
const char *server_path = "server";  

void* _readThread(void* data) {
	int sockfd = *(int *)data;
    char readbuf[MAXLINE];
	while(read(sockfd, readbuf, MAXLINE) > 0){
		printf("%2d : %s", sockfd, readbuf);
		memset(readbuf, 0, sizeof(readbuf));		
	}
	perror("read error"); 
	pthread_exit(NULL); 
}


int main() {  
    struct  sockaddr_un serun;  
    int len;  
    char writebuf[MAXLINE];  
    int sockfd;
	pthread_t readThread;
 
    if ((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){  
        perror("client socket error");  
        exit(1);  
    }  
 
    memset(&serun, 0, sizeof(serun));  
    serun.sun_family = AF_UNIX;  
    strcpy(serun.sun_path, server_path);  
    len = offsetof(struct sockaddr_un, sun_path) + strlen(serun.sun_path);  
    if (connect(sockfd, (struct sockaddr *)&serun, len) < 0){  
        perror("connect error");  
        exit(1);  
    }  
	
	pthread_create(&readThread, NULL, _readThread, &sockfd);
	
    while(fgets(writebuf, MAXLINE, stdin) != NULL) {    
		write(sockfd, writebuf, strlen(writebuf));
		memset(writebuf, 0, sizeof(writebuf));	
    }   
    close(sockfd);  
    return 0;  
}
