cmake_minimum_required(VERSION 3.10.0)

set(LIB_NAME CppUtils)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

set(SRC_LIST
    cpputils/worker.cc
    cpputils/logger.cc
    cpputils/logger.hpp
    cpputils/guarded_thread.hpp
)

add_library(${LIB_NAME} STATIC ${SRC_LIST})
target_include_directories(${LIB_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})




