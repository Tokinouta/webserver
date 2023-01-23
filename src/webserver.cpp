#include "webserver.hpp"

#include <arpa/inet.h>
#include <assert.h>
#include <sys/socket.h>
#include <unistd.h>

#include <format>
#include <iostream>

webserver::webserver(const char* ip, int port) {
  memset(&address, 0, sizeof(address));
  address.sin_family = AF_INET;
  inet_pton(AF_INET, ip, &address.sin_addr);
  address.sin_port = htons(port);

  sock = socket(PF_INET, SOCK_STREAM, 0);
  if (bind(sock, (struct sockaddr*)&address, sizeof(address)) == -1) {
    std::cerr << "error binding" << std::endl;
  }
  if (listen(sock, 5) == -1) {
    std::cerr << "error listening" << std::endl;
  }
}

webserver::~webserver() { close(sock); }

void webserver::print() {
  std::cout << std::format("{}", sock) << std::endl;
}

void webserver::connect() {
  struct sockaddr_in client;
  socklen_t client_addrlength = sizeof(client);
  int connfd = accept(sock, (struct sockaddr*)&client, &client_addrlength);
  if (connfd < 0) {
    std::cout << "errno is " << errno << "\n";
  } else {
    handle();
  }
}

void webserver::handle() {
//   // 用于保存HTTP应答的状态行、头部字段和一个空行的缓存区
//   char header_buf[BUFFER_SIZE];
//   memset(header_buf, 0, BUFFER_SIZE);

//   char* file_buf;  // 用于存放目标文件内容的应用程序缓存
//   struct stat file_stat;  // 用于获取目标文件的属性， 比如是否为目录，文件大小
//   bool valid{true};  // 记录目标文件是否是有效文件
//   int len{0};  // 缓存区header_buf目前巳经使用了多少字节的空间

//   if (stat(file_name, &file_stat) < 0) {  // 目标文件不存在
//     valid = false;
//   } else {
//     if (S_ISDIR(file_stat.st_mode)) {  // 目标文件是一个目录
//       valid = false;
//     } else if (file_stat.st_mode & S_IROTH) {  // 当前用户有读取目标文件的权限
//       int fd{open(file_name, O_RDONLY)};
//       file_buf = new char[file_stat.st_size + 1];
//       memset(file_buf, 0, file_stat.st_size + 1);
//       if (read(fd, file_buf, file_stat.st_size) < 0) {
//         valid = false;
//       }
//     } else {
//       valid = false;
//     }
//   }

//   // 如果目标文件有效，则发送正常的HTTP应答
//   if (valid) {
//     ret = snprintf(header_buf, BUFFER_SIZE - 1, "%s %s\r\n", "HTTP/1.1",
//                    status_line[0]);
//     len += ret;
//     ret = snprintf(header_buf + len, BUFFER_SIZE - 1 - len,
//                    "Content-Length: %d\r\n", file_stat.st_size);
//     len += ret;
//     ret = snprintf(header_buf + len, BUFFER_SIZE - 1 - len, "%s", "\r\n");

//     struct iovec iv[2];
//     iv[0].iov_base = header_buf;
//     iv[0].iov_len = strlen(header_buf);
//     iv[1].iov_base = file_buf;
//     iv[1].iov_len = file_stat.st_size;
//     ret = writev(connfd, iv, 2);
//   } else {
//     ret = snprintf(header_buf, BUFFER_SIZE - 1, "%s %s\r\n", "HTTP/1.1",
//                    status_line[1]);
//     len += ret;
//     ret = snprintf(header_buf + len, BUFFER_SIZE - 1 - len, "%s", "\r\n");
//     send(connfd, header_buf, strlen(header_buf), 0);
//   }
//   close(connfd);
//   delete[] file_buf;
}