#include "request_handlers.hpp"

#include <format>
#include <sstream>

#include "http_response.hpp"
namespace fs = std::filesystem;

HttpResponse h(const std::string& url) {
  auto& log{Logger::get()};
  std::ifstream file_buf;
  std::stringstream file_content;
  HttpResponse response_;

  string file_name{fs::current_path().append("test.txt")};

  std::cout << file_name << std::endl;
  auto valid{true};  // 记录目标文件是否是有效文件
  if (!fs::exists(file_name)) {
    log.error("File not exists.");
    valid = false;
  } else if (fs::is_directory(file_name)) {
    log.error("File is a directory.");
    valid = false;
  } else {
    auto perms{fs::status(file_name).permissions()};
    if ((perms & fs::perms::owner_read) != fs::perms::none) {
      file_buf.open(file_name);
      string a;
      while (!file_buf.eof()) {
        std::getline(file_buf, a);
        std::cout << a << std::endl;
        file_content << a << '\n';
      }
      // file_buf. >> file_content; // TODO 需要修改一下这里以读取整个文件
      valid = !file_buf.fail() && !file_buf.bad();
      log.info(std::format("File read. Content: {}", file_content.str()));
    } else {
      log.error("Can't read file.");
      valid = false;
    }
  }
  file_buf.close();
  auto content_length{file_content.str().size()};
  response_.add_body(file_content.str());

  // 如果目标文件有效，则发送正常的HTTP应答
  if (valid) {
    response_.set_status(HttpStatusCode::OK);
    response_.add_header(string("Content-Length"),
                         std::to_string(content_length));
  } else {
    response_.set_status(HttpStatusCode::NOT_FOUND);
  }
  return response_;
}