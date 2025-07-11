#include <algorithm>
#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"
#include "graph.hpp"
#include "rooted_spanning_tree.hpp"
#include "two_respecting_cuts.hpp"
#include "graph_generation.hpp"
#include "random.hpp"
#include "util.hpp"


using Catch::Matchers::WithinRel;

static Cut smallest2resp(const RootedSpanningTree& rst) {
    Cut smallestCut;
    smallestCut.weight = INFINITE_WEIGHT;
    // strictly 1-respecting
    for (RootedSpanningTree::EdgeIndex i(0); i < rst.numEdges(); ++i) {
        Cut cut = cutFromVertices(
            rst.graph(), rst.cutVerticesFromCrossingEdges({ i })
        );
        if (cut.weight < smallestCut.weight) {
            smallestCut = std::move(cut);
        }
    }
    // strictly 2-respecting
    for (RootedSpanningTree::EdgeIndex i(0); i < rst.numEdges(); ++i) {
        for (RootedSpanningTree::EdgeIndex j(0); j < rst.numEdges(); ++j) {
            if (i == j) continue;
            Cut cut = cutFromVertices(
                rst.graph(), 
                rst.cutVerticesFromCrossingEdges({ i, j })
            );
            if (cut.weight < smallestCut.weight) {
                smallestCut = std::move(cut);
            }
        }
    }
    return smallestCut;
}


TEST_CASE("Find smallest 2-respecting cut", "[2-respecting]") {
    randomGenerator.seed(123456);

    SECTION("Strictly 1-respecting #1") {
        WeightedGraph graph(6, {
            WeightedEdge(0, 1, 3.0), // 0
            WeightedEdge(1, 2, 3.0), // 1
            WeightedEdge(2, 3, 3.0), // 2
            WeightedEdge(3, 0, 3.0), // 3
            WeightedEdge(2, 4, 1.0), // 4
            WeightedEdge(4, 5, 5.0), // 5
            WeightedEdge(5, 2, 1.0), // 6
        });
        RootedSpanningTree rst(graph, { 
            WeightedGraph::EdgeIndex(0), 
            WeightedGraph::EdgeIndex(1), 
            WeightedGraph::EdgeIndex(3), 
            WeightedGraph::EdgeIndex(4), 
            WeightedGraph::EdgeIndex(5) 
        }, 0);
        Cut cut = findSmallest2RespectingCut(rst);
        INFO(cut2string(cut));
        REQUIRE(cut.weight == 2.0);
        REQUIRE(cut.vertices.size() == 2);
        std::sort(cut.vertices.begin(), cut.vertices.end());
        REQUIRE(cut.vertices[0] == 4);
        REQUIRE(cut.vertices[1] == 5);
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
        RootedSpanningTree rst(graph, { 
            WeightedGraph::EdgeIndex(0), 
            WeightedGraph::EdgeIndex(1), 
            WeightedGraph::EdgeIndex(2), 
            WeightedGraph::EdgeIndex(3), 
            WeightedGraph::EdgeIndex(4), 
            WeightedGraph::EdgeIndex(5), 
            WeightedGraph::EdgeIndex(6) 
        }, 0);
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
        RootedSpanningTree rst(graph, { 
            WeightedGraph::EdgeIndex(1), 
            WeightedGraph::EdgeIndex(5), 
            WeightedGraph::EdgeIndex(3), 
            WeightedGraph::EdgeIndex(4) 
        }, 2);
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
        RootedSpanningTree rst(graph, { 
            WeightedGraph::EdgeIndex(0), 
            WeightedGraph::EdgeIndex(1), 
            WeightedGraph::EdgeIndex(3), 
            WeightedGraph::EdgeIndex(4), 
            WeightedGraph::EdgeIndex(5)
        }, 0);
        Cut cut = findSmallest2RespectingCut(rst);
        REQUIRE(cut.weight == 2.0);
        REQUIRE(cut.vertices.size() == 3);
        std::sort(cut.vertices.begin(), cut.vertices.end());
        REQUIRE(cut.vertices[0] == 2);
        REQUIRE(cut.vertices[1] == 3);
        REQUIRE(cut.vertices[2] == 4);
    }

    SECTION("Random trees") {
        for (size_t numVertices = 20; numVertices <= 80; numVertices += 20) {
            WeightedGraph graph = randomConnectedWeightedGraph(numVertices, numVertices - 1, 1.0, 10.0);
            RootedSpanningTree rst = randomSpanningTree(graph);
            Cut cut = findSmallest2RespectingCut(rst);
            Cut referenceCut = smallest2resp(rst);
            INFO(cut2string(cut));
            REQUIRE_THAT(cut.weight, WithinRel(referenceCut.weight));
        }
    }

    SECTION("Sparse graphs") {
        for (size_t numVertices = 20; numVertices <= 80; numVertices += 20) {
            WeightedGraph graph = randomConnectedWeightedGraph(numVertices, numVertices * 3 / 2, 1.0, 10.0);
            RootedSpanningTree rst = randomSpanningTree(graph);
            Cut cut = findSmallest2RespectingCut(rst);
            Cut referenceCut = smallest2resp(rst);
            INFO(cut2string(cut));
            REQUIRE_THAT(cut.weight, WithinRel(referenceCut.weight, 0.0001));
        }
    }
    

    SECTION("Dense graphs") {
        for (size_t numVertices = 20; numVertices <= 80; numVertices += 20) {
            WeightedGraph graph = randomConnectedWeightedGraph(numVertices, numVertices * numVertices / 3, 1.0, 10.0);
            RootedSpanningTree rst = randomSpanningTree(graph);
            Cut cut = findSmallest2RespectingCut(rst);
            Cut referenceCut = smallest2resp(rst);
            INFO(cut2string(cut));
            REQUIRE_THAT(cut.weight, WithinRel(referenceCut.weight, 0.0001));
        }
    }

    
}