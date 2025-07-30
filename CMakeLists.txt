cmake_minimum_required(VERSION 3.16)
project(mincut LANGUAGES CXX)

# Default to Release if no build type is specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Optional: support Asan for single-config or multi-config generators
if(CMAKE_GENERATOR MATCHES "Visual Studio|Xcode|Multi-Config")
  set(CMAKE_CONFIGURATION_TYPES "Debug;Release;RelWithDebInfo;MinSizeRel;Asan" CACHE STRING "" FORCE)
endif()

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add subdirectories
add_subdirectory(src)

# Define executable
add_executable(mincut src/main.cpp)
target_link_libraries(mincut PRIVATE mincut-lib)

# Target-specific compile options
target_compile_options(mincut PRIVATE
  -Wall -Wextra
  "$<$<CONFIG:Debug>:-g>"
  "$<$<CONFIG:Asan>:-O1;-g;-fsanitize=address;-fno-omit-frame-pointer>"
  "$<$<CONFIG:Release>:-O3;-DNDEBUG>"
)

target_link_options(mincut PRIVATE
  "$<$<CONFIG:Asan>:-fsanitize=address>"
)

# Testing support
option(BUILD_TESTING "Build tests with Catch2" OFF)

if(BUILD_TESTING)
  include(FetchContent)
  set(FETCHCONTENT_BASE_DIR ${PROJECT_SOURCE_DIR}/dep)

  FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.4.0
  )
  FetchContent_MakeAvailable(Catch2)

  enable_testing()
  add_subdirectory(tests)
endif()