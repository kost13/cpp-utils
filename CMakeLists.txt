cmake_minimum_required(VERSION 3.10.0)

project(
  CppUtils
  VERSION 0.1
  DESCRIPTION "A set of cpp utility classes"
  LANGUAGES CXX)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

    include(CTest)

    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    find_package(Doxygen)
    if(Doxygen_FOUND)
      add_subdirectory(docs)
    else()
      message(WARNING "Doxygen not found, not building docs")
    endif()

endif()

add_subdirectory(src)

if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR CPP_UTILS_BUILD_TESTING)
   AND BUILD_TESTING)
    find_package(GTest)
    if(GTest_FOUND)
        add_subdirectory(tests)
    else()
        message(WARNING "GTest not found, not building tests")
    endif()
endif()
