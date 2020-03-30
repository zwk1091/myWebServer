#ifndef EVENTPOLL
#define EVENTPOLL
// 添加事件
void add_event(int epollfd , int fd,int state);
// 修改事件
void modify_event(int epollfd , int fd , int state);
// 删除事件
void delete_event(int epollfd , int fd , int state);

#endif