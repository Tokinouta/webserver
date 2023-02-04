#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include <format>
#include <sstream>
#include <string>
#include <unordered_map>

#include "http_states.hpp"

using std::string;

class HttpResponse {
 private:
  HttpStatusCode status_code_;
  std::unordered_map<string, string> headers_;
  string body_;

 public:
  HttpResponse() {}
  ~HttpResponse() {}
  string generate_response(const string& http_version = "HTTP/1.1") {
    std::ostringstream buffer;
    buffer << std::format("{} {}\r\n", http_version,
                          status_string(status_code_));
    for (auto [h, c] : headers_) {
      buffer << std::format("{}: {}\r\n", h, c);
    }
    buffer << "\r\n";
    buffer << body_;
    return buffer.str();
  }

  void set_status(const HttpStatusCode status) { status_code_ = status; }
  void add_header(string&& header, string&& content) {
    headers_.insert({header, content});
  }
  void add_body(string&& body) { body_ = std::move(body); }
};

#endif