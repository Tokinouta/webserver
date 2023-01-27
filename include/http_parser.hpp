#ifndef HTTP_PARSER_HPP
#define HTTP_PARSER_HPP

#include <http_states.hpp>
#include <iostream>
#include <sstream>
#include <string>

using std::string;

class http_parser {
 private:
  HttpParseStatus status_;
  HttpMethod method_;

 public:
  http_parser();
  ~http_parser();

  void parse(string s);
  void parse_request();
  void parse_header();
  void parse_body();
};

#endif