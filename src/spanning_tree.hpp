#pragma once

#include <vector>
#include "graph.hpp"

/**
 * Compute the minimum spanning tree with respect to the edge-multiplicities
 * @param edgeIndices pointer to the location where the indices of the edges in the spanning tree will be stored
 * @return true if success, false if graph disconnected
*/
bool minSpanningTree(size_t* edgeIndices, size_t edgeIndicesSize, const MultiGraph& graph);

/**
 * Compute the minimum spanning tree with respect to weights from the weight-list
 * @param edgeIndices pointer to the location where the indices of the edges in the spanning tree will be stored
 * @return true if success, false if graph disconnected
*/
bool minSpanningTree(size_t* edgeIndices, size_t edgeIndicesSize, const MultiGraph& graph, const EdgeWeight* weights, size_t numWeights);

/**
 * Compute the maximum spanning tree with respect to the edge-multiplicities
 * @param edgeIndices pointer to the location where the indices of the edges in the spanning tree will be stored
 * @return true if success, false if graph disconnected
*/
bool maxSpanningTree(size_t* edgeIndices, size_t edgeIndicesSize, const MultiGraph& graph);

/**
 * Compute the maximum spanning tree with respect to weights from the weight-list
 * @param edgeIndices pointer to the location where the indices of the edges in the spanning tree will be stored
 * @return true if success, false if graph disconnected
*/
bool maxSpanningTree(size_t* edgeIndices, size_t edgeIndicesSize, const MultiGraph& graph, const EdgeWeight* weights, size_t numWeights);


