#pragma once

#include "graph.hpp"
#include "tree_packing.hpp"

/**
 * Finds a tree packing of which the graphs minimum cut 2-respects at least one tree with probability 1 - O(1/n^d)
 * @param d the exponent in the probability of success
 * @param eps1 epsilon for multigraph-approximation
 * @param eps2 epsilon for edge-sampling
 * @param eps3 epsilon for tree-packing
*/
TreePacking findTwoRespectingTrees(const WeightedGraph& graph, double d, double eps1 = 0.001, double eps2 = 1.0 / 9, double eps3 = 1.0 / 8);