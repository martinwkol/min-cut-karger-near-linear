#include <algorithm>
#include "catch2/catch_test_macros.hpp"
#include "graph.hpp"
#include "rooted_spanning_tree.hpp"
#include "two_respecting_cuts.hpp"
#include "util.hpp"

TEST_CASE("Find smallest 2-respecting cut", "[2-respecting]") {
    WeightedGraph graph(5, {
        WeightedEdge(0, 1, 100.0),   // 0
        WeightedEdge(0, 2, 100.0),   // 1
        WeightedEdge(1, 3, 1.0),    // 2
        WeightedEdge(1, 4, 1.0),    // 3
        WeightedEdge(3, 4, 3.0),    // 4
        WeightedEdge(2, 4, 1.0),    // 5
    });

    SECTION("Strictly 2-respecting #1") {
        RootedSpanningTree rst(graph, { 1, 5, 3, 4 }, 2);
        Cut cut = findSmallest2RespectingCut(rst);
        INFO(cut2string(cut));
        REQUIRE(cut.weight == 3.0);
        REQUIRE(cut.vertices.size() == 2);
        std::sort(cut.vertices.begin(), cut.vertices.end());
        REQUIRE(cut.vertices[0] == 3);
        REQUIRE(cut.vertices[1] == 4);
    }

    SECTION("Strictly 2-respecting #2") {
        RootedSpanningTree rst(graph, { 0, 1, 2, 3 }, 0);
        Cut cut = findSmallest2RespectingCut(rst);
        REQUIRE(cut.weight == 3.0);
        REQUIRE(cut.vertices.size() == 2);
        std::sort(cut.vertices.begin(), cut.vertices.end());
        REQUIRE(cut.vertices[0] == 3);
        REQUIRE(cut.vertices[1] == 4);
    }
}