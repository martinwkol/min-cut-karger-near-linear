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

set(SCR_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

set(SOURCES
    ${SCR_DIR}/main.cpp
    ${SCR_DIR}/graph.cpp
    ${SCR_DIR}/random.cpp
    ${SCR_DIR}/rooted_spanning_tree.cpp
    ${SCR_DIR}/segment_tree.cpp
    ${SCR_DIR}/spanning_tree.cpp
    ${SCR_DIR}/stoer_wagner.cpp
    ${SCR_DIR}/tree_edge_weighter.cpp
    ${SCR_DIR}/tree_packing.cpp
    ${SCR_DIR}/two_respecting_cuts.cpp
    ${SCR_DIR}/two_respecting_trees.cpp
    ${SCR_DIR}/union_find.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

