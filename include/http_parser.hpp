#ifndef HTTP_PARSER_HPP
#define HTTP_PARSER_HPP

#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>

#include "http_states.hpp"

using std::string;

class http_parser {
 private:
  HttpParseStatus status_;
  HttpMethod method_;
  string path_;
  std::unordered_map<string, string> headers_;

 public:
  http_parser();
  ~http_parser();

  void parse(string s);
  void parse_request(string s);
  void parse_header(string s);
  void parse_body(string s);
};

#endif