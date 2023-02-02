#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include <string>

#include "http_states.hpp"

using std::string;

class HttpResponse {
 private:
  HttpStatusCode status_code_;

 public:
  HttpResponse(/* args */) {}
  ~HttpResponse() {}
  string generate_response(const string& http_version) {
    return http_version + status_string(status_code_);
  }
};

// HttpResponse::HttpResponse(/* args */) {}

// HttpResponse::~HttpResponse() {}

// string HttpResponse::generate_response(const string& http_version) {
//   return http_version + status_string(status_code_);
// }

#endif