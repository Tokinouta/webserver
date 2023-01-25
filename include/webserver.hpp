#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include <netinet/in.h>

#include <memory>
#include <sstream>

#include "epoll_wrapper.hpp"

class webserver {
 private:
  int sock;
  struct sockaddr_in address;
  std::stringstream content_buf;
  std::unique_ptr<epoll_wrapper> epoller_;

 public:
  webserver(const char* ip, int port, bool enable_et, int timeout);
  ~webserver();
  void print();
  void connect();
  void accept_connection();
  void handle(int connfd);
  void run();
};

#endif