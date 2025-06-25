#include "catch2/catch_test_macros.hpp"
#include "graph.hpp"
#include "tree_packing.hpp"
#include "random.hpp"
#include "graph_generation.hpp"

static void checkIfValid(const TreePacking& packing) {
    MultiGraph::EdgeVector<EdgeWeight> weightOfEdge(packing.graph().numEdges(), 0.0);
    for (const std::vector<MultiGraph::EdgeIndex>& edgeSelection : packing.trees()) {
        for (MultiGraph::EdgeIndex edgeIndex : edgeSelection) {
            weightOfEdge[edgeIndex] += packing.treeWeight();
        }
    }
    for (MultiGraph::EdgeIndex edgeIndex(0); edgeIndex < packing.graph().numEdges(); ++edgeIndex) {
        REQUIRE(weightOfEdge[edgeIndex] <= packing.graph().edge(edgeIndex).multiplicity());
    }
}

TEST_CASE("Compute Tree packing", "[tree-packing]") {
    randomGenerator.seed(123456);

    SECTION("Test random multigraphs") {
        for (int i = 0; i < 10; ++i) {
            WeightedGraph weightedGraph = randomConnectedWeightedGraph(10, 50, 1.0, 5.0);
            MultiGraph multiGraph = weightedGraph.approxAsMultiGraph(0.01);
            TreePacking packing(multiGraph, 1.0 / 5.0);
            checkIfValid(packing);
        }
    }
}