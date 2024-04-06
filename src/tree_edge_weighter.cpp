#include "tree_edge_weighter.hpp"

void TreeEdgeWeighter::nonIntervalAdd(EdgeWeight weight, const RootedSpanningTree::Interval& interval) {
    intervalAdd(-weight, interval);
    mGlobalWeight += weight;
}

void TreeEdgeWeighter::pathAdd(EdgeWeight weight, const std::vector<RootedSpanningTree::Interval>& pathIntervals) {
    for (const RootedSpanningTree::Interval& interval : pathIntervals) {
        mTree.intervalAdd(weight, interval.start, interval.end);
    }
}

void TreeEdgeWeighter::nonPathAdd(EdgeWeight weight, const std::vector<RootedSpanningTree::Interval>& pathIntervals) {
    pathAdd(-weight, pathIntervals);
    mGlobalWeight += weight;
}