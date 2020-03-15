#include "logger.hpp"

#include <cstring>
#include <ctime>
#include <iomanip>

namespace {

void datetime(std::ostream &stream) {
  auto t = std::time(nullptr);
  stream << std::put_time(std::localtime(&t), "%F %T: ");
}

// logger hidden from the user accessible via functions from log namespace
cpputils::Logger logger;
}

cpputils::Logger::~Logger() {
  if (fs_.is_open()) {
    fs_.close();
  }
}

void cpputils::Logger::configure(cpputils::Logger::Config c) {
  std::lock_guard<std::mutex> lock(mutex_);
  show_prefix_ = (c & SHOW_PREFIX) != 0;
  show_datetime_ = (c & SHOW_DATETIME) != 0;
  show_debug_ = (c & SHOW_DEBUG) != 0;
}

void cpputils::Logger::configure(int c) {
  configure(static_cast<cpputils::Logger::Config>(c));
}

void cpputils::Logger::exportToFile(const char *fname) {
  if (fs_.is_open()) {
    warning() << "File stream is already open. Closing it.";
  }

  fs_.close();
  fs_.open(fname, std::ios::out);
  if (!fs_.is_open()) {
    critical() << "Cannot open file" << fname << "to save log";
    export_ = false;
  } else {
    export_ = true;
  }
}

cpputils::Logger::LoggerStream cpputils::Logger::info() {
  return LoggerStream(*this, LogType::INFO);
}

cpputils::Logger::LoggerStream cpputils::Logger::warning() {
  return LoggerStream(*this, LogType::WARNING);
}

cpputils::Logger::LoggerStream cpputils::Logger::debug() {
  return LoggerStream(*this, LogType::DEBUG);
}

cpputils::Logger::LoggerStream cpputils::Logger::critical() {
  return LoggerStream(*this, LogType::CRITICAL);
}

void cpputils::Logger::flush(const std::string &txt,
                             cpputils::Logger::LogType t) {
  flush(txt, t, std::cout);
  if (export_) {
    flush(txt, t, fs_);
  }
}

void cpputils::Logger::flush(const std::string &txt,
                             cpputils::Logger::LogType t, std::ostream &ios) {
  std::lock_guard<std::mutex> lock(mutex_);
  if (show_datetime_) {
    datetime(ios);
  }
  if (show_prefix_ && (show_debug_ || t != LogType::DEBUG)) {
    ios << "[" << std::setw(8) << typeToString(t) << "] ";
  }
  ios << txt << std::endl;
}

const char *cpputils::Logger::typeToString(cpputils::Logger::LogType t) {
  switch (t) {
    case LogType::INFO:
      return "info";
    case LogType::DEBUG:
      return "debug";
    case LogType::WARNING:
      return "warning";
    case LogType::CRITICAL:
      return "critical";
  }
}

cpputils::Logger::LoggerStream::~LoggerStream() {
  logger_.flush(std::move(str()), t_);
}

/// log namespace

void cpputils::log::configure(int c) { logger.configure(c); }

void cpputils::log::configure(cpputils::Logger::Config c) {
  logger.configure(c);
}

void cpputils::log::exportToFile(const char *fname) {
  logger.exportToFile(fname);
}

cpputils::Logger::LoggerStream cpputils::log::info() { return logger.info(); }

cpputils::Logger::LoggerStream cpputils::log::warning() {
  return logger.warning();
}

cpputils::Logger::LoggerStream cpputils::log::debug() { return logger.debug(); }

cpputils::Logger::LoggerStream cpputils::log::critical() {
  return logger.critical();
}
