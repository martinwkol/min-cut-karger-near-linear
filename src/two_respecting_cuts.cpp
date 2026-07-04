#include "two_respecting_cuts.hpp"

#include <tuple>
#include <limits>
#include "tree_edge_weighter.hpp"


struct S1RespResult {
    EdgeWeight weight;
    RootedSpanningTree::EdgeIndex cutEdgeIndex;
};

struct S2RespResult {
    EdgeWeight weight;
    RootedSpanningTree::EdgeIndex cutEdgeIndex1;
    RootedSpanningTree::EdgeIndex cutEdgeIndex2;
};

static S1RespResult findSmallest1RespectingCut(
    const RootedSpanningTree& rst, 
    const RootedSpanningTree::EdgeVector<std::vector<WeightedGraph::EdgeIndex>>& S_plus, 
    const RootedSpanningTree::EdgeVector<std::vector<WeightedGraph::EdgeIndex>>& S_minus
) {       
    const WeightedGraph& graph = rst.getGraph();
    EdgeWeight weight = 0;
    for (WeightedGraph::EdgeIndex edgeIndex : S_plus[RootedSpanningTree::EdgeIndex(0)]) {
        weight += graph.getEdge(edgeIndex).getWeight();
    }

    double minWeight = weight;
    RootedSpanningTree::EdgeIndex rstCrossingEdgeIndex(0);
    for (RootedSpanningTree::EdgeIndex i(1); i < rst.getNumEdges(); ++i) {
        for (WeightedGraph::EdgeIndex edgeIndex : S_plus[i]) {
            weight += graph.getEdge(edgeIndex).getWeight();
        }
        for (WeightedGraph::EdgeIndex edgeIndex : S_minus[i]) {
            weight -= graph.getEdge(edgeIndex).getWeight();
        }
        if (weight < minWeight) {
            minWeight = weight;
            rstCrossingEdgeIndex = i;
        }
    }

    return { minWeight, rstCrossingEdgeIndex };
}

static S2RespResult findSmallestStrictly2RespectingCut(
    const RootedSpanningTree& rst, 
    const WeightedGraph::EdgeVector<std::vector<RootedSpanningTree::EdgeInterval>>& subsequences, 
    const RootedSpanningTree::EdgeVector<std::vector<WeightedGraph::EdgeIndex>>& S_plus, 
    const RootedSpanningTree::EdgeVector<std::vector<WeightedGraph::EdgeIndex>>& S_minus
) {
    const WeightedGraph& graph = rst.getGraph();
    TreeEdgeWeighter tew(rst.getNumEdges());
    WeightedGraph::EdgeVector<bool> inSPlus0(graph.getNumEdges(), false);
    for (WeightedGraph::EdgeIndex edgeIndex : S_plus[RootedSpanningTree::EdgeIndex(0)]) {
        tew.nonPathAdd(graph.getEdge(edgeIndex).getWeight(), subsequences[edgeIndex]);
        inSPlus0[edgeIndex] = true;
    }
    for (WeightedGraph::EdgeIndex edgeIndex(0); edgeIndex < graph.getNumEdges(); ++edgeIndex) {
        if (!inSPlus0[edgeIndex]) {
            tew.pathAdd(graph.getEdge(edgeIndex).getWeight(), subsequences[edgeIndex]);
        }
    }

    tew.intervalAdd(std::numeric_limits<double>::max(), 
        { RootedSpanningTree::EdgeIndex(0), RootedSpanningTree::EdgeIndex(0) }
    );
    double minWeight =  tew.getMinWeight();
    RootedSpanningTree::EdgeIndex rstCrossingEdgeIndex1(0);
    RootedSpanningTree::EdgeIndex rstCrossingEdgeIndex2 = tew.getMinEdgeIndex();
        tew.intervalAdd(std::numeric_limits<double>::max(), 
        { RootedSpanningTree::EdgeIndex(0), RootedSpanningTree::EdgeIndex(0) }
    );
    
    for (RootedSpanningTree::EdgeIndex i(1); i < rst.getNumEdges(); ++i) {
        for (WeightedGraph::EdgeIndex edgeIndex : S_plus[i]) {
            double edgeWeight = graph.getEdge(edgeIndex).getWeight();
            tew.pathAdd(-edgeWeight, subsequences[edgeIndex]);
            tew.nonPathAdd(edgeWeight, subsequences[edgeIndex]);
        }
        for (WeightedGraph::EdgeIndex edgeIndex : S_minus[i]) {
            double edgeWeight = graph.getEdge(edgeIndex).getWeight();
            tew.pathAdd(edgeWeight, subsequences[edgeIndex]);
            tew.nonPathAdd(-edgeWeight, subsequences[edgeIndex]);
        }

        tew.intervalAdd(std::numeric_limits<double>::max(), { i, i });
        if (tew.getMinWeight() < minWeight) {
            minWeight = tew.getMinWeight();
            rstCrossingEdgeIndex1 = i;
            rstCrossingEdgeIndex2 = tew.getMinEdgeIndex();
        }
        tew.intervalAdd(-std::numeric_limits<double>::max(), { i, i });
    }

    return { minWeight, rstCrossingEdgeIndex1, rstCrossingEdgeIndex2 };
}

Cut findSmallest2RespectingCut(const RootedSpanningTree& rst) {
    const WeightedGraph& graph = rst.getGraph();

    WeightedGraph::EdgeVector<std::vector<RootedSpanningTree::EdgeInterval>> subsequences;
    RootedSpanningTree::EdgeVector<std::vector<WeightedGraph::EdgeIndex>> S_plus(graph.getNumVertices());
    RootedSpanningTree::EdgeVector<std::vector<WeightedGraph::EdgeIndex>> S_minus(graph.getNumVertices());
    subsequences.reserve(graph.getNumEdges());
    
    const WeightedGraph::EdgeVector<WeightedEdge>& graphEdges = graph.getEdges();
    for (WeightedGraph::EdgeIndex i(0); i < graphEdges.size(); i++) {
        const WeightedEdge& edge = graphEdges[i];
        subsequences.push_back(rst.findVertex2VertexSubsequences(edge.endpoint(0), edge.endpoint(1)));
        for (const RootedSpanningTree::EdgeInterval& subseq : subsequences.back()) {
            S_plus[subseq.start].push_back(i);
            S_minus[subseq.end + 1].push_back(i);
        }
        for (const RootedSpanningTree::EdgeInterval& subseq : subsequences.back()) {
            if (!S_plus[subseq.start].empty() && !S_minus[subseq.start].empty() && 
                S_plus[subseq.start].back() == i && S_minus[subseq.start].back() == i) {
                
                S_plus[subseq.start].pop_back();
                S_minus[subseq.start].pop_back();
            }
        }
    }

    const S1RespResult s1RespResult = findSmallest1RespectingCut(rst, S_plus, S_minus);
    const S2RespResult s2RespResult = findSmallestStrictly2RespectingCut(rst, subsequences, S_plus, S_minus);

    if (s1RespResult.weight <= s2RespResult.weight) {
        Cut minCut = {
            rst.cutVerticesFromCrossingEdges({ s1RespResult.cutEdgeIndex }),
            s1RespResult.weight
        };
        return minCut;
    } else {
        Cut minCut = {
            rst.cutVerticesFromCrossingEdges({ s2RespResult.cutEdgeIndex1, s2RespResult.cutEdgeIndex2 }),
            s2RespResult.weight
        };
        return minCut;
    }
}