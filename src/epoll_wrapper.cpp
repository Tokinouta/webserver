#include "epoll_wrapper.hpp"

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <format>
#include <iostream>

inline int setnonblocking(int fd) {
  int old_option{fcntl(fd, F_GETFL)};
  int new_option{old_option | O_NONBLOCK};
  fcntl(fd, F_SETFL, new_option);
  return old_option;
}

epoll_wrapper::epoll_wrapper(bool enable_et, int timeout)
    : enable_et_(enable_et), timeout_(timeout) {
  events_ = new epoll_event[MAX_EVENTS];
}

epoll_wrapper::~epoll_wrapper() { delete[] events_; }

void epoll_wrapper::create(int max_fd) {
  epollfd_ = epoll_create(max_fd);
  std::cout << std::format("epoll_create: {}", epollfd_) << std::endl;
}

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
  auto ret{epoll_wait(epollfd_, events_, MAX_EVENTS, timeout_)};
  if (ret < 0) {
    std::cerr << "epoll failure\n";
  }
  return ret;
}

void epoll_wrapper::run(int listenfd, std::function<void()> accept,
                        std::function<void(int)> handle) {
  std::cout << "epoller started to run" << std::endl;
  auto number{wait()};
  std::cout << std::format("got {} fds with new edge", number) << std::endl;
  // auto BUFFER_SIZE{100};
  // char buf[BUFFER_SIZE];
  for (int i = 0; i < number; i++) {
    auto sockfd{events_[i].data.fd};
    auto events{events_[i].events};
    if (sockfd == listenfd) {
      std::cout << "listen" << std::endl;
      accept();
    } else if (events & EPOLLIN) {
      // 读事件
      // 现在主线程中读取数据到缓冲区中；
      // 再由线程池完成业务逻辑
      // 读取http请求
      std::cout << "handle" << std::endl;
      handle(sockfd);
    } else if (events & EPOLLOUT) {
      // 写事件
      // handleWrite(&m_usrs[currfd]);
    } else if (events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR)) {
      // 对端关闭了连接
      // closeConn(&m_usrs[currfd]);
    } else {
      printf("something else happened \n");
    }
  }
}