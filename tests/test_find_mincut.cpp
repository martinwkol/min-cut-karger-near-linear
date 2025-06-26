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
        checkMinCut(
            TestGraphs::simple, 
            findLikelyMinCut(TestGraphs::simple.graph, 3.0)
        );
    }

    SECTION("Medium graph") {
        checkMinCut(
            TestGraphs::medium, 
            findLikelyMinCut(TestGraphs::medium.graph, 3.0)
        );
    }

    SECTION("Multiple min-cuts") {
        checkMinCut(
            TestGraphs::multipleMinCuts, 
            findLikelyMinCut(TestGraphs::multipleMinCuts.graph, 3.0)
        );
    }

    SECTION("Dense graph") {
        checkMinCut(
            TestGraphs::dense, 
            findLikelyMinCut(TestGraphs::dense.graph, 3.0)
        );
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