#include "tree_edge_weighter.hpp"

void TreeEdgeWeighter::nonIntervalAdd(EdgeWeight weight, const RootedSpanningTree::EdgeInterval& interval) {
    intervalAdd(-weight, interval);
    mGlobalWeight += weight;
}

void TreeEdgeWeighter::pathAdd(EdgeWeight weight, const std::vector<RootedSpanningTree::EdgeInterval>& pathIntervals) {
    for (const RootedSpanningTree::EdgeInterval& interval : pathIntervals) {
        mTree.intervalAdd(weight, { interval.start.get(), interval.end.get() } );
    }
}

void TreeEdgeWeighter::nonPathAdd(EdgeWeight weight, const std::vector<RootedSpanningTree::EdgeInterval>& pathIntervals) {
    pathAdd(-weight, pathIntervals);
    mGlobalWeight += weight;
}