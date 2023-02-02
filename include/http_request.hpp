#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

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
};

// inline HttpRequest::HttpRequest(HttpMethod method, string&& path,
//                          std::unordered_map<string, string>&& headers,
//                          string&& body)

// HttpRequest::~HttpRequest()

#endif