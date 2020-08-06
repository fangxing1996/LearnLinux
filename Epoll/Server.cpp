#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <arpa/inet.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/epoll.h>
using namespace std;
#define BUFFSIZE 1024
#define PORT 1500
#define BACKLOG 5
int main(){

	int epfd, nfds;
    struct epoll_event ev0, events[5];
    epfd = epoll_create(2);
    ev0.data.fd = STDIN_FILENO;
    ev0.events = EPOLLIN|EPOLLET;
    epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &ev0);



	int sockfd, new_fd;
	int ret;
	int max;
	struct sockaddr_in my_addr, their_addr;
	unsigned int sin_size1 = sizeof(struct sockaddr);
	unsigned int sin_size2 = sizeof(struct sockaddr);
	//fd_set readfd;
    struct timeval timeout;
	char sendData[BUFFSIZE];
    char recvData[BUFFSIZE];

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1){
		printf("socket failed: %d\n", errno);
		return -1;
	}

	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(PORT);
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(sockfd, (struct sockaddr*)&my_addr, sin_size1) < 0){
		printf("bind error!\n");
		return -1;
	}
	listen(sockfd, BACKLOG);
	cout << "waitting fot connect!\n";
	if((new_fd = accept(sockfd, (struct sockaddr*)&their_addr, &sin_size2)) < 0)
    {
        printf("accept error!\n");
        close(sockfd);
        return -1;
    }
	struct epoll_event ev1;
    ev1.data.fd = new_fd;
    ev1.events = EPOLLIN|EPOLLET;
	epoll_ctl(epfd, EPOLL_CTL_ADD, new_fd, &ev1);
	printf("dectect a connect.\nIP: %s.\nport: %d.\n", inet_ntoa(their_addr.sin_addr), ntohs(their_addr.sin_port));
	while(1){
		// timeout.tv_sec = 1;
  		// timeout.tv_usec = 0;
  		// FD_ZERO(&readfd);
  		// FD_SET(new_fd, &readfd); 
		// FD_SET(fileno(stdin), &readfd);
		// max = new_fd > fileno(stdin) ? new_fd : fileno(stdin);
        // if(select(max + 1, &readfd, NULL, NULL, &timeout) == 0)
    	//     continue;
        // if(FD_ISSET(new_fd, &readfd)){
      	// 		ret = recv(new_fd, recvData, BUFFSIZE, 0);
        //   		cout << "client: " << recvData << endl;
  		// }
		// if(FD_ISSET(fileno(stdin), &readfd)){
		// 	scanf("%s",sendData);
		// 	if(send(new_fd, sendData, BUFFSIZE, 0) == -1){
        //     	printf("send error!\n");
        //     	break;
       	// 	 }
		// }
		nfds = epoll_wait(epfd, events, 5, -1);
			for(int i = 0; i < nfds; i++){  
            	if(events[i].data.fd == STDIN_FILENO){
					
						scanf("%s", sendData);
						if(send(new_fd, sendData, BUFFSIZE, 0) == -1){
            				printf("send error!\n");
            				break;
       		 			}
					
				}
				if(events[i].data.fd == new_fd){
						ret = recv(new_fd, recvData, BUFFSIZE, 0);
          				cout << "client: " << recvData << endl;
				}
				
			}
		}
	close(epfd);
	close(new_fd);
	close(sockfd);
	return 0;
}
