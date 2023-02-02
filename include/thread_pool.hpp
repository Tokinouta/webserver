#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>

// 这个类是对标准库里queue的包装。它对主要的操作都加了锁保护，即操作前必须先获得锁
// 其他的和标准的queue一样。
template <typename T>
class SafeQueue {
 private:
  std::queue<T> queue_;
  std::mutex mutex_;

 public:
  SafeQueue() {}
  ~SafeQueue() {}

  bool empty() {
    std::unique_lock<std::mutex> lock(mutex_);
    return queue_.empty();
  }

  int size() {
    std::unique_lock<std::mutex> lock(mutex_);
    return queue_.size();
  }

  void push(const T &task) {
    std::unique_lock<std::mutex> lock(mutex_);
    queue_.push(task);
  }

  bool pop(T &task) {
    std::unique_lock<std::mutex> lock(mutex_);
    if (queue_.empty()) {
      return false;
    }

    // 注意这个移动操作，它可以认为把这个变量的所有权转移到task变量中
    task = std::move(queue_.front());

    queue_.pop();
    return true;
  }
};

class ThreadPool {
 private:
  class Worker {
   private:
    int id_;
    ThreadPool *pool_;

   public:
    Worker(int id, ThreadPool *pool) : id_(id), pool_(pool) {}
    void operator()() {
      std::function<void()> fn;
      bool has_work{false};
      while (!pool_->is_shutdown_) {
        {
          // 此处只对获取新任务的过程加锁，不管最后有没有拿到工作都会解锁
          // 这里的大括号定义了一个作用域，锁只在这一个作用域里生效
          std::unique_lock<std::mutex> lock(pool_->mutex_);
          if (pool_->queue_.empty() && !pool_->is_shutdown_) {
            pool_->conditional_lock_.wait(lock);
          }
          has_work = pool_->queue_.pop(fn);
        }
        if (has_work) {
          fn();
        }
      }
    }
  };

  SafeQueue<std::function<void()>> queue_;  // 任务队列
  std::vector<std::thread> workers_;        // 线程队列
  bool is_shutdown_;

  std::mutex mutex_;
  std::condition_variable conditional_lock_;

 public:
  ThreadPool(const int n_threads)
      : workers_(std::vector<std::thread>(n_threads)), is_shutdown_(false) {}
  // 拷贝构造函数，并且取消默认父类构造函数
  ThreadPool(const ThreadPool &) = delete;
  // 拷贝构造函数，允许右值引用
  ThreadPool(ThreadPool &&) = delete;
  // 赋值操作
  ThreadPool &operator=(const ThreadPool &) = delete;
  // 赋值操作
  ThreadPool &operator=(ThreadPool &&) = delete;
  ~ThreadPool() {}

  void init() {
    for (auto i{0}; i < workers_.size(); i++) {
      workers_[i] = std::thread(Worker(i, this));
    }
  }

  void shutdown() {
    is_shutdown_ = true;
    conditional_lock_.notify_all();

    for (auto &w : workers_) {
      if (w.joinable()) {
        w.join();
      }
    }
  }

  template <typename F, typename... Args>
  auto submit(F &&f, Args &&...args) -> std::future<decltype(f(args...))> {
    // 连接函数和参数定义，特殊函数类型,避免左、右值错误
    std::function<decltype(f(args...))()> func{
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)};
    auto task{
        std::make_shared<std::packaged_task<decltype(f(args...))()>>(func)};
    auto wrapper_func = [task]() { (*task)(); };

    queue_.push(wrapper_func);
    conditional_lock_.notify_one();
    return task->get_future();
  }
};

#endif