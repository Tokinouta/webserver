#ifndef EPOLL_WRAPPER_HPP
#define EPOLL_WRAPPER_HPP

#include <sys/epoll.h>

#include <functional>
#include <vector>

class epoll_wrapper {
 public:
  static const int MAX_EVENTS{65535};
  epoll_wrapper(bool enable_et, int timeout);
  ~epoll_wrapper();

  void create(int max_fd);
  void add(int fd);
  void del(int fd);
  int wait();
  void run(int listenfd, std::function<void()> accept,
           std::function<void(int)> handle_read,
           std::function<void(int)> handle_write,
           std::function<void(int)> handle_close);

 private:
  int epollfd_;
  bool enable_et_;
  // std::vector<epoll_event> events_;
  epoll_event* events_;
  int timeout_;
};

#endif