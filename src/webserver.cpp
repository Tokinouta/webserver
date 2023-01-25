#include "webserver.hpp"

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
#include <string>

namespace fs = std::filesystem;
using std::string;

static const size_t BUFFER_SIZE{1024};
static const char* status_line[2]{"200 OK", "500 Internal server error"};

webserver::webserver(const char* ip, int port, bool enable_et, int timeout)
    : epoller_(std::make_unique<epoll_wrapper>(enable_et, timeout)) {
  memset(&address, 0, sizeof(address));
  address.sin_family = AF_INET;
  inet_pton(AF_INET, ip, &address.sin_addr);
  address.sin_port = htons(port);
  epoller_->create(5);
}

webserver::~webserver() { close(sock); }

void webserver::print() { std::cout << std::format("{}", sock) << std::endl; }

void webserver::connect() {
  sock = socket(PF_INET, SOCK_STREAM, 0);
  if (bind(sock, (struct sockaddr*)&address, sizeof(address)) == -1) {
    std::cerr << "error binding" << std::endl;
    return;
  }
  if (listen(sock, 5) == -1) {
    std::cerr << "error listening" << std::endl;
    return;
  }
  epoller_->add(sock);  // 添加监听端口
}

void webserver::accept_connection() {
  struct sockaddr_in client;
  socklen_t client_addrlength = sizeof(client);
  int connfd = accept(sock, (struct sockaddr*)&client, &client_addrlength);
  if (connfd < 0) {
    std::cerr << std::format("errno is {}", errno) << std::endl;
  } else {
    epoller_->add(connfd);  // 对connfd开启ET模式
    // handle(connfd);
  }
}

void webserver::handle(int connfd) {
  // 用于保存HTTP应答的状态行、头部字段和一个空行的缓存区
  // char header_buf[BUFFER_SIZE];
  // memset(header_buf, 0, BUFFER_SIZE);
  std::ifstream file_buf;
  string file_content;

  string file_name{fs::current_path().append("test.txt")};

  std::cout << file_name << std::endl;
  auto valid{true};  // 记录目标文件是否是有效文件
  if (!fs::exists(file_name)) {
    std::cerr << "File not exists." << std::endl;
    valid = false;
  } else if (fs::is_directory(file_name)) {
    std::cerr << "File is a directory." << std::endl;
    valid = false;
  } else {
    auto perms{fs::status(file_name).permissions()};
    if ((perms & fs::perms::owner_read) != fs::perms::none) {
      file_buf.open(file_name);
      file_buf >> file_content;
      valid = !file_buf.fail() && !file_buf.bad();
    } else {
      valid = false;
    }
  }

  // 如果目标文件有效，则发送正常的HTTP应答
  if (valid) {
    content_buf << std::format("{} {}\r\n", "HTTP/1.1", status_line[0])
                << std::format("Content-Length: {}\r\n", file_content.size())
                << "\r\n"
                << file_content;
    auto content{content_buf.str()};
    auto len{content.size()};
    send(connfd, reinterpret_cast<const void*>(content.c_str()), len, 0);
  } else {
    content_buf << std::format("{} {}\r\n", "HTTP/1.1", status_line[1])
                << "\r\n";
    auto content{content_buf.str()};
    auto len{content.size()};
    send(connfd, reinterpret_cast<const void*>(content.c_str()), len, 0);
  }
  close(connfd);
  file_buf.close();
  content_buf.clear();
}

void webserver::run() {
  std::cout << "run" << std::endl;
  while (true) {
    epoller_->run(sock, std::bind(&webserver::accept_connection, this),
                  std::bind(&webserver::handle, this, std::placeholders::_1));
  }
}