#ifndef LOG_HPP
#define LOG_HPP

#include <iostream>
#include <memory>
#include <queue>
#include <string>
#include <utility>

enum class Level { ERROR, WARNING, INFO, DEBUG, TRACE };

class Logger {
  //  private:
  //   std::unique_ptr<Logger> instance_;

 private:
  Logger() {}
  Logger(const Logger &) = delete;
  Logger(const Logger &&) = delete;
  Logger &operator=(const Logger &) = delete;

  std::queue<std::pair<Level, std::string>> q;

 public:
  static Logger &get() {
    static Logger s;
    return s;
  }

 public:
  void output() {
    // 这里应该需要一个单独的线程来循环输出日志内容
    auto size{q.size()};
    for (auto i{0}; i < size; i++) {
      auto [level, content]{q.front()};
      q.pop();
      if (level == Level::ERROR) {
        std::cout << content << ' ';
      }
    }
    std::cout << std::endl;
  }
  void log(Level level, std::string &&content) {
    q.emplace(std::make_pair<Level, std::string>(std::move(level),
                                                 std::move(content)));
  }
  void error(std::string &&content) { log(Level::ERROR, std::move(content)); }
  void warning(std::string &&content) {
    log(Level::WARNING, std::move(content));
  }
  void info(std::string &&content) { log(Level::INFO, std::move(content)); }
  void debug(std::string &&content) { log(Level::DEBUG, std::move(content)); }
  void trace(std::string &&content) { log(Level::TRACE, std::move(content)); }
};

#endif