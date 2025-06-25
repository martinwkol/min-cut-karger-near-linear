#include <iostream>
#include <limits>
#include <ctime>

#include "graph.hpp"
#include "random.hpp"
#include "find_mincut.hpp"

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

    WeightedGraph::EdgeVector<WeightedEdge> edges;
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

static void printCut(const Cut& cut) {
    std::cout << cut.weight << '\n';
    for (VertexID vertex : cut.vertices) {
        std::cout << vertex << '\n';
    }
}

int main() {
    initRandomGenerator();
    ConsoleInput input = readConsoleInput();
    Cut minCut = findLikelyMinCut(input.graph, input.d);
    printCut(minCut);
    return 0;
}