#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>

#include "epoll_wrapper.hpp"
#include "http_connection.hpp"

class webserver {
 private:
  int sock;
  struct sockaddr_in address;
  std::stringstream content_buf;
  std::unique_ptr<epoll_wrapper> epoller_;
  std::unordered_map<int, HttpConnection> connection_;

 public:
  webserver(const char* ip, int port, bool enable_et, int timeout);
  ~webserver();
  void print();
  void connect();
  void accept_connection();
  void handle_read(int connfd);
  void handle_write(int connfd);
  void handle_close(int connfd);
  void run();
};

#endif