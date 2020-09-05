// cpputils tests
// author: Lukasz Kostrzewa

#include <gtest/gtest.h>

#include "cpputils/logger.hpp"

namespace {
struct cout_redirect {
  cout_redirect() { old_ = std::cout.rdbuf(buffer_.rdbuf()); }
  std::string str() {
    auto s = buffer_.str();
    buffer_.clear();
    return s;
  }
  ~cout_redirect() { std::cout.rdbuf(old_); }

 private:
  std::stringstream buffer_;
  std::streambuf* old_;
};

bool startsWithDatetime(const std::string& str) {
  if (str.size() < 20) {
    return false;
  }
  auto digit_indices = {0, 1, 2, 3, 5, 6, 8, 9, 11, 12, 14, 15, 17, 18};
  for (auto i : digit_indices) {
    if (!std::isdigit(str[i])) {
      return false;
    }
  }

  for (auto i : {4, 7}) {
    if (str[i] != '-') {
      return false;
    }
  }

  return true;
}

}  // namespace

TEST(CppUtilsLogger, testConfiguration) {
  cpputils::Logger logger;

  {
    cout_redirect cout;
    std::string s = "Abc";
    logger.info() << s;
    auto out = cout.str();
    ASSERT_TRUE(startsWithDatetime(out));
    ASSERT_TRUE(out.find("info") != std::string::npos);
    ASSERT_TRUE(out.find(s) != std::string::npos);
    printf("datetime [    info] msg: %s", out.c_str());
  }
  {
    cout_redirect cout;
    std::string s = "Abc";
    logger.warning() << s;
    auto out = cout.str();
    ASSERT_TRUE(startsWithDatetime(out));
    ASSERT_TRUE(out.find("warning") != std::string::npos);
    ASSERT_TRUE(out.find(s) != std::string::npos);
    printf("datetime [ warning] msg: %s", out.c_str());
  }
  {
    cout_redirect cout;
    std::string s = "Abc";
    logger.critical() << s;
    auto out = cout.str();
    ASSERT_TRUE(startsWithDatetime(out));
    ASSERT_TRUE(out.find("critical") != std::string::npos);
    ASSERT_TRUE(out.find(s) != std::string::npos);
    printf("datetime [critical] msg: %s", out.c_str());
  }
  {
    cout_redirect cout;
    std::string s = "Abc";
    logger.debug() << s;
    auto out = cout.str();
    ASSERT_TRUE(startsWithDatetime(out));
    ASSERT_TRUE(out.find("debug") != std::string::npos);
    ASSERT_TRUE(out.find(s) != std::string::npos);
    printf("datetime [   debug] msg: %s", out.c_str());
  }

  // no debug
  logger.configure(cpputils::Logger::Config::SHOW_PREFIX |
                   cpputils::Logger::Config::SHOW_DATETIME);

  {
    cout_redirect cout;
    std::string s = "Def";
    logger.debug() << s;
    auto out = cout.str();
    ASSERT_TRUE(out.empty());
  }

  {
    cout_redirect cout;
    std::string s = "Def";
    logger.info() << s;
    auto out = cout.str();
    ASSERT_TRUE(startsWithDatetime(out));
    ASSERT_TRUE(out.find("info") != std::string::npos);
    ASSERT_TRUE(out.find(s) != std::string::npos);
    printf("datetime [  info] msg: %s", out.c_str());
  }

  // no datetime
  logger.configure(cpputils::Logger::Config::SHOW_PREFIX);
  {
    cout_redirect cout;
    std::string s = "Ghi";
    logger.info() << s;
    auto out = cout.str();
    ASSERT_FALSE(startsWithDatetime(out));
    ASSERT_TRUE(out.find("info") != std::string::npos);
    ASSERT_TRUE(out.find(s) != std::string::npos);
    printf("[  info] msg: %s", out.c_str());
  }

  // no prefix
  logger.configure(0);
  {
    cout_redirect cout;
    std::string s = "Ghi";
    logger.info() << s;
    auto out = cout.str();
    ASSERT_FALSE(startsWithDatetime(out));
    ASSERT_FALSE(out.find("info") != std::string::npos);
    ASSERT_TRUE(out.find(s) != std::string::npos);
    printf("msg: %s", out.c_str());
  }
}
