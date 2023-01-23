#include <iostream>

#include "webserver.hpp"

int main(int argc, char** argv) {
  std::cout << "hello world!" << std::endl;
  auto a{webserver(10)};
  a.print();
}
