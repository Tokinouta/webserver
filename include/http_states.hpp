#ifndef HTTP_STATES_HPP
#define HTTP_STATES_HPP

#include <string>

enum class HttpStatusCode {
  CONTINUE = 100,
  SWITCHING_PROTOCOLS = 101,
  OK = 200,
  CREATED = 201,
  ACCEPTED = 202,
  NON_AUTHORITATIVE_INFORMATION = 203,
  NO_CONTENT = 204,
  RESET_CONTENT = 205,
  PARTIAL_CONTENT = 206,
  MULTIPLE_CHOICES = 300,
  MOVED_PERMANENTLY = 301,
  FOUND = 302,
  SEE_OTHER = 303,
  NOT_MODIFIED = 304,
  USE_PROXY = 305,
  UNUSED = 306,
  TEMPORARY_REDIRECT = 307,
  PERMANENT_REDIRECT = 308,
  BAD_REQUEST = 400,
  UNAUTHORIZED = 401,
  PAYMENT_REQUIRED = 402,
  FORBIDDEN = 403,
  NOT_FOUND = 404,
  METHOD_NOT_ALLOWED = 405,
  NOT_ACCEPTABLE = 406,
  PROXY_AUTHENTICATION_REQUIRED = 407,
  REQUEST_TIME_OUT = 408,
  CONFLICT = 409,
  GONE = 410,
  LENGTH_REQUIRED = 411,
  PRECONDITION_FAILED = 412,
  PAYLOAD_TOO_LARGE = 413,
  URI_TOO_LARGE = 414,
  UNSUPPORTED_MEDIA_TYPE = 415,
  RANGE_NOT_SATISFIABLE = 416,
  EXPECTATION_FAILED = 417,
  INTERNAL_SERVER_ERROR = 500,
  NOT_IMPLEMENTED = 501,
  BAD_GATEWAY = 502,
  SERVICE_UNAVAILABLE = 503,
  GATEWAY_TIME_OUT = 504,
  HTTP_VERSION_NOT_SUPPORTED = 505,
};

enum class HttpMethod {
  GET,
  POST,
  HEAD,
  OPTIONS,
  PUT,
  PATCH,
  DELETE,
  TRACE,
  CONNECT
};

inline std::string status_string(HttpStatusCode sc) {
  switch (sc) {
    case HttpStatusCode::CONTINUE:
      return std::string("100 Continue");
    case HttpStatusCode::SWITCHING_PROTOCOLS:
      return std::string("101 Switching Protocols");
    case HttpStatusCode::OK:
      return std::string("200 OK");
    case HttpStatusCode::CREATED:
      return std::string("201 Created");
    case HttpStatusCode::ACCEPTED:
      return std::string("202 Accepted");
    case HttpStatusCode::NON_AUTHORITATIVE_INFORMATION:
      return std::string("203 Non-Authoritative Information");
    case HttpStatusCode::NO_CONTENT:
      return std::string("204 No Content");
    case HttpStatusCode::RESET_CONTENT:
      return std::string("205 Reset Content");
    case HttpStatusCode::PARTIAL_CONTENT:
      return std::string("206 Partial Content");
    case HttpStatusCode::MULTIPLE_CHOICES:
      return std::string("300 Multiple Choices");
    case HttpStatusCode::MOVED_PERMANENTLY:
      return std::string("301 Moved Permanently");
    case HttpStatusCode::FOUND:
      return std::string("302 Found");
    case HttpStatusCode::SEE_OTHER:
      return std::string("303 See Other");
    case HttpStatusCode::NOT_MODIFIED:
      return std::string("304 Not Modified");
    case HttpStatusCode::USE_PROXY:
      return std::string("305 Use Proxy");
    case HttpStatusCode::UNUSED:
      return std::string("306 Unused");
    case HttpStatusCode::TEMPORARY_REDIRECT:
      return std::string("307 Temporary Redirect");
    case HttpStatusCode::PERMANENT_REDIRECT:
      return std::string("308 Permanent Redirect");
    case HttpStatusCode::BAD_REQUEST:
      return std::string("400 Bad Request");
    case HttpStatusCode::UNAUTHORIZED:
      return std::string("401 Unauthorized");
    case HttpStatusCode::PAYMENT_REQUIRED:
      return std::string("402 Payment Required");
    case HttpStatusCode::FORBIDDEN:
      return std::string("403 Forbidden");
    case HttpStatusCode::NOT_FOUND:
      return std::string("404 Not Found");
    case HttpStatusCode::METHOD_NOT_ALLOWED:
      return std::string("405 Method Not Allowed");
    case HttpStatusCode::NOT_ACCEPTABLE:
      return std::string("406 Not Acceptable");
    case HttpStatusCode::PROXY_AUTHENTICATION_REQUIRED:
      return std::string("407 Proxy Authentication Required");
    case HttpStatusCode::REQUEST_TIME_OUT:
      return std::string("408 Request Timeout");
    case HttpStatusCode::CONFLICT:
      return std::string("409 Conflict");
    case HttpStatusCode::GONE:
      return std::string("410 Gone");
    case HttpStatusCode::LENGTH_REQUIRED:
      return std::string("411 Length Required");
    case HttpStatusCode::PRECONDITION_FAILED:
      return std::string("412 Precondition Failed");
    case HttpStatusCode::PAYLOAD_TOO_LARGE:
      return std::string("413 Payload Too Large");
    case HttpStatusCode::URI_TOO_LARGE:
      return std::string("414 URI Too Long");
    case HttpStatusCode::UNSUPPORTED_MEDIA_TYPE:
      return std::string("415 Unsupported Media Type");
    case HttpStatusCode::RANGE_NOT_SATISFIABLE:
      return std::string("416 Range Not Satisfiable");
    case HttpStatusCode::EXPECTATION_FAILED:
      return std::string("417 Expectation Failed");
    case HttpStatusCode::INTERNAL_SERVER_ERROR:
      return std::string("500 Internal Server Error");
    case HttpStatusCode::NOT_IMPLEMENTED:
      return std::string("501 Not Implemented");
    case HttpStatusCode::BAD_GATEWAY:
      return std::string("502 Bad Gateway");
    case HttpStatusCode::SERVICE_UNAVAILABLE:
      return std::string("503 Service Unavailable");
    case HttpStatusCode::GATEWAY_TIME_OUT:
      return std::string("504 Gateway Timeout");
    case HttpStatusCode::HTTP_VERSION_NOT_SUPPORTED:
      return std::string("505 HTTP Version Not Supported");
  }
}

#endif