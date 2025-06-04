#pragma once

#include "graph.hpp"
#include <vector>
#include <utility>

/**
 * Find the minimum cut of the given graph using the stoer-wagner algorithm
 * @param graph the weighted graph whose minimum cut is to be found
 * @return a pair of a vector of vertex indices from one side of the cut and the weight of the cut
*/
std::pair<std::vector<VertexID>, EdgeWeight> stoerWagner(const WeightedGraph& graph);