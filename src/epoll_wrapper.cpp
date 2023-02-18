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

#include "log.hpp"

inline int setnonblocking(int fd) {
  int old_option{fcntl(fd, F_GETFL)};
  int new_option{old_option | O_NONBLOCK};
  fcntl(fd, F_SETFL, new_option);
  return old_option;
}

epoll_wrapper::epoll_wrapper(bool enable_et, int timeout)
    : enable_et_(enable_et),
      connect_event_flags_(EPOLLONESHOT | EPOLLRDHUP),
      listen_event_flags_(EPOLLRDHUP),
      timeout_(timeout) {
  events_ = new epoll_event[MAX_EVENTS];
  if (enable_et_) {
    connect_event_flags_ |= EPOLLET;
  }
  // EPOLLRDHUP宏，底层处理socket连接断开的情况
  // EPOLLONESHOT 和 ET 模式不尽相同：
  // 前者是防止一个客户端发送的数据被多个线程分散读取；
  // 后者是避免多次调用epoll_wait提高epoll效率的一种模式
}

epoll_wrapper::~epoll_wrapper() { delete[] events_; }

void epoll_wrapper::create(int max_fd) {
  epollfd_ = epoll_create(max_fd);
  std::cout << std::format("epoll_create: {}", epollfd_) << std::endl;
}

// 将文件描述符 fd 上的 EPOLLIN 注册到 epollfd
void epoll_wrapper::add(int fd, bool is_listen, uint32_t event_flag) {
  epoll_event event;
  event.data.fd = fd;
  if (is_listen) {
    event.events = listen_event_flags_ | event_flag;
  } else {
    event.events = connect_event_flags_ | event_flag;
  }
  epoll_ctl(epollfd_, EPOLL_CTL_ADD, fd, &event);
  setnonblocking(fd);
}

void epoll_wrapper::mod(int fd, uint32_t ev) {
  epoll_event event;
  event.data.fd = fd;
  event.events = connect_event_flags_ | ev;
  epoll_ctl(epollfd_, EPOLL_CTL_MOD, fd, &event);
  setnonblocking(fd);
  std::cout << "modified events" << std::endl;
}

void epoll_wrapper::del(int fd) {
  // if (fd < 0) {
  //   return false;
  // }
  epoll_ctl(epollfd_, EPOLL_CTL_DEL, fd, NULL);
}

int epoll_wrapper::wait() {
  auto ret{epoll_wait(epollfd_, events_, MAX_EVENTS, timeout_)};
  if (ret < 0) {
    std::cerr << "epoll failure\n";
  }
  return ret;
}

void epoll_wrapper::run(int listenfd, std::function<void()> accept,
                        std::function<void(int)> handle_read,
                        std::function<void(int)> handle_write,
                        std::function<void(int)> handle_close) {
  std::cout << "epoller started to run" << std::endl;
  auto number{wait()};
  std::cout << std::format("got {} fds with new edge", number) << std::endl;
  auto& log{Logger::get()};
  // auto BUFFER_SIZE{100};
  // char buf[BUFFER_SIZE];
  for (int i = 0; i < number; i++) {
    auto sockfd{events_[i].data.fd};
    auto events{events_[i].events};
    if (sockfd == listenfd) {
      std::cout << "listen" << std::endl;
      accept();
    } else if (events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR)) {
      // 对端关闭了连接
      log.info(std::format("close fd {}", sockfd));
      del(sockfd);
      handle_close(sockfd);
    } else if (events & EPOLLIN) {
      // 读事件
      // 现在主线程中读取数据到缓冲区中；
      // 再由线程池完成业务逻辑
      // 读取http请求

      // 需要关联文件描述符和链接对象
      log.info(std::format("read fd {}", sockfd));
      handle_read(sockfd);
    } else if (events & EPOLLOUT) {
      // 写事件
      log.info(std::format("write fd {}", sockfd));
      handle_write(sockfd);
    } else {
      log.error("something else happened \n");
    }
  }
}