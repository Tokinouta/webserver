#ifndef HTTP_CONNECTION_HPP
#define HTTP_CONNECTION_HPP

#include <optional>

#include "http_request.hpp"
#include "http_response.hpp"

class HttpConnection {
 private:
  std::optional<HttpRequest> request_;
  HttpResponse response_;

 public:
  HttpConnection(/* args */);
  ~HttpConnection();
};

HttpConnection::HttpConnection(/* args */) {}

HttpConnection::~HttpConnection() {}

#endif