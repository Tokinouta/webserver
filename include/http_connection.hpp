#ifndef HTTP_CONNECTION_HPP
#define HTTP_CONNECTION_HPP

#include <sys/socket.h>

#include <filesystem>
#include <format>
#include <fstream>
#include <optional>
#include <string>

#include "http_parser.hpp"
#include "http_request.hpp"
#include "http_response.hpp"

static const int BUF_SIZE{65536};
class HttpConnection {
 private:
  std::optional<HttpRequest> request_;
  HttpResponse response_;
  HttpParser parser_;
  char* buffer_;

 public:
  HttpConnection();
  ~HttpConnection();
  void receive(int connfd);
  void parse(const char* buffer);

  bool is_request_available() { return request_.has_value(); }
  void prepare_response();
  std::string get_response_string() {return response_.content();}
};
#endif