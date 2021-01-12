#include <stdlib.h>  
#include <stdio.h>  
#include <stddef.h>  
#include <sys/socket.h>  
#include <sys/un.h>  
#include <errno.h>  
#include <string.h>  
#include <unistd.h>  
#include <ctype.h>
#include <unistd.h>  
#include <pthread.h> 
 
#define MAXLINE 80  
 
const char *socket_path = "server";  

void* _writeThread(void* data) {
	char writebuf[MAXLINE];
	while(fgets(writebuf, MAXLINE, stdin) != NULL){
		write( *(int *)data, writebuf, strlen(writebuf));
		memset(writebuf, 0, sizeof(writebuf));	
	}
	pthread_exit(NULL);
}

int main(void)  
{  
    struct sockaddr_un serun, cliun;  
    socklen_t cliun_len;  
    int listenfd, connfd, size;  
    char readbuf[MAXLINE] = {0};  
	pthread_t writeThread;
	
    if ((listenfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {  
        perror("socket error");  
        exit(1);  
    }  
	
	pthread_create(&writeThread, NULL, _writeThread, &connfd);
    memset(&serun, 0, sizeof(serun));  
    serun.sun_family = AF_UNIX;  
    strcpy(serun.sun_path, socket_path);  
    size = offsetof(struct sockaddr_un, sun_path) + strlen(serun.sun_path);  
    unlink(socket_path);  
    if (bind(listenfd, (struct sockaddr *)&serun, size) < 0) {  
        perror("bind error");  
        exit(1);  
    }  
      
    if (listen(listenfd, 20) < 0) {  
        perror("listen error");  
        exit(1);          
    }  
    printf("Accepting connections ...\n");  
	
	cliun_len = sizeof(cliun);
	
	
	while((connfd = accept(listenfd, (struct sockaddr *)&cliun, &cliun_len)) >= 0)
	{
		while(read(connfd, readbuf, sizeof(readbuf)) > 0) {
			
			printf("%2d : %s", connfd, readbuf);  
			
			if(strcmp(readbuf, "sleep\n") == 0)
			{
				printf("sleep..\n");
				sleep(10);
			}
			memset(readbuf, 0, sizeof(readbuf));
		}  
	}
	perror("accept error");
    close(connfd);        
    close(listenfd);  
    return 0;  
} 