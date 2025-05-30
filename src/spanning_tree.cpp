#include "spanning_tree.hpp"

#include <vector>
#include <algorithm>
#include <numeric>
#include <stdexcept>
#include "union_find.hpp"

template <bool MAX, bool USE_WEIGHTS>
static bool spanningTree(size_t* edgeIndicesBuffer, size_t bufferSize, const MultiGraph& graph, const EdgeWeight* weights, size_t numWeights) {
    const std::vector<MultiEdge>& edges = graph.edges();
    
    if (bufferSize < graph.numVertices() - 1) return false;
    if (USE_WEIGHTS && edges.size() != numWeights) return false;
    if (graph.numVertices() <= 1) return true;

    std::vector<size_t> ordering(edges.size());
    std::iota(ordering.begin(), ordering.end(), 0);
    if constexpr(USE_WEIGHTS) {
        std::sort(ordering.begin(), ordering.end(), [weights](size_t i0, size_t i1) {
            if constexpr(MAX)   return weights[i0] > weights[i1];
            else                return weights[i0] < weights[i1];
        });
    } else {
        std::sort(ordering.begin(), ordering.end(), [&edges](size_t i0, size_t i1) {
            if constexpr(MAX)   return edges[i0].multiplicity() > edges[i1].multiplicity();
            else                return edges[i0].multiplicity() < edges[i1].multiplicity();
        });
    }

    UnionFind uf(graph.numVertices());
    size_t numSelectedEdges = 0;
    for (size_t index : ordering) {
        const MultiEdge& edge = edges[index];

        if (uf.find(edge.endpoint(0)) != uf.find(edge.endpoint(1))) {
            *edgeIndicesBuffer++ = index;
            numSelectedEdges++;

            if (numSelectedEdges == graph.numVertices() - 1) {
                return true;
            }

            uf.unionSets(edge.endpoint(0), edge.endpoint(1));
        }
    }

    return false;
} 


bool minSpanningTree(size_t* edgeIndicesBuffer, size_t bufferSize, const MultiGraph& graph) {
    return spanningTree<false, false>(edgeIndicesBuffer, bufferSize, graph, nullptr, 0);
}

bool minSpanningTree(size_t* edgeIndicesBuffer, size_t bufferSize, const MultiGraph& graph, const EdgeWeight* weights, size_t numWeights) {
    return spanningTree<false, true>(edgeIndicesBuffer, bufferSize, graph, weights, numWeights);
}

bool maxSpanningTree(size_t* edgeIndicesBuffer, size_t bufferSize, const MultiGraph& graph) {
    return spanningTree<true, false>(edgeIndicesBuffer, bufferSize, graph, nullptr, 0);
}

bool maxSpanningTree(size_t* edgeIndicesBuffer, size_t bufferSize, const MultiGraph& graph, const EdgeWeight* weights, size_t numWeights) {
    return spanningTree<true, true>(edgeIndicesBuffer, bufferSize, graph, weights, numWeights);
}
