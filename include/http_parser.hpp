#ifndef HTTP_PARSER_HPP
#define HTTP_PARSER_HPP

#include <iostream>
#include <optional>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>

#include "http_request.hpp"
#include "http_states.hpp"

using std::string;

enum class HttpParseStatus { START, HEADER, BODY, END, FAILED };

class http_parser {
 private:
  HttpParseStatus status_;

 public:
  http_parser();
  ~http_parser();

  std::optional<HttpRequest> parse(string s);
  void parse_request(string s, HttpMethod& method, string& path);
  void parse_header(string s, std::unordered_map<string, string>& headers);
  void parse_body(string s, string& body);
};

#endif