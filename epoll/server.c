#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>

#define MAX_EVENT_NUMBER 1024
#define BUFFER_SIZE 10

// gcc -Wall -std=c99 server.c -o server 

int setnonblocking(int fd) {
	int old_option = fcntl(fd, F_GETFL);
	int new_option = old_option | O_NONBLOCK;
	fcntl(fd,F_SETFL,new_option);
	return old_option;
}

void addfd(int epollfd,int fd, int  enable_et){
	struct epoll_event event;
	event.data.fd = fd;
	event.events = EPOLLIN;
	if (enable_et) {
		event.events |= EPOLLET;
	}
	epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event);
	setnonblocking(fd);
}

// lt工作流程
void lt(struct epoll_event* events, int number, int epollfd,int listenfd) {
	char buf[BUFFER_SIZE];
	for (int i = 0; i < number; ++i) {
		int sockfd = events[i].data.fd;
		if (sockfd == listenfd){
			struct sockaddr_in client_address;
			socklen_t client_addrlength = sizeof(client_address);
			int connfd = accept(listenfd,(struct sockaddr*)&client_address,
				&client_addrlength);
			addfd(epollfd,connfd,0); // 对connfd 禁用ET
		}else if (events[i].events & EPOLLIN) {
			// 只要socket里面读缓存中还有未读的数据，这段代码被触发
			printf("event trigger once\n");
			memset(buf,'\0',BUFFER_SIZE);
			int ret = recv(sockfd,buf,BUFFER_SIZE-1,0);
			if(ret <= 0) {
				close(sockfd);
				continue;
			}
			printf("get %d bytes of content:%s\n", ret, buf );
		}else {
			printf("something else happend\n");
		}
	}
}

// et工作流程
void et(struct epoll_event* events,int number, int epollfd, int listenfd) {
	char buf[BUFFER_SIZE];
	for (int i = 0; i < number; ++i) {
		int sockfd = events[i].data.fd;
		if (sockfd == listenfd) {
			int sockfd = events[i].data.fd;
			if (sockfd == listenfd) {
				struct sockaddr_in client_address;
				socklen_t client_addrlength = sizeof(client_address);
				int connfd = accept(listenfd,(struct sockaddr*)&client_address,
					&client_addrlength);
				addfd(epollfd,connfd,1);
			}
		}else if(events[i].events & EPOLLIN) {
			// 这段代码不断重复触发,所以我们循环读取数据,以确保把socket读取缓存中的所有数据读出
			// 假设现在对方一口气发送了大量数据过来，以至于这边没法通过一次read就全部读取完毕。
			// 由于ET模式只会通知一次，所以这边对应的数据读取函数中不得不循环read多次，以保证把对方本次发送的数据全部读取完毕
			printf("et event trigger once\n");
			while(1) {
				memset(buf,'\0',BUFFER_SIZE);
				int ret = recv(sockfd,buf,BUFFER_SIZE-1,0);
				if (ret < 0) {
					// 对于非阻塞IO,下面的条件成立表示数据已经全部读取完毕。
					// 此后,epoll就能再次触发sockfd上面的EPOLLIN事情,以驱动下一次读操作。
					if( (errno== EAGAIN) || (errno == EWOULDBLOCK)){
						printf("read later\n");
						break;
					}
					close(sockfd);
					break;
				}else if ( ret == 0) {
					close(sockfd);
				}else {
					printf("get %d bytes of content:%s\n",ret,buf);
				}
			}
		}else {
			printf("something else happend\n");
		}
	}
}

// server localhost 6666 1 
int main(int argc, char* argv[]) {
	if (argc <= 2) {
		printf("usage: %s ip_address port_numbder\n", basename(argv[0]) );
		return 1;
	}

	const char* ip = argv[1];
	int port = atoi(argv[2]);
	int ret = 0;
	int isEt = 0;
	if (argc == 4) 	{
		isEt = atoi(argv[3]);
	}
	struct  sockaddr_in address;
	bzero(&address, sizeof(address));
	address.sin_family = AF_INET;
	inet_pton(AF_INET,ip,&address.sin_addr);
	address.sin_port = htons(port);

	int listenfd = socket(PF_INET,SOCK_STREAM,0);
	assert( listenfd >= 0);

	ret = bind(listenfd,(struct sockaddr*)&address, sizeof(address));
	assert(ret != -1);

	ret = listen(listenfd,5);
	assert(ret != -1);

	struct epoll_event events[MAX_EVENT_NUMBER];
	int epollfd = epoll_create(5);
	assert(epollfd != -1);
	addfd(epollfd,listenfd,1);

	while( 1 ) {
		int ret = epoll_wait(epollfd,events,MAX_EVENT_NUMBER,-1);
		if ( ret < 0) {
			printf("epoll failure\n");
			break;
		}
		if (isEt){
			et(events,ret, epollfd,listenfd);
		}else {
			lt(events,ret, epollfd,listenfd);
		}
	}
	close(listenfd);
	return 0;
}

