#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <arpa/inet.h>
#include <string.h>
using namespace std;
#define DEST_PORT 1500
#define DEST_IP "127.0.0.1"
#define BUFFSIZE 1024

int main(){
	int sockfd, new_fd;
	struct sockaddr_in dest_addr;
	char sendData[BUFFSIZE];
	char recvData[BUFFSIZE];
	fd_set readfd;
    struct timeval timeout;
	int max;
	int ret;

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		cout << "socket error!" << endl;
		return -1;
	}


	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(DEST_PORT);
	dest_addr.sin_addr.s_addr = inet_addr(DEST_IP);
	bzero(&(dest_addr.sin_zero), 8);

	if(connect(sockfd, (struct sockaddr*)&dest_addr, sizeof(struct sockaddr)) < 0){
		cout << "connect error!" << endl;
		close(sockfd);
		return -1;
	}
	
	while(true){
		timeout.tv_sec = 1;
  		timeout.tv_usec = 0;
  		FD_ZERO(&readfd);
  		FD_SET(sockfd, &readfd); 
		FD_SET(fileno(stdin), &readfd);
		max = sockfd > fileno(stdin) ? sockfd : fileno(stdin);
		
        if(select(max + 1, &readfd, NULL, NULL, &timeout) == 0)
    	    continue;
        if(FD_ISSET(sockfd, &readfd)){
      			ret = recv(sockfd, recvData, BUFFSIZE, 0);
          		cout << "server: " << recvData << endl;
  		}
		if(FD_ISSET(fileno(stdin), &readfd)){
			scanf("%s",sendData);
			if(strcmp(sendData, "quit") == 0)
				break;
			if(send(sockfd, sendData, BUFFSIZE, 0) == -1){
            	printf("send error!\n");
       		 }
		}             
	}
	close(sockfd);
	return 0;	
}
