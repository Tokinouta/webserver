#include "thread_pool.hpp"

#include <cassert>
#include <chrono>
#include <format>
#include <iostream>
#include <random>
#include <thread>

std::random_device rd;  // 真实随机数产生器
std::mt19937 mt(rd());  // 生成计算随机数mt
std::uniform_int_distribution<int> dist(-1000,
                                        1000);  // 生成-1000到1000间均匀分布
auto rnd{std::bind(dist, mt)};

// 设置线程睡眠时间
void simulate_hard_computation() {
  std::this_thread::sleep_for(std::chrono::milliseconds(2000 + rnd()));
}

void multiply(const int a, const int b) {
  simulate_hard_computation();
  const auto res{a * b};
  std::cout << std::format("{} * {} = {}", a, b, res) << std::endl;
}

void multiply_output(int &out, const int a, const int b) {
  simulate_hard_computation();
  out = a * b;
  std::cout << std::format("{} * {} = {}", a, b, out) << std::endl;
}

int multiply_return(const int a, const int b) {
  simulate_hard_computation();
  const auto res{a * b};
  std::cout << std::format("{} * {} = {}", a, b, res) << std::endl;
  return res;
}

int main() {
  // 创建3个线程的线程池并初始化
  ThreadPool pool(3);
  pool.init();

  // 提交乘法操作，总共30个
  for (int i = 1; i <= 3; ++i) {
    for (int j = 1; j <= 10; ++j) {
      pool.submit(multiply, i, j);
    }
  }
  // 使用ref传递的输出参数提交函数
  int output_ref;
  auto future1 = pool.submit(multiply_output, std::ref(output_ref), 5, 6);
  future1.get();  // 等待乘法输出完成
  assert(output_ref == 30);

  // 使用return参数提交函数
  auto future2 = pool.submit(multiply_return, 5, 3);
  int res = future2.get();  // 等待乘法输出完成
  assert(res == 15);

  // 关闭线程池
  pool.shutdown();
  return 0;
}