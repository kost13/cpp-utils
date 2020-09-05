// cpputils tests
// author: Lukasz Kostrzewa

#include <thread>
#include <chrono>

#include <gtest/gtest.h>

#include "cpputils/guarded_thread.hpp"

TEST(CppUtilsGuardedThread, testDestructor){
    //sta::thread test would fail
    cpputils::guarded_thread gt([]{ std::this_thread::sleep_for(std::chrono::milliseconds(2)); });
}
