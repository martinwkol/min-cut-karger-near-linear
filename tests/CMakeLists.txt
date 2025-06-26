include(CTest)
include(Catch)

set(TEST_HELPERS
    stoer_wagner.cpp
    graph_generation.cpp
    util.cpp
)

add_executable(tests 
    test_tree_packing.cpp
    test_two_respecting_trees.cpp
    test_two_respecting_cuts.cpp
    test_find_mincut.cpp 
    ${TEST_HELPERS}
)
target_include_directories(tests PRIVATE ${CMAKE_SRC_DIR}/src)

target_link_libraries(tests PRIVATE mincut-lib)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

target_compile_options(tests PRIVATE
  -Wall -Wextra
  "$<$<CONFIG:Debug>:-g>"
  "$<$<CONFIG:Asan>:-O1;-g;-fsanitize=address;-fno-omit-frame-pointer>"
  "$<$<CONFIG:Release>:-O3;-DNDEBUG>"
)

target_link_options(tests PRIVATE
  "$<$<CONFIG:Asan>:-fsanitize=address>"
)

catch_discover_tests(tests)