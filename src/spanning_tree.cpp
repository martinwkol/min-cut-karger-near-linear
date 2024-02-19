#include "spanning_tree.hpp"

#include <vector>
#include <algorithm>
#include <numeric>
#include <stdexcept>
#include "union_find.hpp"

template <bool MAX_SPANNING_TREE, bool USE_WEIGHTS>
static bool spanningTree(size_t* edgeIndices, size_t edgeIndicesSize, const MultiGraph& graph, const EdgeWeight* weights, size_t numWeights) {
    if (graph.numVertices() <= 1) {
        return true;
    }
    if (edgeIndicesSize < graph.numVertices() - 1) {
        throw std::runtime_error("edgeIndices too small");
    }

    const std::vector<MultiEdge>& edges = graph.edges();
    const std::vector<MultiEdge>* edgesPtr = &edges;
    

    if constexpr(USE_WEIGHTS) {
        if (edges.size() != numWeights) {
            throw std::runtime_error("the number of edges does not match the number of edges");
        }
    }

    std::vector<size_t> sortedIndices(edges.size());
    std::iota(sortedIndices.begin(), sortedIndices.end(), 0);

    if constexpr(USE_WEIGHTS && MAX_SPANNING_TREE) {
        std::sort(sortedIndices.begin(), sortedIndices.end(), [weights](size_t i0, size_t i1) {
            return weights[i0] > weights[i1];
        });
    }
    if constexpr(USE_WEIGHTS && !MAX_SPANNING_TREE) {
        std::sort(sortedIndices.begin(), sortedIndices.end(), [weights](size_t i0, size_t i1) {
            return weights[i0] < weights[i1];
        });
    }
    if constexpr(!USE_WEIGHTS && MAX_SPANNING_TREE) {
        std::sort(sortedIndices.begin(), sortedIndices.end(), [edgesPtr](size_t i0, size_t i1) {
            return (*edgesPtr)[i0].multiplicity() > (*edgesPtr)[i1].multiplicity();
        });
    }
    if constexpr(!USE_WEIGHTS && !MAX_SPANNING_TREE) {
        std::sort(sortedIndices.begin(), sortedIndices.end(), [edgesPtr](size_t i0, size_t i1) {
            return (*edgesPtr)[i0].multiplicity() < (*edgesPtr)[i1].multiplicity();
        });
    }

    UnionFind uf(graph.numVertices());
    size_t numSelectedEdges = 0;
    for (size_t index : sortedIndices) {
        const MultiEdge& edge = edges[index];

        if (uf.find(edge.endpoint(0)) != uf.find(edge.endpoint(1))) {
            *edgeIndices++ = index;
            numSelectedEdges++;

            if (numSelectedEdges == graph.numVertices() - 1) {
                return true;
            }

            uf.unionSets(edge.endpoint(0), edge.endpoint(1));
        }
    }

    throw false;
} 

bool minSpanningTree(size_t* edgeIndices, size_t edgeIndicesSize, const MultiGraph& graph) {
    return spanningTree<false, false>(edgeIndices, edgeIndicesSize, graph, nullptr, 0);
}

bool minSpanningTree(size_t* edgeIndices, size_t edgeIndicesSize, const MultiGraph& graph, const EdgeWeight* weights, size_t numWeights) {
    return spanningTree<false, true>(edgeIndices, edgeIndicesSize, graph, weights, numWeights);
}

bool maxSpanningTree(size_t* edgeIndices, size_t edgeIndicesSize, const MultiGraph& graph) {
    return spanningTree<true, false>(edgeIndices, edgeIndicesSize, graph, nullptr, 0);
}

bool maxSpanningTree(size_t* edgeIndices, size_t edgeIndicesSize, const MultiGraph& graph, const EdgeWeight* weights, size_t numWeights) {
    return spanningTree<true, true>(edgeIndices, edgeIndicesSize, graph, weights, numWeights);
}
