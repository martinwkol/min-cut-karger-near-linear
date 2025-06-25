#include "graph_generation.hpp"
#include "random.hpp"
#include "union_find.hpp"

#include <vector>
#include <set>
#include <algorithm>
#include <cassert>

struct VertexPair {
    VertexID v0;
    VertexID v1;
    bool operator<(const VertexPair& other) const { 
        return
            v0 < other.v0 ||
            (v0 == other.v0 && v1 < other.v1);
    }
};


WeightedGraph randomConnectedWeightedGraph(size_t numVertices, size_t numEdges, EdgeWeight minWeight, EdgeWeight maxWeight) {
    assert(numEdges <= numVertices * (numVertices - 1) / 2);
    
    std::uniform_int_distribution<int> intDistr(0, std::numeric_limits<int>::max());
    std::uniform_real_distribution<double> doubleDistr(minWeight, maxWeight);

    WeightedGraph::EdgeVector<WeightedEdge> edges;
    edges.reserve(numEdges);

    std::set<VertexPair> unusedEdges;

    for (VertexID endpoint0 = 1; endpoint0 < numVertices; endpoint0++) {
        for (VertexID endpoint1 = 0; endpoint1 < endpoint0; endpoint1++) {
            unusedEdges.insert({ endpoint0, endpoint1 });
        }
    }

    // Ensure that the graph is connected
    for (VertexID endpoint0 = 1; endpoint0 < numVertices; endpoint0++) {
        VertexID endpoint1 = intDistr(randomGenerator) % endpoint0;
        EdgeWeight weight = doubleDistr(randomGenerator);
        edges.emplace_back(endpoint0, endpoint1, weight);
        unusedEdges.erase({ endpoint0, endpoint1 });
    }

    // Add remaining edges
    for (size_t i = numVertices; i < numEdges; i++) {
        int unusedIdx = intDistr(randomGenerator) % unusedEdges.size();
        auto it = unusedEdges.begin();
        std::advance(it, unusedIdx);
        EdgeWeight weight = doubleDistr(randomGenerator);
        edges.emplace_back(it->v0, it->v1, weight);
        unusedEdges.erase(it);
    }

    return WeightedGraph(numVertices, std::move(edges));
}

RootedSpanningTree randomSpanningTree(const WeightedGraph& graph) {
    std::uniform_int_distribution<int> intDistr(0, std::numeric_limits<int>::max());

    std::vector<WeightedGraph::EdgeIndex> edgeSelection;
    edgeSelection.reserve(graph.numVertices() - 1);

    std::vector<WeightedGraph::EdgeIndex> unvisitedEdgeIdxs(graph.numEdges());
    for (size_t i = 0; i < unvisitedEdgeIdxs.size(); ++i) {
        unvisitedEdgeIdxs[i] = WeightedGraph::EdgeIndex(i);
    }

    UnionFind uf(graph.numVertices());
    while (!unvisitedEdgeIdxs.empty() && edgeSelection.size() < graph.numVertices() - 1) {
        int ueiIndex = intDistr(randomGenerator) % unvisitedEdgeIdxs.size();
        WeightedGraph::EdgeIndex edgeIndex = unvisitedEdgeIdxs[ueiIndex];
        const Edge& edge = graph.edge(edgeIndex);

        if (uf.unionSets(edge.endpoint(0), edge.endpoint(1))) {
            edgeSelection.push_back(edgeIndex);
        }
        
        unvisitedEdgeIdxs[ueiIndex] = unvisitedEdgeIdxs.back();
        unvisitedEdgeIdxs.pop_back();
    }
    return RootedSpanningTree(graph, std::move(edgeSelection), 0);
}
