// cpputils tests
// author: Lukasz Kostrzewa

#include <gtest/gtest.h>

#include <chrono>
#include <thread>

#include "cpputils/worker.h"

TEST(CppUtilsWorker, testAsync) {
  cpputils::Worker worker;
  worker.start();

  ASSERT_TRUE(worker.empty());

  std::vector<int> nums;

  worker.async([&nums] { nums.push_back(1); });
  worker.async([] { return; });

  while (!worker.empty())
    ;

  ASSERT_TRUE(!nums.empty());
  ASSERT_EQ(nums[0], 1);

  worker.async([&nums] {
    nums.push_back(2);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  });
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  worker.async([&nums] { nums.push_back(3); });
  worker.priorityAsync([&nums] { nums.push_back(4); });

  worker.async([] { return; });

  while (!worker.empty())
    ;

  ASSERT_EQ(nums.size(), 4);
  ASSERT_EQ(nums[1], 2);
  // called as priority
  ASSERT_EQ(nums[2], 4);
  ASSERT_EQ(nums[3], 3);
}

TEST(CppUtilsWorker, testStop) {
  cpputils::Worker worker;
  worker.start();
  worker.async(
      [] { std::this_thread::sleep_for(std::chrono::milliseconds(50)); });
  worker.async(
      [] { std::this_thread::sleep_for(std::chrono::milliseconds(50)); });
  worker.stop(false);
  ASSERT_TRUE(!worker.empty());
  worker.start();
  worker.async(
      [] { std::this_thread::sleep_for(std::chrono::milliseconds(50)); });
  worker.async(
      [] { std::this_thread::sleep_for(std::chrono::milliseconds(50)); });
  worker.stop(true);
  ASSERT_TRUE(worker.empty());
}
