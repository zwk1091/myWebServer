#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <sys/types.h>
#include "util.h"
#include "epoll.h"

#define IPADDRESS "127.0.0.5"
#define PORT 5555
#define MAXSIZE 1024
#define LISTENQ 5
#define FDSIZE 1000
#define EPOLLEVENTS 100

// ＊创建套接字并进行绑定 
int socket_bind(const char * ip , int port) ;
// I0 多路复用 epoll
void do_epoll(int listenfd) ;
// 事件处理函数 
void handle_events (int epollfd, struct epoll_event *events , int num, int
listenfd, char *buf);
// 处理接收到的连接
void handle_accpet(int epollfd , int listenfd) ;

// // 读处理
// void do_read(int epollfd ,int fd,char *buf);
// // 写处理
// void do_write(int epollfd,int fd,char *buf);


void do_epoll(int listenfd) {
	int epollfd ;
	struct epoll_event events [EPOLLEVENTS];
	int ret ;
	char buf[MAXSIZE] ;
	memset(buf,0 , MAXSIZE) ;
// 创建一个描述符
	epollfd = epoll_create(FDSIZE) ;
// 添加监听描述符事件
	add_event(epollfd,listenfd , EPOLLIN) ;
	while(1) {
// 获取已经准备好的描述符事件 
		ret = epoll_wait(epollfd , events , EPOLLEVENTS , -1);
		// printf("*******************************ret%d\n", ret);
		handle_events(epollfd , events , ret , listenfd , buf) ;
	}
	close (epollfd);
}

void handle_accpet(int epollfd,int listenfd) {
	int clifd;
	struct sockaddr_in cliaddr;
	socklen_t cliaddrlen;
	clifd = accept(listenfd , (struct sockaddr*)&cliaddr , (socklen_t*)&cliaddrlen);
	// printf("**********************************%d \n", clifd);
	if (clifd == -1) {
		perror ( "accept error :");
	} else {
		printf(" accept a new clients ： %s, %d\n " , inet_ntoa(cliaddr.sin_addr) , cliaddr.sin_port) ;
// ／ ＊ 添加一个客户描述符和事件＊／
		add_event(epollfd, clifd,EPOLLIN);
	}
}



void handle_events (int epollfd , struct epoll_event *events, int num , int
listenfd , char *buf) {
	int i;
	int fd ;
	// printf("num: %d\n",num);
// 进行选好遍历
	for(i=0; i<num;i++){
		
		fd = events[i].data.fd;
		// printf("%d\n", fd);
// ／＊根据描述符的类型和事件类型进行处理＊／
		// handle connect request
		if ( (fd == listenfd) && (events[i].events & EPOLLIN))
			handle_accpet(epollfd , listenfd);
		// handle read request
		else if (events[i].events & EPOLLIN)
			do_read(epollfd , fd , buf);
		// handle write request
		else if (events[i].events & EPOLLOUT)
			do_write(epollfd, fd , buf);
	}
}

int socket_bind() {
	// create socket
	int listenfd;
	struct sockaddr_in servaddr ;
	if( (listenfd = socket(AF_INET, SOCK_STREAM,0))==-1){
		printf("create socket error: %s(errno ：%d) \n", strerror(errno),errno);
		return 0;
	} 

	printf("listenfd: %d",listenfd);
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET ;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(6666);
	if( bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {
		printf ("bind socket error ： %s(errno ：%d)\n",strerror(errno),errno);
		return 0;
	}
	return listenfd;
	// int listenfd ;
	// struct sockaddr_in servaddr ;
	// listenfd = socket(AF_INET, SOCK_STREAM,0);
	// if (listenfd == -1) {
	// 	perror ("socket error : ") ;
	// 	exit (1) ;
	// }
	// perror("socket!");
	// bzero(&servaddr,sizeof (servaddr)) ;
	// servaddr.sin_family = AF_INET;

	// // inet_pton(AF_INET, ip , &servaddr.sin_addr) ;
	// servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	// servaddr.sin_port = htons(port) ;
	// if (bind(listenfd, (struct sockaddr*)&servaddr , sizeof(servaddr)) == -1) {
	// 	perror ( "bind error:") ;
	// 	exit (1);
	// 	return listenfd;
	// }
	// perror("bind!");
}

int main(int argc, char** argv) {
	int listenfd, connfd;
	// struct sockaddr_in servaddr ;
	char buff[4096];
	int n;
	listenfd = socket_bind();
	// // create socket
	// if( (listenfd = socket(AF_INET, SOCK_STREAM,0))==-1){
	// 	printf("create socket error: %s(errno ：%d) \n", strerror(errno),errno);
	// 	return 0;
	// } 

	// printf("listenfd: %d",listenfd);
	// memset(&servaddr, 0, sizeof(servaddr));
	// servaddr.sin_family = AF_INET ;
	// servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	// servaddr.sin_port = htons(6666);
	// if( bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {
	// 	printf ("bind socket error ： %s(errno ：%d)\n",strerror(errno),errno);
	// 	return 0;
	// }
	// listen to this socket
	if( listen(listenfd, 10) == -1){
		printf ("listen socket error ：%s(errno ：%d) \n", strerror(errno),errno);
		return 0;
	}

	do_epoll(listenfd);
	// printf ("======waiting for client ’ s request======\n");
	// while (1) {
	// 	if( (connfd = accept(listenfd , (struct sockaddr*)NULL, NULL))== -1) {
	// 		printf ( "accept socket error ： %serrno ： %d)" ,strerror(errno) , errno) ;
	// 		continue ;
	// 	}
	// 	printf("************************");
	// 	printf("%d\n",connfd);
	// 	n = recv(connfd,buff,MAXLINE,0);
	// 	buff[n] ='\0';
	// 	printf ("recv msg from client ： %s\n", buff);
	// 	close (connfd);
	// }
	close(listenfd);
	return 0 ;
}