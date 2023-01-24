#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include <netinet/in.h>

#include <sstream>

class webserver {
 private:
  int sock;
  struct sockaddr_in address;
  std::stringstream content_buf;

 public:
  webserver(const char* ip, int port);
  ~webserver();
  void print();
  void connect();
  void handle(int connfd);
};

#endif