include(CTest)
include(Catch)

set(TEST_HELPERS
    stoer_wagner.cpp
    graph_generation.cpp
)

function (add_test name)
    add_executable(${name} ${ARGN} ${TEST_HELPERS})
    target_include_directories(${name} PRIVATE ${CMAKE_SRC_DIR}/src)
    target_link_libraries(${name} PRIVATE mincut-lib)
    target_link_libraries(${name} PRIVATE Catch2::Catch2WithMain)
    catch_discover_tests(${name})
endfunction()

add_unit_test(t test_random_graphs.cpp)