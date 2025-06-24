#include "tree_packing.hpp"

#include <cmath>
#include "spanning_tree.hpp"
#include "random.hpp"

TreePacking::TreePacking(const MultiGraph& graph, double epsilon) : mGraph(graph) {
    mEpsilon = epsilon;
    mTreeWeight = epsilon / (std::log(graph.numSimpleEdges()) * 3);

    size_t numEdges = graph.edges().size();
    std::vector<EdgeWeight> loads(numEdges, 0.0);
    std::vector<EdgeMultiplicity> simpleEdgeCounters(numEdges, 0);

    while(true) {
        std::vector<MultiGraph::EdgeIndex> selection(graph.numVertices() - 1);
        if (!minSpanningTree(selection.data(), selection.size(), graph, loads.data(), loads.size())) {
            return;
        }
        for (MultiGraph::EdgeIndex edgeIndex : selection) {
            if (loads[edgeIndex.val] + mTreeWeight > 1.0) {
                return;
            }
            simpleEdgeCounters[edgeIndex.val]++;
            if (simpleEdgeCounters[edgeIndex.val] >= graph.edge(edgeIndex).multiplicity()) {
                simpleEdgeCounters[edgeIndex.val] = 0;
                loads[edgeIndex.val] += mTreeWeight;
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
