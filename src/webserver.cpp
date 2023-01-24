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

webserver::webserver(const char* ip, int port) {
  memset(&address, 0, sizeof(address));
  address.sin_family = AF_INET;
  inet_pton(AF_INET, ip, &address.sin_addr);
  address.sin_port = htons(port);
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

  struct sockaddr_in client;
  socklen_t client_addrlength = sizeof(client);
  int connfd = accept(sock, (struct sockaddr*)&client, &client_addrlength);
  if (connfd < 0) {
    std::cerr << std::format("errno is {}", errno) << std::endl;
  } else {
    handle(connfd);
  }
}

void webserver::handle(int connfd) {
  // 用于保存HTTP应答的状态行、头部字段和一个空行的缓存区
  // char header_buf[BUFFER_SIZE];
  // memset(header_buf, 0, BUFFER_SIZE);
  std::ifstream file_buf;
  string file_content;

  string file_name{fs::current_path().append("test.txt")};
  // char* file_buf;  // 用于存放目标文件内容的应用程序缓存

  // struct stat file_stat;  // 用于获取目标文件的属性，
  // 比如是否为目录，文件大小
  // auto len{0};  // 缓存区header_buf目前巳经使用了多少字节的空间

  // if (stat(file_name, &file_stat) < 0) {  // 目标文件不存在
  //   valid = false;
  // } else {
  //   if (S_ISDIR(file_stat.st_mode)) {  // 目标文件是一个目录
  //     valid = false;
  //   } else if (file_stat.st_mode & S_IROTH) {  //
  //   当前用户有读取目标文件的权限
  //     int fd{open(file_name, O_RDONLY)};
  //     file_buf = new char[file_stat.st_size + 1];
  //     memset(file_buf, 0, file_stat.st_size + 1);
  //     if (read(fd, file_buf, file_stat.st_size) < 0) {
  //       valid = false;
  //     }
  //   } else {
  //     valid = false;
  //   }
  // }

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
    // struct iovec iv[2];
    // iv[0].iov_base =
    // reinterpret_cast<void*>(const_cast<char*>(header.c_str())); iv[0].iov_len
    // = header.size(); iv[1].iov_base =
    //     reinterpret_cast<void*>(const_cast<char*>(content.c_str()));
    // iv[1].iov_len = content.size();
    // writev(connfd, iv, 2);
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