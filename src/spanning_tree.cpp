#include "spanning_tree.hpp"

#include <vector>
#include <algorithm>
#include <numeric>
#include <stdexcept>
#include "union_find.hpp"

template <bool MAX, bool USE_WEIGHTS>
static bool spanningTree(MultiGraph::EdgeIndex* edgeIndicesBuffer, size_t bufferSize, const MultiGraph& graph, const EdgeWeight* weights, size_t numWeights) {
    if (bufferSize < graph.numVertices() - 1) return false;
    if (USE_WEIGHTS && graph.numEdges() != numWeights) return false;
    if (graph.numVertices() <= 1) return true;

    std::vector<MultiGraph::EdgeIndex> ordering(graph.numEdges());
    for (size_t i = 0; i < graph.numEdges(); i++) {
        ordering[i] = MultiGraph::EdgeIndex(i);
    }
    if constexpr(USE_WEIGHTS) {
        std::sort(ordering.begin(), ordering.end(), [weights](MultiGraph::EdgeIndex i0, MultiGraph::EdgeIndex i1) {
            if constexpr(MAX)   return weights[i0.get()] > weights[i1.get()];
            else                return weights[i0.get()] < weights[i1.get()];
        });
    } else {
        std::sort(ordering.begin(), ordering.end(), [&graph](MultiGraph::EdgeIndex i0, MultiGraph::EdgeIndex i1) {
            if constexpr(MAX)   return graph.edge(i0).multiplicity() > graph.edge(i1).multiplicity();
            else                return graph.edge(i0).multiplicity() < graph.edge(i1).multiplicity();
        });
    }

    UnionFind uf(graph.numVertices());
    size_t numSelectedEdges = 0;
    for (MultiGraph::EdgeIndex index : ordering) {
        const MultiEdge& edge = graph.edge(index);

        if (uf.unionSets(edge.endpoint(0), edge.endpoint(1))) {
            *edgeIndicesBuffer++ = index;
            numSelectedEdges++;

            if (numSelectedEdges == graph.numVertices() - 1) {
                return true;
            }
        }
    }

    return false;
} 


bool minSpanningTree(MultiGraph::EdgeIndex* edgeIndicesBuffer, size_t bufferSize, const MultiGraph& graph) {
    return spanningTree<false, false>(edgeIndicesBuffer, bufferSize, graph, nullptr, 0);
}

bool minSpanningTree(MultiGraph::EdgeIndex* edgeIndicesBuffer, size_t bufferSize, const MultiGraph& graph, const EdgeWeight* weights, size_t numWeights) {
    return spanningTree<false, true>(edgeIndicesBuffer, bufferSize, graph, weights, numWeights);
}

bool maxSpanningTree(MultiGraph::EdgeIndex* edgeIndicesBuffer, size_t bufferSize, const MultiGraph& graph) {
    return spanningTree<true, false>(edgeIndicesBuffer, bufferSize, graph, nullptr, 0);
}

bool maxSpanningTree(MultiGraph::EdgeIndex* edgeIndicesBuffer, size_t bufferSize, const MultiGraph& graph, const EdgeWeight* weights, size_t numWeights) {
    return spanningTree<true, true>(edgeIndicesBuffer, bufferSize, graph, weights, numWeights);
}
