#include "epoll_wrapper.hpp"

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>

inline int setnonblocking(int fd) {
  int old_option{fcntl(fd, F_GETFL)};
  int new_option{old_option | O_NONBLOCK};
  fcntl(fd, F_SETFL, new_option);
  return old_option;
}

epoll_wrapper::epoll_wrapper(bool enable_et, int timeout)
    : enable_et_(enable_et), timeout_(timeout) {}

epoll_wrapper::~epoll_wrapper() {}

void epoll_wrapper::create(int max_fd) { epollfd_ = epoll_create(max_fd); }

// 将文件描述符fd 上的EPOLLIN 注册到epollfd
// 指示的epoll内核事件表中，参数enable_et指定是否对fd启用ET模式
void epoll_wrapper::add(int fd) {
  epoll_event event;
  event.data.fd = fd;
  event.events = EPOLLIN;
  if (enable_et_) {
    event.events |= EPOLLET;
  }
  epoll_ctl(epollfd_, EPOLL_CTL_ADD, fd, &event);
  setnonblocking(fd);
}

int epoll_wrapper::wait() {
  auto ret{epoll_wait(epollfd_, &events_[0], events_.size(), timeout_)};
  if (ret < 0) {
    std::cerr << "epoll failure\n";
  }
  return ret;
}

void epoll_wrapper::et(int listenfd) {
  auto number{wait()};
  auto BUFFER_SIZE{100};
  char buf[BUFFER_SIZE];
  for (int i = 0; i < number; i++) {
    int sockfd = events_[i].data.fd;
    if (sockfd == listenfd) {
      struct sockaddr_in client_address;
      socklen_t client_addrlength = sizeof(client_address);
      int connfd{accept(listenfd, (struct sockaddr*)&client_address,
                        &client_addrlength)};
      add(connfd);  // 对connfd开启ET模式
    } else if (events_[i].events & EPOLLIN) {
      // 这段代码不会被重复触发，所以我们循环读取数据，以确保把socket
      // 读缓存中的所有数据读出
      printf("event trigger once\n");
      while (1) {
        memset(buf, '\0', BUFFER_SIZE);
        auto ret{recv(sockfd, buf, BUFFER_SIZE - 1, 0)};
        if (ret < 0) {
          // 对于非阻塞IO，下面的条件成立表示数据巳经全部读取完毕．此后，epoll就能再次触发sockfd上的EPOLLIN事件．以驱动下一次读操作
          if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
            printf("read later\n");
            break;
          }
          close(sockfd);
          break;
        } else if (ret == 0) {
          close(sockfd);
        } else {
          printf("get %d bytes of content: %s\n", ret, buf);
        }
      }
    } else {
      printf("something else happened \n");
    }
  }
}