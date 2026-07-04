#pragma once

#include <utility>

#include "segment_tree.hpp"
#include "rooted_spanning_tree.hpp"

class TreeEdgeWeighter {
public:
    explicit TreeEdgeWeighter(size_t numEdges) : mTree(numEdges), mGlobalWeight(0) {}

    size_t getNumEdges() const { return mTree.getListSize(); }
    EdgeWeight getMinWeight() const { return mTree.getMinValue() + mGlobalWeight; }
    RootedSpanningTree::EdgeIndex getMinEdgeIndex() { return RootedSpanningTree::EdgeIndex(mTree.getMinIndex()); }

    void intervalAdd(EdgeWeight weight, const RootedSpanningTree::EdgeInterval& interval) 
        { mTree.intervalAdd(weight, { interval.start.get(), interval.end.get() }); }
    void nonIntervalAdd(EdgeWeight weight, const RootedSpanningTree::EdgeInterval& interval);
    void pathAdd(EdgeWeight weight, const std::vector<RootedSpanningTree::EdgeInterval>& pathIntervals);
    void nonPathAdd(EdgeWeight weight, const std::vector<RootedSpanningTree::EdgeInterval>& pathIntervals);

private:
    SegmentTree mTree;
    EdgeWeight mGlobalWeight;
};