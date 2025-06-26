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

static void checkFromWeightedGraph(const WeightedGraph& graph, double multApproxEps, double packingEps) {
    MultiGraph multiGraph = graph.approxAsMultiGraph(multApproxEps);
    TreePacking packing(multiGraph, packingEps);
    checkIfValid(packing);
    checkPackingSize(packing, packingEps);
}

TEST_CASE("Compute Tree packing", "[tree-packing]") {
    randomGenerator.seed(123456);

    SECTION("Simple graph") {
        WeightedGraph graph(4, {
            WeightedEdge(0, 1, 2.0),
            WeightedEdge(0, 2, 3.0),
            WeightedEdge(1, 3, 1.0),
            WeightedEdge(2, 3, 4.0),
        });
        checkFromWeightedGraph(graph, 0.01, 1.0 / 5.0);
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
        checkFromWeightedGraph(graph, 0.01, 1.0 / 5.0);
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
        checkFromWeightedGraph(graph, 0.01, 1.0 / 5.0);
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
        checkFromWeightedGraph(graph, 0.01, 1.0 / 5.0);
    }

    SECTION("Random sparse graphs, Weights: [1, 5]") {
        for (size_t numVertices = 10; numVertices <= 40; numVertices += 10) {
            checkFromWeightedGraph(
                randomConnectedWeightedGraph(numVertices, numVertices * 3 / 2, 1.0, 5.0),
                0.01, 1.0 / 5.0
            );
        }
    }

    SECTION("Random dense graphs, Weights: [1, 5]") {
        for (size_t numVertices = 10; numVertices <= 25; numVertices += 5) {
            checkFromWeightedGraph(
                randomConnectedWeightedGraph(numVertices, numVertices * numVertices / 3, 1.0, 5.0),
                0.01, 1.0 / 5.0
            );
        }
    }
}