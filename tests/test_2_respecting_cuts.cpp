#include <algorithm>
#include "catch2/catch_test_macros.hpp"
#include "graph.hpp"
#include "rooted_spanning_tree.hpp"
#include "two_respecting_cuts.hpp"
#include "util.hpp"

TEST_CASE("Find smallest 2-respecting cut", "[2-respecting]") {
    SECTION("Strictly 1-respecting #1") {
        WeightedGraph graph(7, {
            WeightedEdge(0, 1, 3.0), // 0
            WeightedEdge(1, 2, 3.0), // 1
            WeightedEdge(2, 3, 3.0), // 2
            WeightedEdge(3, 0, 3.0), // 3
            WeightedEdge(2, 5, 1.0), // 4
            WeightedEdge(5, 6, 5.0), // 5
            WeightedEdge(6, 2, 1.0), // 6
        });
        RootedSpanningTree rst(graph, { 0, 1, 3, 4, 5 }, 0);
        Cut cut = findSmallest2RespectingCut(rst);
        INFO(cut2string(cut));
        REQUIRE(cut.weight == 2.0);
        REQUIRE(cut.vertices.size() == 2);
        std::sort(cut.vertices.begin(), cut.vertices.end());
        REQUIRE(cut.vertices[0] == 5);
        REQUIRE(cut.vertices[1] == 6);
    }

    SECTION("Strictly 1-respecting #2") {
        WeightedGraph graph(8, {
            WeightedEdge(0, 1, 5.0), // 0
            WeightedEdge(0, 2, 3.0), // 1
            WeightedEdge(1, 3, 0.5), // 2
            WeightedEdge(3, 4, 2.0), // 3
            WeightedEdge(3, 5, 1.5), // 4
            WeightedEdge(2, 6, 5.0), // 5
            WeightedEdge(2, 7, 1.0), // 6
        });
        RootedSpanningTree rst(graph, { 0, 1, 2, 3, 4, 5, 6 }, 0);
        Cut cut = findSmallest2RespectingCut(rst);
        INFO(cut2string(cut));
        REQUIRE(cut.weight == 0.5);
        REQUIRE(cut.vertices.size() == 3);
        std::sort(cut.vertices.begin(), cut.vertices.end());
        REQUIRE(cut.vertices[0] == 3);
        REQUIRE(cut.vertices[1] == 4);
        REQUIRE(cut.vertices[2] == 5);
    }

    SECTION("Strictly 2-respecting #1") {
        WeightedGraph graph(5, {
            WeightedEdge(0, 1, 100.0),      // 0
            WeightedEdge(0, 2, 100.0),      // 1
            WeightedEdge(1, 3, 1.0),        // 2
            WeightedEdge(1, 4, 1.0),        // 3
            WeightedEdge(3, 4, 3.0),        // 4
            WeightedEdge(2, 4, 1.0),        // 5
        });
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
        WeightedGraph graph(6, {
            WeightedEdge(0, 1, 5.0), // 0
            WeightedEdge(1, 2, 1.0), // 1
            WeightedEdge(2, 3, 4.0), // 2
            WeightedEdge(3, 4, 3.0), // 3
            WeightedEdge(4, 5, 1.0), // 4
            WeightedEdge(5, 0, 2.0), // 5
        });
        RootedSpanningTree rst(graph, { 0, 1, 3, 4, 5 }, 0);
        Cut cut = findSmallest2RespectingCut(rst);
        REQUIRE(cut.weight == 2.0);
        REQUIRE(cut.vertices.size() == 3);
        std::sort(cut.vertices.begin(), cut.vertices.end());
        REQUIRE(cut.vertices[0] == 2);
        REQUIRE(cut.vertices[1] == 3);
        REQUIRE(cut.vertices[2] == 4);
    }
}