#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "handler.hpp"
#include "http_response.hpp"

const std::string response_content{
    "<!DOCTYPE html>\n"
    "<html lang=\"en\">\n"
    "<head>\n"
    "    <meta charset=\"UTF-8\">\n"
    "    <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n"
    "    <meta name=\"viewport\" content=\"width=device-width, "
    "initial-scale=1.0\">\n"
    "    <title>Document</title>\n"
    "</head>\n"
    "<body>\n"
    "    \n"
    "</body>\n"
    "</html>\n"};
const std::string file_name("./response.html");

void prepare_response_file() {
  std::fstream fs(file_name);
  if (!fs.is_open()) {
    fs.clear();
    fs.open(file_name, std::ios::out);  // create file
    fs.close();
    fs.open(file_name);
  }
  fs << response_content;
  fs.close();
}

int main() {
  prepare_response_file();
  auto handler{Handler()};
  auto someFunction = [](const std::string& url) -> HttpResponse {
    std::ifstream file_buf;
    std::stringstream file_content;
    HttpResponse response_;

    std::cout << file_name << std::endl;

    file_buf.open(file_name);
    string a;
    while (!file_buf.eof()) {
      std::getline(file_buf, a);
      std::cout << a << std::endl;
      file_content << a << '\n';
    }
    // file_buf. >> file_content; // TODO 需要修改一下这里以读取整个文件
    file_buf.close();
    auto content_length{file_content.str().size()};
    response_.add_body(file_content.str());

    response_.set_status(HttpStatusCode::OK);
    response_.add_header(string("Content-Length"),
                         std::to_string(content_length));
    return response_;
  };

  auto url{string("/wap/setting/info")};
  handler.register_route(std::move(url), someFunction);

  HttpConnection conn;
  string request =
      "POST /wap/setting/info HTTP/1.1\r\n"
      "Accept: application/json, text/plain, */*\r\n"
      "Accept-Encoding: gzip, deflate, br\r\n"
      "Accept-Language: "
      "zh-CN,zh;q=0.9,en;q=0.8,en-GB;q=0.7,en-US;q=0.6,ja;q=0.5,zh-TW;q=0.4\r\n"
      "Cache-Control: no-cache\r\n"
      "Connection: keep-alive\r\n"
      "Content-Length: 0\r\n"
      "Content-Type: application/x-www-form-urlencoded\r\n"
      "Host: app.buaa.edu.cn\r\n"
      "Origin: https://app.buaa.edu.cn\r\n"
      "Pragma: no-cache\r\n"
      "Referer: https://app.buaa.edu.cn/site/buaaStudentNcov/index\r\n"
      "Sec-Fetch-Dest: empty\r\n"
      "Sec-Fetch-Mode: cors\r\n"
      "Sec-Fetch-Site: same-origin\r\n"
      "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) "
      "AppleWebKit/537.36 (KHTML, like Gecko) Chrome/100.0.4896.60 "
      "Safari/537.36 Edg/100.0.1185.29\r\n"
      "X-Requested-With: XMLHttpRequest\r\n"
      "sec-ch-ua: \" Not A;Brand\";v=\"99\", \"Chromium\";v=\"100\", "
      "\"Microsoft Edge\";v=\"100\"\r\n"
      "sec-ch-ua-mobile: ?0\r\n"
      "sec-ch-ua-platform: \"Windows\"\r\n"
      "\r\n"
      "<html>\n"
      "</html>";
  conn.receive_request(request.c_str());
  conn.parse();

  handler.call(conn);
  conn.prepare_response();

  std::filesystem::remove(file_name);

  auto response{conn.generate_response()};
  auto ground_truth{conn.response_header() + response_content + '\n'}; // 这个属于读文件的地方的bug，最后会多出来一个换行符，之后再修
  std::cout << response.size() << std::endl;
  std::cout << ground_truth.size() << std::endl;
  assert(response == ground_truth);

  return 0;
}