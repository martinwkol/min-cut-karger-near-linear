#include "tree_packing.hpp"

#include <cmath>
#include "spanning_tree.hpp"
#include "random.hpp"

TreePacking::TreePacking(const MultiGraph& graph, double epsilon) : mGraph(graph) {
    mEpsilon = epsilon;
    mTreeWeight = epsilon / (std::log(graph.getNumSimpleEdges()) * 3);

    size_t numEdges = graph.getEdges().size();
    GraphEdgeVector<EdgeWeight> loads(numEdges, 0.0);
    GraphEdgeVector<EdgeMultiplicity> simpleEdgeCounters(numEdges, 0);

    while(true) {
        std::vector<MultiGraph::EdgeIndex> selection(graph.getNumVertices() - 1);
        if (!minSpanningTree(selection, graph, loads)) {
            return;
        }
        for (MultiGraph::EdgeIndex edgeIndex : selection) {
            if (loads[edgeIndex] + mTreeWeight > 1.0) {
                return;
            }
            simpleEdgeCounters[edgeIndex]++;
            if (simpleEdgeCounters[edgeIndex] >= graph.getEdge(edgeIndex).multiplicity()) {
                simpleEdgeCounters[edgeIndex] = 0;
                loads[edgeIndex] += mTreeWeight;
            }
        }
        mTrees.push_back(std::move(selection));
    }
}

void TreePacking::sampleTrees(size_t targetSize) {
    if (targetSize >= mTrees.size()) return;
    for (size_t i = 0; i < targetSize; i++) {
        std::uniform_int_distribution uid(i, mTrees.size() - 1);
        size_t index = uid(randomGenerator);
        std::swap(mTrees[i], mTrees[index]);
    }
    mTrees.resize(targetSize);
}
