#include "http_parser.hpp"

static const std::regex request_regex(
    "(GET|POST|HEAD|OPTIONS|PUT|PATCH|DELETE|TRACE|CONNECT) (.*) "
    "HTTP/(?:[\\d\\.]*)\\r");
static const std::regex header_regex("([A-Za-z\\-]): (.*)\\r");

HttpParser::HttpParser() : status_(HttpParseStatus::START) {}

HttpParser::~HttpParser() {}

std::optional<HttpRequest> HttpParser::parse(const string& s) {
  std::istringstream ss(s);
  HttpMethod method;
  string path;
  std::unordered_map<string, string> headers;
  string body;

  // note: the following loop terminates when std::ios_base::operator bool()
  // on the stream returned from getline() returns false
  for (;;) {
    auto pos{ss.tellg()};
    string a;
    switch (status_) {
      case HttpParseStatus::START: {
        std::getline(ss, a);
        parse_request(a, method, path);
        break;
      }

      case HttpParseStatus::HEADER: {
        std::getline(ss, a);
        parse_header(a, headers);
        // if (a == "\r") {
        //   // eof只有尝试读了才会变为true，所以不能用
        //   status_ = ss.tellg() == s.size() ? HttpParseStatus::END
        //                                    : HttpParseStatus::BODY;
        // } else {
        //   parse_header(a);
        // }
        break;
      }

      case HttpParseStatus::BODY: {
        if (pos == s.size()) {
          status_ = HttpParseStatus::END;
        } else {
          string a{s.substr(pos, s.size() - pos)};
          parse_body(a, body);
        }
        break;
      }

      case HttpParseStatus::END: {
        auto request{HttpRequest(method, std::move(path), std::move(headers),
                                 std::move(body))};
        return std::make_optional<HttpRequest>(request);
        break;
      }

      case HttpParseStatus::FAILED: {
        return std::nullopt;
        break;
      }
    }
  }
}

void HttpParser::parse_request(string& s, HttpMethod& method_, string& path_) {
  std::smatch request_match;
  if (std::regex_search(s, request_match, request_regex)) {
    auto method{request_match[1].str()};
    if (method == "GET") {
      method_ = HttpMethod::GET;
    } else if (method == "POST") {
      method_ = HttpMethod::POST;
    } else if (method == "HEAD") {
      method_ = HttpMethod::HEAD;
    } else if (method == "OPTIONS") {
      method_ = HttpMethod::OPTIONS;
    } else if (method == "PUT") {
      method_ = HttpMethod::PUT;
    } else if (method == "PATCH") {
      method_ = HttpMethod::PATCH;
    } else if (method == "DELETE") {
      method_ = HttpMethod::DELETE;
    } else if (method == "TRACE") {
      method_ = HttpMethod::TRACE;
    } else if (method == "CONNECT") {
      method_ = HttpMethod::CONNECT;
    }
    path_ = request_match[2];
    status_ = HttpParseStatus::HEADER;
  } else {
    status_ = HttpParseStatus::FAILED;
  }
}

void HttpParser::parse_header(string& s,
                               std::unordered_map<string, string>& headers) {
  if (s == "\r") {
    status_ = HttpParseStatus::BODY;
  } else {
    std::smatch header_match;
    if (std::regex_search(s, header_match, header_regex)) {
      auto header{header_match[1].str()};
      auto value{header_match[2].str()};
      headers.insert({header, value});
    } else {
      status_ = HttpParseStatus::FAILED;
    }
  }
}

void HttpParser::parse_body(string& s, string& body) {
  body = s;
  if (true) {
    status_ = HttpParseStatus::END;
  } else {
    status_ = HttpParseStatus::FAILED;
  }
}
