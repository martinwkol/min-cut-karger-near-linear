#include "two_respecting_cuts.hpp"

#include <tuple>
#include <limits>
#include "tree_edge_weighter.hpp"

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

static std::tuple<EdgeWeight, size_t, size_t> findSmallestStrictly2RespectingCut(const RootedSpanningTree& rst, 
    const std::vector<std::vector<RootedSpanningTree::Interval>>& subsequences, 
    const std::vector<std::vector<size_t>>& S_plus, const std::vector<std::vector<size_t>>& S_minus) {
    
    const WeightedGraph& graph = rst.graph();
    TreeEdgeWeighter tew(rst.numEdges());
    std::vector<bool> inSPlus0(rst.numEdges(), false);
    for (size_t edgeIndex : S_plus[0]) {
        tew.nonPathAdd(graph.edge(edgeIndex).weight(), subsequences[edgeIndex]);
        inSPlus0[edgeIndex] = true;
    }
    for (size_t i = 0; i < rst.numEdges(); i++) {
        if (!inSPlus0[i]) {
            tew.pathAdd(graph.edge(i).weight(), subsequences[i]);
        }
    }

    tew.intervalAdd(std::numeric_limits<double>::max(), { 0, 0 });
    double minWeight =  tew.minWeight();
    size_t rstCrossingEdgeIndex1 = 0;
    size_t rstCrossingEdgeIndex2 = tew.minEdgeIndex();
    tew.intervalAdd(-std::numeric_limits<double>::max(), { 0, 0 });
    
    for (size_t i = 1; i < rst.numEdges(); i++) {
        for (size_t edgeIndex : S_plus[i]) {
            double edgeWeight = graph.edge(edgeIndex).weight();
            tew.pathAdd(-edgeWeight, subsequences[edgeIndex]);
            tew.nonPathAdd(edgeWeight, subsequences[edgeIndex]);
        }
        for (size_t edgeIndex : S_minus[i]) {
            double edgeWeight = graph.edge(edgeIndex).weight();
            tew.pathAdd(edgeWeight, subsequences[edgeIndex]);
            tew.nonPathAdd(-edgeWeight, subsequences[edgeIndex]);
        }

        tew.intervalAdd(std::numeric_limits<double>::max(), { i, i });
        if (tew.minWeight() < minWeight) {
            minWeight = tew.minWeight();
            rstCrossingEdgeIndex1 = i;
            rstCrossingEdgeIndex2 = tew.minEdgeIndex();
        }
        tew.intervalAdd(-std::numeric_limits<double>::max(), { i, i });
    }

    return { minWeight, rstCrossingEdgeIndex1, rstCrossingEdgeIndex2 };
}

Cut findSmallest2RespectingCut(const RootedSpanningTree& rst) {
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
            S_minus[subseq.end + 1].push_back(i);
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
    auto smallestStrictly2RespectingCut = findSmallestStrictly2RespectingCut(rst, subsequences, S_plus, S_minus);

    if (std::get<0>(smallest1RespectingCut) <= std::get<0>(smallestStrictly2RespectingCut)) {
        return { rst.cutVerticesFromCrossingEdges({ std::get<1>(smallest1RespectingCut) }), std::get<0>(smallest1RespectingCut) };
    } else {
        return { rst.cutVerticesFromCrossingEdges({ std::get<1>(smallestStrictly2RespectingCut), std::get<2>(smallestStrictly2RespectingCut) }), std::get<0>(smallestStrictly2RespectingCut) };
    }
}