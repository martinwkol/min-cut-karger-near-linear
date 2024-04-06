#include "two_respecting_cuts.hpp"

std::vector<size_t> findSmallest2RespectingCut(const RootedSpanningTree& rst) {
    std::vector<std::vector<RootedSpanningTree::Interval>> subsequences(rst.graph().numEdges());
    std::vector<std::vector<size_t>> S_plus(rst.graph().numVertices());
    std::vector<std::vector<size_t>> S_minus(rst.graph().numVertices());
    
    const std::vector<WeightedEdge>& graphEdges = rst.graph().edges();
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
}