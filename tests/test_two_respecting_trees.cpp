#include "catch2/catch_test_macros.hpp"
#include "two_respecting_trees.hpp"
#include "rooted_spanning_tree.hpp"
#include "random.hpp"
#include "graph_generation.hpp"
#include "stoer_wagner.hpp"
#include "util.hpp"

static void searchTwoRespectingTree(const WeightedGraph& graph, const TreePacking& packing, const Cut& minCut) {
    std::vector<bool> inCut(graph.numVertices(), false);
    for (VertexID vertex : minCut.vertices) {
        inCut[vertex] = true;
    }
    
    bool twoRespectingTreeFound = false;
    for (const std::vector<WeightedGraph::EdgeIndex>& edgeSelection : packing.trees()) {
        RootedSpanningTree rst(graph, edgeSelection, 0);
        int crossingEdgeCounter = 0;
        for (RootedSpanningTree::EdgeIndex i(0); i < rst.numEdges(); ++i) {
            const WeightedEdge& edge = rst.edge(i);
            if (
                (inCut[edge.endpoint(0)] && !inCut[edge.endpoint(1)])
                ||
                (!inCut[edge.endpoint(0)] && inCut[edge.endpoint(1)])
            ) crossingEdgeCounter++;
            if (crossingEdgeCounter >= 3) break;
        }
        if (crossingEdgeCounter <= 2) {
            twoRespectingTreeFound = true;
            break;
        }
    }
    REQUIRE(twoRespectingTreeFound);
}

static void check(const WeightedGraph& graph) {
    Cut minCut = stoerWagner(graph);
    TreePacking packing = findTwoRespectingTrees(graph, 3.0);
    searchTwoRespectingTree(graph, packing, minCut);
}

TEST_CASE("Test find 2-respecting trees", "[find-2-resp]") {
    randomGenerator.seed(123456);

    SECTION("Simple graph") {
        check(TestGraphs::simple.graph);
    }

    SECTION("Medium graph") {
        check(TestGraphs::medium.graph);
    }

    SECTION("Multiple min-cuts") {
        check(TestGraphs::multipleMinCuts.graph);
    }

    SECTION("Dense graph") {
        check(TestGraphs::dense.graph);
    }

    SECTION("Random sparse graphs, Weights: [1, 5]") {
        for (size_t numVertices = 10; numVertices <= 40; numVertices += 10) {
            check(
                randomConnectedWeightedGraph(numVertices, 
                    numVertices * 3 / 2, 
                    1.0, 
                    5.0
                )
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
                )
            );
        }
    }
}