#pragma once

#include "types.hpp"
#include "graph.hpp"

/**
 * Compute a minimum cut of the given graph. This function finds the correct minimum cut
 * with probability 1 - O(1 / n^d).
 * 
 * @param graph The graph whose minimum cut is to be computed
 * @param d The exponent in the probability of success
 */
Cut findLikelyMinCut(const WeightedGraph& graph, double d);