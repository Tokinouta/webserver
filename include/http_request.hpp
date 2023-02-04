#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include <format>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>

#include "http_states.hpp"

using std::string;

class HttpRequest {
 private:
  HttpMethod method_;
  string path_;
  std::unordered_map<string, string> headers_;
  string body_;

 public:
  HttpRequest(HttpMethod method, string&& path,
              std::unordered_map<string, string>&& headers, string&& body)
      : method_(method), path_(path), headers_(headers), body_(body) {}
  ~HttpRequest() {}
  string generate_response(const string& http_version = "HTTP/1.1") {
    auto buffer{std::ostringstream()};
    buffer << std::format("{} {} {}\r\n", method_, path_, http_version);
    // 其实这里输出的时候按字段排序并不必要，但是测试数据是按顺序排的，这里也排一下便于测试
    std::map<std::string, std::string> headers(headers_.begin(), headers_.end());
    for (auto [h, c] : headers) {
      buffer << std::format("{}: {}\r\n", h, c);
    }
    buffer << "\r\n";
    buffer << body_;
    return buffer.str();
  }
};

// inline HttpRequest::HttpRequest(HttpMethod method, string&& path,
//                          std::unordered_map<string, string>&& headers,
//                          string&& body)

// HttpRequest::~HttpRequest()

#endif