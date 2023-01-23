#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include <netinet/in.h>

class webserver {
 private:
  int sock;
  struct sockaddr_in address;

 public:
  webserver(const char* ip, int port);
  ~webserver();
  void print();
  void connect();
  void handle();
};

#endif