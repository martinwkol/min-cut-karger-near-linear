#include <stdexcept>
#include <algorithm>
#include <sstream>
#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"
#include "graph_generation.hpp"
#include "stoer_wagner.hpp"
#include "find_mincut.hpp"
#include "util.hpp"

using Catch::Matchers::WithinRel;


static void compateToSW(const WeightedGraph& graph) {
    Cut stoerWagnerResult = stoerWagner(graph);
    Cut nearLinearResult = findLikelyMinCut(graph, 3.0);

    INFO("Own:\n");
    INFO(cut2string(nearLinearResult));
    INFO("Stoer Wagner:\n");
    INFO(cut2string(stoerWagnerResult));
    INFO(graph2string(graph));

    REQUIRE_THAT(nearLinearResult.weight, WithinRel(stoerWagnerResult.weight, 0.0001));
}

TEST_CASE("Find minumum cut", "[mincut]" ) {
    SECTION("Simple graph") {
        WeightedGraph graph(4, {
            WeightedEdge(0, 1, 2.0),
            WeightedEdge(0, 2, 3.0),
            WeightedEdge(1, 3, 1.0),
            WeightedEdge(2, 3, 4.0),
        });
        Cut minCut = findLikelyMinCut(graph, 3.0);
        requireEqual(minCut, { { 1 }, 3.0 }, graph.numVertices());
    }

    SECTION("Medium graph") {
        WeightedGraph graph(6, {
            WeightedEdge(0, 1, 3.0),
            WeightedEdge(0, 2, 2.0),
            WeightedEdge(1, 2, 1.0),
            WeightedEdge(1, 3, 2.0),
            WeightedEdge(2, 4, 2.0),
            WeightedEdge(3, 5, 3.0),
            WeightedEdge(4, 5, 3.0),
        });
        Cut minCut = findLikelyMinCut(graph, 3.0);
        requireEqual(minCut, { { 0, 1, 2 }, 4.0 }, graph.numVertices());
    }

    SECTION("Multiple min-cuts") {
        WeightedGraph graph(4, {
            WeightedEdge(0, 1, 1.0),
            WeightedEdge(0, 2, 1.0),
            WeightedEdge(0, 3, 1.0),
            WeightedEdge(1, 2, 1.0),
            WeightedEdge(1, 3, 1.0),
            WeightedEdge(2, 3, 1.0),
        });
        Cut minCut = findLikelyMinCut(graph, 3.0);
        REQUIRE(minCut.weight == 3.0);
        bool correctSize = minCut.vertices.size() == 1 || minCut.vertices.size() == 3;
        REQUIRE(correctSize);
    }

    SECTION("Dense graph") {
        WeightedGraph graph(5, {
            WeightedEdge(0, 1, 4.0),
            WeightedEdge(0, 2, 1.0),
            WeightedEdge(0, 3, 6.0),
            WeightedEdge(0, 4, 9.0),

            WeightedEdge(1, 2, 8.0),
            WeightedEdge(1, 3, 1.0),
            WeightedEdge(1, 4, 1.0),

            WeightedEdge(2, 3, 2.0),
            WeightedEdge(2, 4, 1.0),
            
            WeightedEdge(3, 4, 5.0),
        });
        Cut minCut = findLikelyMinCut(graph, 3.0);
        requireEqual(minCut, { { 1, 2 }, 10.0 }, graph.numVertices());
    }

    SECTION("Random sparse graphs, Weights: [1, 5]") {
        for (size_t numVertices = 10; numVertices <= 40; numVertices += 10) {
            compateToSW(randomConnectedWeightedGraph(numVertices, numVertices * 3 / 2, 1.0, 5.0));
        }
    }

    SECTION("Random dense graphs, Weights: [1, 5]") {
        for (size_t numVertices = 10; numVertices <= 25; numVertices += 5) {
            compateToSW(randomConnectedWeightedGraph(numVertices, numVertices * numVertices / 3, 1.0, 5.0));
        }
    }
}