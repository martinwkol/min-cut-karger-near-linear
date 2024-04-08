#include <iostream>
#include <limits>

#include "graph.hpp"
#include "two_respecting_trees.hpp"
#include "two_respecting_cuts.hpp"

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
    WeightedGraph graph = readWeightedGraph();
    TreePacking packing = findTwoRespectingTrees(graph, 3);
    std::vector<VertexID> minCut;
    EdgeWeight minCutWeight = INFINITE_WEIGHT;
    for (const std::vector<size_t>& edgeSelection : packing.trees()) {
        RootedSpanningTree rst(graph, edgeSelection, 0);
        auto smallestCut = findSmallest2RespectingCut(rst);
        if (smallestCut.second < minCutWeight) {
            minCutWeight = smallestCut.second;
            minCut = std::move(smallestCut.first);
        }
    }
    std::cout << minCutWeight << std::endl;
    for (VertexID vertex : minCut) {
        std::cout << vertex << '\n';
    }

    return 0;
}