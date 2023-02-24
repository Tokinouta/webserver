#ifndef LOG_HPP
#define LOG_HPP

#include <pthread.h>

#include <chrono>
#include <condition_variable>
#include <format>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <mutex>
#include <ostream>
#include <queue>
#include <sstream>
#include <string>
#include <thread>
#include <utility>

enum class Level { ERROR, WARNING, INFO, DEBUG, TRACE };
using namespace std::chrono_literals;

class Logger {
  //  private:
  //   std::unique_ptr<Logger> instance_;

 private:
  Logger()
      : fs_(std::ofstream("./log")), log_thread_(std::thread([this]() {
          std::cout << this->q_.size() << std::endl;
          while (true) {
            std::this_thread::sleep_for(200ms);
            this->output();
          }
        })) {
          std::cout << "hi, I am logger" << std::endl;
        }
  Logger(const Logger &) = delete;
  Logger(const Logger &&) = delete;
  Logger &operator=(const Logger &) = delete;

  std::queue<std::pair<Level, std::string>> q_;
  std::ofstream fs_;
  std::thread log_thread_;
  std::mutex mutex_;
  Level log_level_{Level::INFO};

  void output() {
    // std::cout << fs_.is_open() << std::endl;
    // 这里应该需要一个单独的线程来循环输出日志内容

    long size{0};

    {
      std::lock_guard<std::mutex> lc(mutex_);
      size = q_.size();
      for (auto i{0}; i < size; i++) {
        auto [level, content]{q_.front()};
        q_.pop();
        if (level <= log_level_) {
        std::cout << content << '\n';
        fs_ << content << '\n';
        }
      }
    }

    if (size > 0) {
      std::cout << std::endl;
      fs_.flush();
    }
  }

  void log(Level level, std::string &&content) {
    // auto time{std::chrono::system_clock::now()};
    // std::chrono::hh_mm_ss h{std::chrono::floor<std::chrono::seconds>(time)};
    // std::chrono::year_month_day
    // y{std::chrono::floor<std::chrono::days>(time)};
    std::ostringstream os;
    auto now{std::chrono::system_clock::now()};
    auto duration = now.time_since_epoch();
    auto micros =
        std::chrono::duration_cast<std::chrono::microseconds>(duration)
            .count() %
        1000000;
    auto time{std::chrono::system_clock::to_time_t(now)};
    os << std::put_time(std::localtime(&time), "%F %T")
       << std::format(".{}: {}", micros, content);
    {
      std::lock_guard<std::mutex> lc(mutex_);
      q_.emplace(
          std::make_pair<Level, std::string>(std::move(level), os.str()));
    }
  }

 public:
  ~Logger() {
    fs_.close();
    pthread_cancel(log_thread_.native_handle());
    log_thread_.join();
  }

  static Logger &get() {
    static Logger s;
    return s;
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