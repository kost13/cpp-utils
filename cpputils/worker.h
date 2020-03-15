#ifndef CPPUTILS_WORKER_H
#define CPPUTILS_WORKER_H

#include <functional>
#include <memory>

namespace cpputils {

///
/// \brief The Worker class executes tasks in worker thread
///
class Worker {
 public:
  /// construct opaque
  Worker();

  /// thread cannot be copied
  Worker(const Worker &) = delete;

  Worker(Worker &&) = default;

  /// call std::thread::join() on its thread
  ~Worker();

  /// start worker thread
  bool start();

  /// stop worker thread and clar tasks queue if clear_queue = true
  bool stop(bool clear_queue);

  /// add task in the end of the tasks queue
  void async(std::function<void()> f);

  /// add task in the front of the tasks queue
  void priorityAsync(std::function<void()> f);

 private:
  /// hide implementation with opaque pointer
  struct Opaque;
  std::unique_ptr<Opaque> o_;
};
}

#endif  // CPPUTILS_WORKER_H
