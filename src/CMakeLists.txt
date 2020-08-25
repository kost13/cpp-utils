cmake_minimum_required(VERSION 3.10.0)

set(HEADER_PREFIX "${PROJECT_SOURCE_DIR}/include/cpputils")

set(HEADER_LIST
    "${HEADER_PREFIX}/logger.hpp"
    "${HEADER_PREFIX}/guarded_thread.hpp"
    "${HEADER_PREFIX}/worker.h"
)

set(SRC_LIST
    worker.cc
    logger.cc
)

set(LIB_NAME cpp-utils)

add_library(${LIB_NAME} STATIC ${SRC_LIST} ${HEADER_LIST})

target_include_directories(${LIB_NAME} PUBLIC ../include)

if (CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(${LIB_NAME} PRIVATE -Werror -Wall -Wextra -std=c++14)
    target_link_libraries(${LIB_NAME} PRIVATE -pthread)
elseif(MSVC)
    target_compile_options(${LIB_NAME} PRIVATE /W3)
endif()

source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST})

source_group(
  TREE "${CMAKE_CURRENT_SOURCE_DIR}"
  PREFIX "Source Files"
  FILES ${SRC_LIST})

