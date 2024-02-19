#pragma once

#include <vector>
#include "graph.hpp"

/**
 * Compute the minimum spanning tree with respect to the edge-multiplicities
 * @return the indices of the edges in the spanning tree
*/
std::vector<size_t> minSpanningTree(const MultiGraph& graph);

/**
 * Compute the minimum spanning tree with respect to weights from the weight-list
 * @return the indices of the edges in the spanning tree
*/
std::vector<size_t> minSpanningTree(const MultiGraph& graph, const EdgeWeight* weights, size_t numWeights);

/**
 * Compute the maximum spanning tree with respect to the edge-multiplicities
 * @return the indices of the edges in the spanning tree
*/
std::vector<size_t> maxSpanningTree(const MultiGraph& graph);

/**
 * Compute the maximum spanning tree with respect to weights from the weight-list
 * @return the indices of the edges in the spanning tree
*/
std::vector<size_t> maxSpanningTree(const MultiGraph& graph, const EdgeWeight* weights, size_t numWeights);


