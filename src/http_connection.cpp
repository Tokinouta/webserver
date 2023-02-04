#include "http_connection.hpp"

#include "http_states.hpp"

namespace fs = std::filesystem;

HttpConnection::HttpConnection() : parser_() {}

HttpConnection::~HttpConnection() {}

void HttpConnection::parse() {
  request_ = parser_.parse(request_buffer_);
}

void HttpConnection::prepare_response() {
  // std::stringstream content_buf;
  std::ifstream file_buf;
  string file_content;

  string file_name{fs::current_path().append("test.txt")};

  std::cout << file_name << std::endl;
  auto valid{true};  // 记录目标文件是否是有效文件
  if (!fs::exists(file_name)) {
    std::cerr << "File not exists." << std::endl;
    valid = false;
  } else if (fs::is_directory(file_name)) {
    std::cerr << "File is a directory." << std::endl;
    valid = false;
  } else {
    auto perms{fs::status(file_name).permissions()};
    if ((perms & fs::perms::owner_read) != fs::perms::none) {
      file_buf.open(file_name);
      file_buf >> file_content;
      valid = !file_buf.fail() && !file_buf.bad();
    } else {
      valid = false;
    }
  }
  file_buf.close();
  auto content_length{file_content.size()};
  response_.add_body(std::move(file_content));

  // 如果目标文件有效，则发送正常的HTTP应答
  if (valid) {
    response_.set_status(HttpStatusCode::OK);
    response_.add_header(string("Content-Length"),
                         std::to_string(content_length));
  } else {
    response_.set_status(HttpStatusCode::NOT_FOUND);
  }
  response_buffer_ = response_.generate_response();
}