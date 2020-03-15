cmake_minimum_required(VERSION 3.16.0)

project(example LANGUAGES CXX)

include_directories(.)
include_directories(../cpputils)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")


set(SRC_LIST
    main.cpp
    ../cpputils/worker.cc
    ../cpputils/logger.cc

    )

set(HEADERS_LIST
    ../cpputils/worker.h
    ../cpputils/logger.hpp
    ../cpputils/guarded_thread.hpp
    )


add_executable(${PROJECT_NAME} ${SRC_LIST} ${HEADERS_LIST})
