set(TEST_SOURCES
    stoer_wagner.cpp
    graph_generation.cpp
)

function (add_unit_test name)
    add_executable(${name} ${ARGN} ${TEST_SOURCES})
    target_include_directories(${name} PRIVATE ${CMAKE_SRC_DIR}/src)
    target_link_libraries(${name} PRIVATE mincut-lib)
    add_test(NAME ${name} COMMAND ${name})
endfunction()

add_unit_test(t test_random_graphs.cpp)