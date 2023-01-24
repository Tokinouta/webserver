#ifndef EPOLL_WRAPPER_HPP
#define EPOLL_WRAPPER_HPP

#include <vector>
#include <sys/epoll.h>

class epoll_wrapper {
 private:
  int epollfd_;
  bool enable_et_;
  std::vector<epoll_event> events_;
  int timeout_;

 public:
  epoll_wrapper(bool enable_et, int timeout);
  ~epoll_wrapper();

  void create(int max_fd);
  void add(int fd);
  int wait();
  void et(int listenfd);
};

#endif