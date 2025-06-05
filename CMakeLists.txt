cmake_minimum_required(VERSION 3.16)

project(mincut LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_subdirectory(src)

add_executable(mincut src/main.cpp)
target_link_libraries(mincut PRIVATE mincut-lib)


# Testing
option(BUILD_TESTING "Build tests with Catch2" OFF)

if(BUILD_TESTING)
  include(FetchContent)
  set(FETCHCONTENT_BASE_DIR ${PROJECT_SOURCE_DIR}/dep)
  FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.4.0
  )
  FetchContent_MakeAvailable(Catch2)
  list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
  enable_testing()
  add_subdirectory(tests)
endif()