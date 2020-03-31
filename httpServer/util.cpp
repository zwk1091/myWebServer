#include "util.h"
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

void do_read(int epollfd , int fd,char *buf) {
	int nread ;
	nread = read(fd , buf , MAXSIZE) ;
	if (nread == -1) {
		perror ( " read error :") ;
		close(fd) ;
		delete_event(epollfd , fd , EPOLLIN) ;
	} else if (nread == 0){
		fprintf(stderr , "client close . \n") ;
		close (fd) ;
		delete_event(epollfd,fd,EPOLLIN) ;
	} else {
		printf ("read message is ：%s" ,buf);
// 修改描述符对应的事件，由读改为写＊／
		modify_event (epollfd, fd, EPOLLOUT);
	}
}
void do_write(int epollfd,int fd,char *buf) {
	int nwrite;
	nwrite = write(fd,buf,strlen(buf));
	if (nwrite == -1){
		perror ("write error : ");
		close (fd);
		delete_event(epollfd,fd,EPOLLOUT) ;
	} else
		modify_event(epollfd , fd , EPOLLIN) ;
	memset(buf,0,MAXSIZE);
}