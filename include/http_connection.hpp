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
#include "http_states.hpp"

static const int BUF_SIZE{65536};
class HttpConnection {
 private:
  std::optional<HttpRequest> request_;
  HttpResponse response_;
  HttpParser parser_;
  string request_buffer_;
  string response_buffer_;

 public:
  HttpConnection();
  ~HttpConnection();
  void receive_request(const char* request) {
    request_buffer_ = string(request);
  }
  void parse();
  bool is_request_available() { return request_.has_value(); }

  void prepare_response();
  void prepare_error_response(HttpStatusCode status) {
    response_.set_status(status);
    response_buffer_ = response_.generate_response();
  }
  std::string generate_response() { return response_buffer_; }

  const std::string& path() const { return request_->path(); }
  HttpResponse& response() { return response_; }
};
#endif