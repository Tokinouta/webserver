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
  void add(int fd, bool is_listen = false, uint32_t event_flag = EPOLLIN);
  void del(int fd);
  void mod(int fd, uint32_t ev);
  int wait();
  void run(int listenfd, std::function<void()> accept,
           std::function<void(int)> handle_read,
           std::function<void(int)> handle_write,
           std::function<void(int)> handle_close);

 private:
  int epollfd_;
  bool enable_et_;
  uint32_t connect_event_flags_;
  uint32_t listen_event_flags_;
  epoll_event* events_;
  int timeout_;
};

#endif