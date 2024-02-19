#pragma once

#include <vector>
#include "graph.hpp"

class TreePacking {
private:
    typedef std::vector<size_t> EdgeSelection;

    const MultiGraph& mGraph;
    std::vector<EdgeSelection> mTrees;
    double mTreeWeight;
    double mPackingWeight;
    double mEpsilon;

public:
    /**
     * Compute a tree packing of the given graph with weight at least (1 - epsilon) times the weight of the largest tree packing.
    */
    TreePacking(const MultiGraph& graph, double epsilon);

    const MultiGraph& graph() const { return mGraph; }
    const std::vector<EdgeSelection>& trees() const { return mTrees; }
    double treeWeight() const { return mTreeWeight; }
    double packingWeight() const { return mPackingWeight; }
    double epsilon() const { return mEpsilon; }
};