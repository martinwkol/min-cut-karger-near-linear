cmake_minimum_required(VERSION 3.10.0)

project(mincut LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

enable_testing()

add_subdirectory(src)
add_subdirectory(tests)

add_executable(mincut src/main.cpp)
target_link_libraries(mincut PRIVATE mincut-lib)

