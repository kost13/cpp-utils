cmake_minimum_required(VERSION 3.10.0)

macro(add_gtest TESTNAME)
    add_executable(${TESTNAME} ${TESTNAME}.cc)
    target_link_libraries(${TESTNAME} PRIVATE cpp-utils gtest gtest_main)
    gtest_discover_tests(${TESTNAME})
endmacro()

add_gtest(test_worker)
add_gtest(test_logger)
add_gtest(test_guarded_thread)

