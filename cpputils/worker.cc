// cpputils
// author: Lukasz Kostrzewa

#include "worker.h"

#include <condition_variable>
#include <deque>
#include <mutex>
#include <thread>

struct cpputils::Worker::Opaque {
  Opaque() : is_running_{false} {}

  void startThread() {
    thread_ = std::thread([this] {
      while (true) {
        task_type task;
        {
          std::unique_lock<std::mutex> lock(mutex_);
          condition_.wait(lock,
                          [&] { return !task_queue_.empty() || !is_running_; });

          if (!is_running_) {
            return;
          }

          task = std::move(task_queue_.front());
          task_queue_.pop_front();
        }

        task();
      }
    });
  }

  using task_type = std::function<void()>;
  std::condition_variable condition_;
  std::deque<task_type> task_queue_;
  std::mutex mutex_;
  std::thread thread_;
  bool is_running_;
};

cpputils::Worker::Worker() : o_(std::make_unique<Opaque>()) {}

cpputils::Worker::~Worker() { stop(true); }

bool cpputils::Worker::start() {
  {
    std::lock_guard<std::mutex> lock(o_->mutex_);
    if (o_->is_running_) {
      return false;
    }
    o_->is_running_ = true;
  }

  o_->startThread();
  return true;
}

bool cpputils::Worker::stop(bool clear_queue) {
  {
    std::lock_guard<std::mutex> lock(o_->mutex_);
    if (!o_->is_running_) {
      return false;
    }
    o_->is_running_ = false;

    if (clear_queue) {
      o_->task_queue_.clear();
    }
  }

  o_->condition_.notify_all();
  o_->thread_.join();
  return true;
}

void cpputils::Worker::async(const std::function<void()>& f) {
  std::lock_guard<std::mutex> lock(o_->mutex_);
  o_->task_queue_.push_back(f);
  o_->condition_.notify_all();
}

void cpputils::Worker::priorityAsync(const std::function<void()>& f) {
  std::lock_guard<std::mutex> lock(o_->mutex_);
  o_->task_queue_.push_front(f);
  o_->condition_.notify_all();
}

bool cpputils::Worker::empty() {
  std::lock_guard<std::mutex> lock(o_->mutex_);
  return o_->task_queue_.empty();
}
