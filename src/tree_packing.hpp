#pragma once

#include <vector>
#include "graph.hpp"

class TreePacking {
public:
    typedef std::vector<GraphEdgeIndex> EdgeSelection;

    /**
     * Compute a tree packing of the given graph with weight at least (1 - epsilon) times the weight of the largest tree packing.
    */
    TreePacking(const MultiGraph& graph, double epsilon);

    const MultiGraph& getGraph() const { return mGraph; }
    const std::vector<EdgeSelection>& getTrees() const { return mTrees; }
    double getTreeWeight() const { return mTreeWeight; }
    double getPackingWeight() const { return mTreeWeight * mTrees.size(); }
    double getEpsilon() const { return mEpsilon; }

    /**
     * Reduce size of packing to targetSize by randomly sampling trees
    */
    void sampleTrees(size_t targetSize);

private:
    const MultiGraph& mGraph;
    std::vector<EdgeSelection> mTrees;
    double mTreeWeight;
    double mEpsilon;
};