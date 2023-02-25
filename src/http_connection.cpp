#include "http_connection.hpp"

#include "http_states.hpp"
#include "log.hpp"


HttpConnection::HttpConnection() : parser_() {}

HttpConnection::~HttpConnection() {}

void HttpConnection::parse() { request_ = parser_.parse(request_buffer_); }

void HttpConnection::prepare_response() {
  auto& log{Logger::get()};
  
  response_buffer_ = response_.generate_response();
  log.info("Response generated");
}