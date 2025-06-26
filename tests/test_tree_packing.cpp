#include "catch2/catch_test_macros.hpp"
#include "graph.hpp"
#include "tree_packing.hpp"
#include "random.hpp"
#include "graph_generation.hpp"
#include "stoer_wagner.hpp"
#include "util.hpp"

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

static void check(const WeightedGraph& graph, double multApproxEps, double packingEps) {
    MultiGraph multiGraph = graph.approxAsMultiGraph(multApproxEps);
    TreePacking packing(multiGraph, packingEps);
    checkIfValid(packing);
    checkPackingSize(packing, packingEps);
}

TEST_CASE("Compute Tree packing", "[tree-packing]") {
    randomGenerator.seed(123456);
    double multApproxEps = 0.01;
    double packingEps = 1.0 / 5.0;

    SECTION("Simple graph") {
        check(TestGraphs::simple.graph, multApproxEps, packingEps);
    }

    SECTION("Medium graph") {
        check(TestGraphs::medium.graph, multApproxEps, packingEps);
    }

    SECTION("Multiple min-cuts") {
        check(TestGraphs::multipleMinCuts.graph, multApproxEps, packingEps);
    }

    SECTION("Dense graph") {
        check(TestGraphs::dense.graph, multApproxEps, packingEps);
    }

    SECTION("Random sparse graphs, Weights: [1, 5]") {
        for (size_t numVertices = 10; numVertices <= 40; numVertices += 10) {
            check(
                randomConnectedWeightedGraph(
                    numVertices, 
                    numVertices * 3 / 2, 
                    1.0, 
                    5.0
                ),
                multApproxEps, packingEps
            );
        }
    }

    SECTION("Random dense graphs, Weights: [1, 5]") {
        for (size_t numVertices = 10; numVertices <= 25; numVertices += 5) {
            check(
                randomConnectedWeightedGraph(
                    numVertices, 
                    numVertices * numVertices / 3, 
                    1.0, 
                    5.0
                ),
                multApproxEps, packingEps
            );
        }
    }
}