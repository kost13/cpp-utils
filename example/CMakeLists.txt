cmake_minimum_required(VERSION 3.10.0)

project(CppUtilsExample LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

include_directories(.. cpputils)

set(SRC_LIST
    main.cpp
    ../cpputils/worker.cc
    ../cpputils/logger.cc
    ../cpputils/logger.hpp
    ../cpputils/guarded_thread.hpp
)

add_executable(${PROJECT_NAME} ${SRC_LIST})

