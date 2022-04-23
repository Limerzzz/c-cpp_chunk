/*
 * @Author: Limer
 * @Date: 2022-04-19 22:01:01
 * @LastEditors: Limer
 * @LastEditTime: 2022-04-23 14:45:28
 * @Description:The program is a demo which implement a thread pool.
 * @link: https://skykeyjoker.com/2021/04/23/cpp11-thread-pool/
 */
#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>
#include <vector>

template <typename T>
class safequeue {
 private:
  std::queue<T> _queue;
  std::mutex _mutex;

 public:
  safequeue(){};
  safequeue(safequeue&& other){};
  ~safequeue(){};

  bool empty();
  int size();
  void enqueue(T& t);
  bool dequeue(T& t);
};

template <typename T>
bool safequeue<T>::empty() {
  std::unique_lock<std::mutex> _lock(_mutex);
  return _queue.empty();
}

template <typename T>
int safequeue<T>::size() {
  std::unique_lock<std::mutex> _lock(_mutex);
  return _queue.size();
}

template <typename T>
void safequeue<T>::enqueue(T& t) {
  std::unique_lock<std::mutex> _lock(_mutex);
  _queue.emplace(t);
}

template <typename T>
bool safequeue<T>::dequeue(T& t) {
  std::unique_lock<std::mutex> _lock(_mutex);
  if (_queue.empty()) return false;
  t = std::move(_queue.front());
  _queue.pop();
  return true;
}

class ThreadPool {
 private:
  class ThreadWorker {
   private:
    int _id;
    ThreadPool* _pool;

   public:
    ThreadWorker(ThreadPool* pool, const int id) : _pool(pool), _id(id) {}
    void operator()() {
      std::function<void()> func;
      bool dequeued;
      while (!_pool->m_shutdown) {
        {
          std::unique_lock<std::mutex> lock(_pool->_mtx);
          if (_pool->_queue.empty()) {
            _pool->_cv.wait(lock);
          }
          dequeued = _pool->_queue.dequeue(func);
        }
        if (dequeued) func();
      }
    }
  };
  bool m_shutdown;
  safequeue<std::function<void()>> _queue;
  std::vector<std::thread> _threads;
  std::mutex _mtx;
  std::condition_variable _cv;

 public:
  ThreadPool(const int n_threads = 4);
  ThreadPool(const ThreadPool&) = delete;
  ThreadPool(ThreadPool&&) = delete;
  ThreadPool& operator=(const ThreadPool&) = delete;
  ThreadPool& operator=(ThreadPool&&) = delete;
  void init();
  void shutdown();
  template <typename F, typename... Args>
  auto submit(F&& f, Args&&... args) -> std::future<decltype(f(args...))>;
};

ThreadPool::ThreadPool(const int n_threads)
    : _threads(std::vector<std::thread>(n_threads)), m_shutdown(false) {}

void ThreadPool::init() {
  for (int i = 0; i < _threads.size(); ++i) {
    _threads[i] = std::thread(ThreadWorker(this, i));
  }
}

void ThreadPool::shutdown() {
  m_shutdown = true;
  _cv.notify_all();
  for (int i = 0; i < _threads.size(); ++i) {
    if (_threads[i].joinable()) {
      _threads[i].join();
    }
  }
}

template <typename F, typename... Args>
auto ThreadPool::submit(F&& f, Args&&... args)
    -> std::future<decltype(f(args...))> {  // tail type direvation
  std::function<decltype(f(args...))()> func =
      std::bind(std::forward<F>(f), std::forward<Args>(args)...);

  auto task_ptr =
      std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);

  std::function<void()> warpper_func = [task_ptr]() { (*task_ptr)(); };

  _queue.enqueue(warpper_func);

  _cv.notify_one();

  return task_ptr->get_future();
}

#endif