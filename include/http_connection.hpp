#ifndef HTTP_CONNECTION_HPP
#define HTTP_CONNECTION_HPP

#include <optional>
#include <sys/socket.h>

#include "http_request.hpp"
#include "http_response.hpp"

static const int BUF_SIZE{65536};
class HttpConnection {
 private:
  std::optional<HttpRequest> request_;
  HttpResponse response_;
  char* buffer_;

 public:
  HttpConnection(/* args */);
  ~HttpConnection();
  void receive(int connfd);
};
#endif