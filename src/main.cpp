#include <iostream>
#include <limits>
#include <ctime>

#include "graph.hpp"
#include "two_respecting_trees.hpp"
#include "two_respecting_cuts.hpp"
#include "random.hpp"

struct ConsoleInput {
    double d;
    WeightedGraph graph;
};

static void initRandomGenerator() {
    randomGenerator.seed(std::time(0));
}

static ConsoleInput readConsoleInput() {
    double d;
    size_t numVertices;
    size_t numEdges;

    std::cin >> d;
    std::cin >> numVertices >> numEdges;

    std::vector<WeightedEdge> edges;
    edges.reserve(numEdges);

    for (size_t i = 0; i < numEdges; i++) {
        VertexID endpoint0;
        VertexID endpoint1;
        EdgeWeight weight;

        std::cin >> endpoint0 >> endpoint1 >> weight;
        edges.emplace_back(endpoint0, endpoint1, weight);
    }

    return { d, WeightedGraph(numVertices, std::move(edges)) };
}

static Cut findLikelyMinCut(const WeightedGraph& graph, const TreePacking& packing) {
    Cut minCut;
    minCut.weight = INFINITE_WEIGHT;
    for (const std::vector<size_t>& edgeSelection : packing.trees()) {
        RootedSpanningTree rst(graph, edgeSelection, 0);
        Cut smallestCut = findSmallest2RespectingCut(rst);
        if (smallestCut.weight < minCut.weight) {
            minCut = std::move(smallestCut);
        }
    }
    return minCut;
}

static void printCut(const Cut& cut) {
    std::cout << cut.weight << '\n';
    for (VertexID vertex : cut.vertices) {
        std::cout << vertex << '\n';
    }
}

int main() {
    initRandomGenerator();
    ConsoleInput input = readConsoleInput();
    TreePacking packing = findTwoRespectingTrees(input.graph, input.d);
    Cut minCut = findLikelyMinCut(input.graph, packing);
    printCut(minCut);
    return 0;
}