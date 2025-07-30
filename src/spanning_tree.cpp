#include "spanning_tree.hpp"

#include <vector>
#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <cassert>
#include "union_find.hpp"

template <typename Compare>
static bool spanningTreeTemplate(std::span<MultiGraph::EdgeIndex> edgeIndicesBuffer, const MultiGraph& graph, Compare&& less) {
    assert(edgeIndicesBuffer.size() >= graph.numVertices() - 1);    
    if (graph.numVertices() <= 1) return true;

    std::vector<MultiGraph::EdgeIndex> ordering(graph.numEdges());
    std::iota(ordering.begin(), ordering.end(), MultiGraph::EdgeIndex(0));
    std::sort(ordering.begin(), ordering.end(), less);

    UnionFind uf(graph.numVertices());
    size_t numSelected = 0;
    auto outIt = edgeIndicesBuffer.begin();
    for (MultiGraph::EdgeIndex index : ordering) {
        const MultiEdge& edge = graph.edge(index);

        if (uf.unionSets(edge.endpoint(0), edge.endpoint(1))) {
            *outIt++ = index;
            numSelected++;

            if (numSelected == graph.numVertices() - 1) {
                return true;
            }
        }
    }

    return false;
} 


// Multiplicity-based comparison (increasing/decreasing)
static auto edgeMultiplicityLess(const MultiGraph& graph) {
    return [&graph](MultiGraph::EdgeIndex i, MultiGraph::EdgeIndex j) {
        return graph.edge(i).multiplicity() < graph.edge(j).multiplicity();
    };
}

static auto edgeMultiplicityGreater(const MultiGraph& graph) {
    return [&graph](MultiGraph::EdgeIndex i, MultiGraph::EdgeIndex j) {
        return graph.edge(i).multiplicity() > graph.edge(j).multiplicity();
    };
}

// Weight-based comparison (increasing/decreasing)
static auto weightLess(std::span<const EdgeWeight> weights) {
    return [weights](MultiGraph::EdgeIndex i, MultiGraph::EdgeIndex j) {
        return weights[i.get()] < weights[j.get()];
    };
}

static auto weightGreater(std::span<const EdgeWeight> weights) {
    return [weights](MultiGraph::EdgeIndex i, MultiGraph::EdgeIndex j) {
        return weights[i.get()] > weights[j.get()];
    };
}


bool minSpanningTree(std::span<MultiGraph::EdgeIndex> edgeIndicesBuffer, const MultiGraph& graph) {
    return spanningTreeTemplate(edgeIndicesBuffer, graph, edgeMultiplicityLess(graph));
}

bool minSpanningTree(std::span<MultiGraph::EdgeIndex> edgeIndicesBuffer, const MultiGraph& graph, std::span<const EdgeWeight> weights) {
    return spanningTreeTemplate(edgeIndicesBuffer, graph, weightLess(weights));
}

bool maxSpanningTree(std::span<MultiGraph::EdgeIndex> edgeIndicesBuffer, const MultiGraph& graph) {
    return spanningTreeTemplate(edgeIndicesBuffer, graph, edgeMultiplicityGreater(graph));
}

bool maxSpanningTree(std::span<MultiGraph::EdgeIndex> edgeIndicesBuffer, const MultiGraph& graph, std::span<const EdgeWeight> weights) {
    return spanningTreeTemplate(edgeIndicesBuffer, graph, weightGreater(weights));
}
