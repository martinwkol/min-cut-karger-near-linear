#include "two_respecting_cuts.hpp"

#include <tuple>

static std::tuple<EdgeWeight, size_t> findSmallest1RespectingCut(const RootedSpanningTree& rst, const std::vector<std::vector<size_t>>& S_plus, const std::vector<std::vector<size_t>>& S_minus) {
    const WeightedGraph& graph = rst.graph();
    
    EdgeWeight weight = 0;
    for (size_t edgeIndex : S_plus[0]) {
        weight += graph.edge(edgeIndex).weight();
    }

    double minWeight = weight;
    size_t rstCrossingEdgeIndex = 0;
    for (size_t i = 1; i < rst.numEdges(); i++) {
        for (size_t edgeIndex : S_plus[i]) {
            weight += graph.edge(edgeIndex).weight();
        }
        for (size_t edgeIndex : S_minus[i]) {
            weight -= graph.edge(edgeIndex).weight();
        }
        if (weight < minWeight) {
            minWeight = weight;
            rstCrossingEdgeIndex = i;
        }
    }

    return { minWeight, rstCrossingEdgeIndex };
}

std::vector<size_t> findSmallest2RespectingCut(const RootedSpanningTree& rst) {
    const WeightedGraph& graph = rst.graph();

    std::vector<std::vector<RootedSpanningTree::Interval>> subsequences(graph.numEdges());
    std::vector<std::vector<size_t>> S_plus(graph.numVertices());
    std::vector<std::vector<size_t>> S_minus(graph.numVertices());
    
    const std::vector<WeightedEdge>& graphEdges = graph.edges();
    for (size_t i = 0; i < graphEdges.size(); i++) {
        const WeightedEdge& edge = graphEdges[i];
        subsequences.push_back(rst.findVertex2VertexSubsequences(edge.endpoint(0), edge.endpoint(1)));
        for (const RootedSpanningTree::Interval& subseq : subsequences.back()) {
            S_plus[subseq.start].push_back(i);
            S_minus[subseq.end].push_back(i);
        }
        for (const RootedSpanningTree::Interval& subseq : subsequences.back()) {
            if (!S_plus[subseq.start].empty() && !S_minus[subseq.start].empty() && 
                S_plus[subseq.start].back() == i && S_minus[subseq.start].back() == i) {
                
                S_plus[subseq.start].pop_back();
                S_minus[subseq.start].pop_back();
            }
        }
    }

    auto smallest1RespectingCut = findSmallest1RespectingCut(rst, S_plus, S_minus);

}