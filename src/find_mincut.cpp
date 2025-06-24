#include "find_mincut.hpp"
#include "tree_packing.hpp"
#include "rooted_spanning_tree.hpp"
#include "two_respecting_trees.hpp"
#include "two_respecting_cuts.hpp"

static Cut findSmallestCut(const WeightedGraph& graph, const TreePacking& packing) {
    Cut bestCut;
    bestCut.weight = INFINITE_WEIGHT;
    for (const std::vector<WeightedGraph::EdgeIndex>& edgeSelection : packing.trees()) {
        RootedSpanningTree rst(graph, edgeSelection, 0);
        Cut smallestCut = findSmallest2RespectingCut(rst);
        if (smallestCut.weight < bestCut.weight) {
            bestCut = std::move(smallestCut);
        }
    }
    return bestCut;
}

Cut findLikelyMinCut(const WeightedGraph &graph, double d) {
    TreePacking packing = findTwoRespectingTrees(graph, d);
    return findSmallestCut(graph, packing);
}