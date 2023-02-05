#include "webserver.hpp"

#include <sys/epoll.h>

#include <cstring>
#include <format>

#include "epoll_wrapper.hpp"
#include "http_connection.hpp"
#include "http_states.hpp"

// TODO:
// 增加connect处的错误处理，比如加一个错误码，或者手搓一个类似于rust那种result结构体

using std::string;

static const size_t BUFFER_SIZE{65536};

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
  epoller_->add(sock, true);  // 添加监听端口
  is_connected_ = true;
}

void webserver::accept_connection() {
  struct sockaddr_in client;
  socklen_t client_addrlength = sizeof(client);
  int connfd = accept(sock, (struct sockaddr*)&client, &client_addrlength);
  if (connfd < 0) {
    std::cerr << std::format("errno is {}", errno) << std::endl;
  } else {
    epoller_->add(connfd);  // 对connfd开启ET模式
    connection_.insert({connfd, HttpConnection()});
  }
}

void webserver::handle_read(int connfd) {
  // 获取fd对应的http connection
  auto& conn{connection_[connfd]};
  auto buffer_ = new char[BUFFER_SIZE];
  memset(buffer_, 0, BUFFER_SIZE);
  auto read_index{0};
  while (true) {
    // if (m_clt_read_idx >= BUF_SIZE) {
    //   log(LOG_ERR, __FILE__, __LINE__, "%s",
    //       "the client read buffer is full, let server write");
    //   return BUFFER_FULL;
    // }

    int bytes_read =
        recv(connfd, buffer_ + read_index, BUFFER_SIZE - read_index, 0);
    if (bytes_read == -1) {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        break;
      }
    } else if (bytes_read == 0) {
      // return CLOSED;
      break;
    }

    read_index += bytes_read;
    std::cout << std::format("read {} bytes", read_index) << std::endl;
    // }
    // return ((m_clt_read_idx - m_clt_write_idx) > 0) ? OK : NOTHING;
  }
  if (read_index == 0) {
    return;
  }
  conn.receive_request(buffer_);
  std::cout << "buffer: " << buffer_ << std::endl;
  delete[] buffer_;
  handle_request(connfd);
}

void webserver::handle_write(int connfd) {
  // 用于保存HTTP应答的状态行、头部字段和一个空行的缓存区
  // char header_buf[BUFFER_SIZE];
  // memset(header_buf, 0, BUFFER_SIZE);

  // 获取fd对应的http connection
  auto& conn{connection_[connfd]};
  auto response{conn.generate_response()};
  send(connfd, reinterpret_cast<const void*>(response.c_str()), response.size(),
       0);
  epoller_->mod(connfd, EPOLLIN);
}

void webserver::handle_close(int connfd) {
  // 获取fd对应的http connection
  auto conn{connection_[connfd]};

  // conn.receive(connfd);
}

void webserver::handle_request(int connfd) {
  std::cout << "start handling request" << std::endl;
  auto& conn{connection_[connfd]};
  conn.parse();
  if (!conn.is_request_available()) {
    conn.prepare_error_response(HttpStatusCode::BAD_REQUEST);
  } else {
    conn.prepare_response();
  }
  // 通过设置EPOLLOUT手动触发写事件
  epoller_->mod(connfd, EPOLLOUT);
}

void webserver::run() {
  std::cout << "run" << std::endl;
  while (true) {
    epoller_->run(
        sock, std::bind(&webserver::accept_connection, this),
        std::bind(&webserver::handle_read, this, std::placeholders::_1),
        std::bind(&webserver::handle_write, this, std::placeholders::_1),
        std::bind(&webserver::handle_close, this, std::placeholders::_1));
  }
}