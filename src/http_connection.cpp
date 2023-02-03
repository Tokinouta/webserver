#include "http_connection.hpp"

#include "http_states.hpp"

namespace fs = std::filesystem;

HttpConnection::HttpConnection() : parser_() { buffer_ = new char[BUF_SIZE]; }

HttpConnection::~HttpConnection() { delete[] buffer_; }

void HttpConnection::receive(int connfd) {
  int bytes_read = 0;
  while (true) {
    // if (m_clt_read_idx >= BUF_SIZE) {
    //   log(LOG_ERR, __FILE__, __LINE__, "%s",
    //       "the client read buffer is full, let server write");
    //   return BUFFER_FULL;
    // }

    bytes_read = recv(connfd, buffer_, BUF_SIZE, 0);
    if (bytes_read == -1) {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        break;
      }
    }
    // } else if (bytes_read == 0) {
    //   return CLOSED;
    // }

    // m_clt_read_idx += bytes_read;
    // }
    // return ((m_clt_read_idx - m_clt_write_idx) > 0) ? OK : NOTHING;
  }
}

void HttpConnection::parse(const char* buffer) {
  request_ = parser_.parse(string(buffer));
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
  response_.add_body(std::move(file_content));

  // 如果目标文件有效，则发送正常的HTTP应答
  // auto out_buffer{response_.write()};
  // out_buffer << (request_.has_value() ? "HTTP/1.1" : "HTTP/1.1");
  // response_.generate_response(request_.has_value() ? "HTTP/1.1" :
  // "HTTP/1.1");
  if (valid) {
    response_.set_status(HttpStatusCode::OK);
    response_.add_header(string("Content-Length"),
                         std::to_string(file_content.size()));
    // out_buffer << std::format("{} {}\r\n", "HTTP/1.1", status_line[0])
    //            << std::format("Content-Length: {}\r\n", file_content.size())
    //            << "\r\n"
    //            << file_content;
    // auto content{content_buf.str()};
    // auto len{content.size()};
    // send(connfd, reinterpret_cast<const void*>(content.c_str()), len, 0);
  } else {
    response_.set_status(HttpStatusCode::NOT_FOUND);
    // out_buffer << std::format("{} {}\r\n", "HTTP/1.1", status_line[1])
    //            << "\r\n";
    // auto content{content_buf.str()};
    // auto len{content.size()};
    // send(connfd, reinterpret_cast<const void*>(content.c_str()), len, 0);
  }

  // close(connfd);
  // content_buf.clear();
}