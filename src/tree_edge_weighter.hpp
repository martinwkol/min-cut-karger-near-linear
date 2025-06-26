#pragma once

#include <utility>

#include "segment_tree.hpp"
#include "rooted_spanning_tree.hpp"

class TreeEdgeWeighter {
public:
    explicit TreeEdgeWeighter(size_t numEdges) : mTree(numEdges), mGlobalWeight(0) {}

    size_t numEdges() const { return mTree.listSize(); }
    EdgeWeight minWeight() const { return mTree.minValue() + mGlobalWeight; }
    RootedSpanningTree::EdgeIndex minEdgeIndex() { return RootedSpanningTree::EdgeIndex(mTree.minIndex()); }

    void intervalAdd(EdgeWeight weight, const RootedSpanningTree::EdgeInterval& interval) 
        { mTree.intervalAdd(weight, { interval.start.get(), interval.end.get() }); }
    void nonIntervalAdd(EdgeWeight weight, const RootedSpanningTree::EdgeInterval& interval);
    void pathAdd(EdgeWeight weight, const std::vector<RootedSpanningTree::EdgeInterval>& pathIntervals);
    void nonPathAdd(EdgeWeight weight, const std::vector<RootedSpanningTree::EdgeInterval>& pathIntervals);

private:
    SegmentTree mTree;
    EdgeWeight mGlobalWeight;
};