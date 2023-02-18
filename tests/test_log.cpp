#include <unistd.h>

#include <string>
#include <thread>
#include <vector>

#include "log.hpp"

int main() {
  // 这里需要用一个引用类型，即使是auto也需要显式写出来这是一个引用。
  // https://stackoverflow.com/questions/54042714/call-to-deleted-constructor-of
  //   auto& log{Logger::get()};
  auto f1 = [] {
    auto& log{Logger::get()};
    log.debug(std::string("This is a debug message."));
  };
  auto f2 = [] {
    auto& log{Logger::get()};
    log.trace(std::string("This is a trace message."));
  };
  auto f3 = [] {
    auto& log{Logger::get()};
    log.info(std::string("This is a info message."));
  };
  auto f4 = [] {
    auto& log{Logger::get()};
    log.warning(std::string("This is a warning message."));
  };
  auto f5 = [] {
    auto& log{Logger::get()};
    log.error(std::string("This is a error message."));
  };
  std::vector<std::thread> threads(5);
  for (auto i{0}; i < 5; i++) {
    switch (i) {
      case 0: {
        threads[i] = std::thread(f1);
        break;
      }
      case 1: {
        threads[i] = std::thread(f2);
        break;
      }
      case 2: {
        threads[i] = std::thread(f3);
        break;
      }
      case 3: {
        threads[i] = std::thread(f4);
        break;
      }
      case 4: {
        threads[i] = std::thread(f5);
        break;
      }
    }
  }
  for (auto i{0}; i < 5; i++) {
    threads[i].join();
  }
  sleep(3);

  auto fs(std::ifstream("./log"));
  for (std::string a; std::getline(fs, a);) {
    std::cout << a << std::endl;
  }
  return 0;
}