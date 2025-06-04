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