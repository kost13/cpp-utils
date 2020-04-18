cmake_minimum_required(VERSION 3.10.0)

set(LIB_NAME CppUtils)

set(SRC_LIST
    cpputils/worker.cc
    cpputils/logger.cc
    cpputils/logger.hpp
    cpputils/guarded_thread.hpp
)

add_library(${LIB_NAME} STATIC ${SRC_LIST})

target_include_directories(${LIB_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if (CMAKE_COMPILER_IS_GNUCXX)    
    target_compile_options(${LIB_NAME} PRIVATE -Werror -Wall -Wextra -std=c++14)
    target_link_libraries(${LIB_NAME} PRIVATE -pthread)
endif()




