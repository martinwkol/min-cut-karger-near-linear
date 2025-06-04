#include <iostream>
#include <limits>
#include <ctime>

#include "graph.hpp"
#include "two_respecting_trees.hpp"
#include "two_respecting_cuts.hpp"
#include "random.hpp"

double readDouble() {
    double d;
    std::cin >> d;
    return d;
}

WeightedGraph readWeightedGraph() {
    size_t numVertices;
    size_t numEdges;
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

    return WeightedGraph(numVertices, std::move(edges));
}

int main() {
    randomGenerator.seed(std::time(0));
    double d = readDouble();
    WeightedGraph graph = readWeightedGraph();
    TreePacking packing = findTwoRespectingTrees(graph, d);
    Cut minCut;
    minCut.weight = INFINITE_WEIGHT;
    for (const std::vector<size_t>& edgeSelection : packing.trees()) {
        RootedSpanningTree rst(graph, edgeSelection, 0);
        Cut smallestCut = findSmallest2RespectingCut(rst);
        if (smallestCut.weight < minCut.weight) {
            minCut = std::move(smallestCut);
        }
    }
    std::cout << minCut.weight << std::endl;
    for (VertexID vertex : minCut.vertices) {
        std::cout << vertex << '\n';
    }

    return 0;
}