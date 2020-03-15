#ifndef CPPUTILS_LOGGER_H
#define CPPUTILS_LOGGER_H

#include <fstream>
#include <iostream>
#include <mutex>
#include <sstream>
#include <vector>

namespace cpputils {

///
/// \brief The Logger class
/// Thread-safe logging tool

class Logger {
 public:
  enum Config { SHOW_PREFIX = 0x01, SHOW_DATETIME = 0x02, SHOW_DEBUG = 0x04 };
  class LoggerStream;

  void configure(Config c);
  void configure(int c);

  void exportToFile(const char *fname);

  LoggerStream info();
  LoggerStream warning();
  LoggerStream debug();
  LoggerStream critical();

 private:
  enum class LogType { INFO, DEBUG, WARNING, CRITICAL };
  void flush(const std::string &txt, LogType t);
  void flush(const std::string &txt, LogType t, std::ostream &ios);
  const char *typeToString(LogType t);

  std::fstream fs_;
  std::mutex mutex_;

  bool show_prefix_{true};
  bool show_datetime_{true};
  bool show_debug_{true};
  bool export_{false};

 public:
  class LoggerStream : public std::ostringstream {
   public:
    LoggerStream(Logger &logger, LogType t) : logger_(logger), t_(t) {}
    LoggerStream(const LoggerStream &o) : logger_(o.logger_), t_(o.t_) {}
    LoggerStream(LoggerStream &&) = default;
    ~LoggerStream() { logger_.flush(std::move(str()), t_); }

    template <typename T>
    LoggerStream &operator<<(const std::vector<T> &v) {
      std::string s = "[";
      if (!v.empty()) {
        for (const auto &i : v) {
          s += (std::to_string(i) + ", ");
        }
        s.pop_back();
        s.pop_back();
      }
      s += "]";
      *this << s;
      return *this;
    }

   private:
    Logger &logger_;
    LogType t_;
  };
};

/// namespace log gives access to static Logger
namespace log {

void exportToFile(const char *fname);
void configure(Logger::Config c);
void configure(int c);

Logger::LoggerStream info();
Logger::LoggerStream warning();
Logger::LoggerStream debug();
Logger::LoggerStream critical();

}  // log

}  // cpputils

#endif  // CPPUTILS_LOGGER_H
