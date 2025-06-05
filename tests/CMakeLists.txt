include(CTest)
include(Catch)

set(TEST_HELPERS
    stoer_wagner.cpp
    graph_generation.cpp
)

add_executable(tests test_random_graphs.cpp ${TEST_HELPERS})
target_include_directories(tests PRIVATE ${CMAKE_SRC_DIR}/src)
target_link_libraries(tests PRIVATE mincut-lib)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
catch_discover_tests(tests)