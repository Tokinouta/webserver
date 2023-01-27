#include "http_parser.hpp"

http_parser::http_parser() : status_(HttpParseStatus::START) {}

http_parser::~http_parser() {}

void http_parser::parse(const string s) {
  std::istringstream ss(s);
  // note: the following loop terminates when std::ios_base::operator bool()
  // on the stream returned from getline() returns false
  for (std::array<char, 4> a; ss.getline(&a[0], 4, '|');) {
    switch (status_) {
      case HttpParseStatus::START:
        parse_request();
        break;

      case HttpParseStatus::HEADER:
        parse_header();
        break;

      case HttpParseStatus::BODY:
        parse_body();
        break;

      default:
        break;
    }
  }
}

void http_parser::parse_request() {
  if (true) {
    status_ = HttpParseStatus::HEADER;
  }
}

void http_parser::parse_header() {
  if (true) {
    status_ = HttpParseStatus::BODY;
  }
}

void http_parser::parse_body() {
  if (true) {
    status_ = HttpParseStatus::END;
  } else {
    status_ = HttpParseStatus::FAILED;
  }
}
