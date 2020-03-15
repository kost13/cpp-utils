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

  /// constructs opaque
  Worker();

  /// destructor call std::thread::join() on its thread
  ~Worker();

  /// start worker thread
  bool start();

  /// stop worker thread and delete tasks queue
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
