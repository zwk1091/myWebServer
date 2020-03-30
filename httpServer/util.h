#include <cstdlib>
#include "epoll.h"
#include <sys/epoll.h>


#define IPADDRESS "127.0.0.5"
#define PORT 5555
#define MAXSIZE 1024
#define LISTENQ 5
#define FDSIZE 1000
#define EPOLLEVENTS 100

void do_read(int epollfd , int fd,char *buf);
void do_write(int epollfd,int fd,char *buf);