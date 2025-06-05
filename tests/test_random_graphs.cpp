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
    Cut nearLinearResult = findLikelyMinCut(graph, 1.0);

    INFO("Own:\n");
    INFO(cut2string(nearLinearResult));
    INFO("Stoer Wagner:\n");
    INFO(cut2string(stoerWagnerResult));

    REQUIRE_THAT(nearLinearResult.weight, WithinRel(stoerWagnerResult.weight, 0.1));
}

TEST_CASE("Minimum cut on random graphs", "[mincut][mincut-general]" ) {
    SECTION("Vertices: 10, Edges: 20, Weights: [1, 5]") {
        for (int i = 0; i < 10; ++i) {
            compateToSW(randomConnectedWeightedGraph(10, 50, 1.0, 5.0));
        }
    }
    SECTION("Vertices: 50, Edges: 200, Weights: [1, 5]") {
        compateToSW(randomConnectedWeightedGraph(50, 200, 1.0, 5.0));
    }
}