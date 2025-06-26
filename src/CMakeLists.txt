add_library(mincut-lib
    find_mincut.cpp
    graph.cpp
    random.cpp
    rooted_spanning_tree.cpp
    segment_tree.cpp
    spanning_tree.cpp
    tree_edge_weighter.cpp
    tree_packing.cpp
    two_respecting_cuts.cpp
    two_respecting_trees.cpp
    union_find.cpp
)

target_include_directories(mincut-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(mincut-lib PRIVATE
  -Wall -Wextra
  "$<$<CONFIG:Debug>:-g>"
  "$<$<CONFIG:Asan>:-O1;-g;-fsanitize=address;-fno-omit-frame-pointer>"
  "$<$<CONFIG:Release>:-O3;-DNDEBUG>"
)

target_link_options(mincut-lib PRIVATE
  "$<$<CONFIG:Asan>:-fsanitize=address>"
)