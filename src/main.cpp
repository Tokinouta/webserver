#include <arpa/inet.h>
#include <assert.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>
#include <format>
#include <iostream>
#include <string>

#include "webserver.hpp"

int main(int argc, char* argv[]) {
  std::cout << "hello world!" << std::endl;
  if (argc <= 2) {
    std::cout << std::format("Usage: {} <ip address> <port>", argv[0]) << std::endl;
    return 1;
  }
  auto ip{argv[1]};
  auto port{atoi(argv[2])};
  auto a{webserver(ip, port)};
  a.print();
}
