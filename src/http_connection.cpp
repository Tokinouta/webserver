#include "http_connection.hpp"

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

void HttpConnection::parse() { request_ = parser_.parse(string(buffer_)); }
