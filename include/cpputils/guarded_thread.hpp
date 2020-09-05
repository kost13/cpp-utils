// cpputils
// author: Lukasz Kostrzewa

#ifndef CPPUTILS_GUARDED_THREAD_H
#define CPPUTILS_GUARDED_THREAD_H

#include <thread>

namespace cpputils {
///
/// \brief The guarded_thread calls join in destructor
/// Based on The C++ Programming Language, Bjarne Stroustrup, 42.2.4
///
struct guarded_thread : std::thread {
  using std::thread::thread;
  using std::thread::operator=;
  ~guarded_thread() {
    if (joinable()) {
      join();
    }
  }
};

}  // namespace cpputils

#endif  // CPPUTILS_GUARDED_THREAD_H
