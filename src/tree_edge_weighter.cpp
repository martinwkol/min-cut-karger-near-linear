#include "tree_edge_weighter.hpp"

void TreeEdgeWeighter::nonIntervalAdd(EdgeWeight weight, const Interval& interval) {
    intervalAdd(-weight, interval);
    mGlobalWeight += weight;
}

void TreeEdgeWeighter::pathAdd(EdgeWeight weight, const std::vector<Interval>& pathIntervals) {
    for (const Interval& interval : pathIntervals) {
        mTree.intervalAdd(weight, interval.start, interval.end);
    }
}

void TreeEdgeWeighter::nonPathAdd(EdgeWeight weight, const std::vector<Interval>& pathIntervals) {
    pathAdd(-weight, pathIntervals);
    mGlobalWeight += weight;
}