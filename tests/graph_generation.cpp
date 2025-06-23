#include "graph_generation.hpp"
#include "random.hpp"
#include "union_find.hpp"

#include <vector>

WeightedGraph randomConnectedWeightedGraph(size_t numVertices, size_t numEdges, EdgeWeight minWeight, EdgeWeight maxWeight) {
    std::uniform_int_distribution<int> vertexDistr(0, std::numeric_limits<int>::max());
    std::uniform_real_distribution<double> weightDistr(minWeight, maxWeight);

    std::vector<WeightedEdge> edges;
    edges.reserve(numEdges);

    // Ensure that the graph is connected
    for (VertexID endpoint0 = 1; endpoint0 < numVertices; endpoint0++) {
        VertexID endpoint1 = vertexDistr(randomGenerator) % endpoint0;
        EdgeWeight weight = weightDistr(randomGenerator);
        edges.emplace_back(endpoint0, endpoint1, weight);
    }

    // Add remaining edges
    for (size_t i = numVertices; i < numEdges; i++) {
        VertexID endpoint0 = vertexDistr(randomGenerator) % numVertices;
        VertexID endpoint1 = vertexDistr(randomGenerator) % (numVertices - 1);
        if (endpoint1 >= endpoint0) endpoint1++;
        EdgeWeight weight = weightDistr(randomGenerator);
        edges.emplace_back(endpoint0, endpoint1, weight);
    }

    return WeightedGraph(numVertices, std::move(edges));
}

RootedSpanningTree randomSpanningTree(const WeightedGraph& graph) {
    std::uniform_int_distribution<int> intDistr(0, std::numeric_limits<int>::max());
    std::vector<size_t> edgeSelection;
    std::vector<size_t> unvisitedEdgeIdxs(graph.numEdges());
    UnionFind uf(graph.numVertices());
    edgeSelection.reserve(graph.numVertices() - 1);
    std::iota(unvisitedEdgeIdxs.begin(), unvisitedEdgeIdxs.end(), 0);
    while (!unvisitedEdgeIdxs.empty() && edgeSelection.size() < graph.numVertices() - 1) {
        int index = intDistr(randomGenerator) % unvisitedEdgeIdxs.size();
        const Edge& edge = graph.edge(index);
        if (uf.find(edge.endpoint(0) != uf.find(edge.endpoint(1)))) {
            uf.unionSets(edge.endpoint(0), edge.endpoint(1));
            edgeSelection.push_back(index);
        }
        unvisitedEdgeIdxs[index] = unvisitedEdgeIdxs.back();
        unvisitedEdgeIdxs.pop_back();
    }
    return RootedSpanningTree(graph, std::move(edgeSelection), 0);
}
