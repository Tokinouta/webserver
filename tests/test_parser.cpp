#include <cassert>
#include <iostream>
#include <string>

#include "http_parser.hpp"

using std::string;

int main() {
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
  HttpParser parser;
  auto re{parser.parse(request)};
  assert(re.has_value());
//   std::cout << re.has_value() << std::endl;
  auto ra{re.value().generate_response()};
//   std::cout << ra;
  assert(ra == request);
}
