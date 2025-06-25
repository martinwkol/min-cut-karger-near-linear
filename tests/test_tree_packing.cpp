#include "catch2/catch_test_macros.hpp"
#include "graph.hpp"
#include "tree_packing.hpp"
#include "random.hpp"
#include "graph_generation.hpp"
#include "stoer_wagner.hpp"

static WeightedGraph multiAsWeighted(const MultiGraph& multiGraph) {
    WeightedGraph::EdgeVector<WeightedEdge> wEdges;
    wEdges.reserve(multiGraph.numEdges());
    for (const MultiEdge& mEdge : multiGraph.edges()) {
        wEdges.emplace_back(mEdge.endpoint(0), mEdge.endpoint(1), mEdge.multiplicity());
    }
    return WeightedGraph(multiGraph.numVertices(), std::move(wEdges));
}

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

static void checkPackingSize(const TreePacking& packing, double epsilon) {
    WeightedGraph graph = multiAsWeighted(packing.graph());
    Cut minCut = stoerWagner(graph);
    REQUIRE(packing.packingWeight() >= (1 - epsilon) * minCut.weight / 2);
}

TEST_CASE("Compute Tree packing", "[tree-packing]") {
    randomGenerator.seed(123456);

    SECTION("Test random multigraphs") {
        double packingEpsilon = 1.0 / 5.0;
        for (int i = 0; i < 10; ++i) {
            WeightedGraph weightedGraph = randomConnectedWeightedGraph(10, 50, 1.0, 5.0);
            MultiGraph multiGraph = weightedGraph.approxAsMultiGraph(0.01);
            TreePacking packing(multiGraph, packingEpsilon);
            checkIfValid(packing);
            checkPackingSize(packing, packingEpsilon);
        }
    }
}