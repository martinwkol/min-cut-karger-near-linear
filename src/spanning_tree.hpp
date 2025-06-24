#pragma once

#include <vector>
#include "graph.hpp"

/**
 * Compute the minimum spanning tree (MST) of the given graph based on edge multiplicities.
 * @param edgeIndicesBuffer Pointer to a pre-allocated buffer where the indices of the edges forming 
 *        the MST will be written. The buffer must have space for at least (numVertices - 1) edges.
 * @param bufferSize The number of entries available in edgeIndicesBuffer.
 * @param graph The input MultiGraph from which the MST will be computed.
 * @return true if the spanning tree was successfully computed.
 *         false if:
 *           - the graph is disconnected,
 *           - the bufferSize is too small.
 */
bool minSpanningTree(MultiGraph::EdgeIndex* edgeIndicesBuffer, size_t bufferSize, const MultiGraph& graph);

/**
 * Compute the minimum spanning tree (MST) using explicit edge weights.
 *
 * @param edgeIndicesBuffer Pointer to a pre-allocated buffer to store the indices of the MST edges.
 *        Must hold at least (numVertices - 1) entries.
 * @param bufferSize Number of entries available in edgeIndicesBuffer.
 * @param graph The input MultiGraph to process.
 * @param weights Array of edge weights, where weights[i] corresponds to edge i in the graph.
 * @param numWeights The number of weights provided. Must exactly match the number of edges in the graph.
 * @return true if the spanning tree was successfully computed;
 *         false if:
 *           - the graph is disconnected,
 *           - bufferSize is too small,
 *           - numWeights does not match the number of edges in the graph.
 */
bool minSpanningTree(MultiGraph::EdgeIndex* edgeIndicesBuffer, size_t bufferSize, const MultiGraph& graph, const EdgeWeight* weights, size_t numWeights);

/**
 * Compute the maximum spanning tree (MaxST) of the graph using edge multiplicities.
 *
 * @param edgeIndicesBuffer Pointer to a pre-allocated buffer to store the indices of the MaxST edges.
 * @param bufferSize Number of entries available in edgeIndicesBuffer. Should be at least (numVertices - 1).
 * @param graph The input MultiGraph to process.
 * @return true if the maximum spanning tree was successfully computed;
 *         false if:
 *           - the graph is disconnected,
 *           - bufferSize is too small.
 */
bool maxSpanningTree(MultiGraph::EdgeIndex* edgeIndicesBuffer, size_t bufferSize, const MultiGraph& graph);

/**
 * Compute the maximum spanning tree (MaxST) using explicit edge weights.
 *
 * @param edgeIndicesBuffer Pointer to a pre-allocated buffer to store the indices of the MaxST edges.
 * @param bufferSize Number of entries available in edgeIndicesBuffer.
 * @param graph The input MultiGraph.
 * @param weights Array of edge weights (type double), where weights[i] corresponds to edge i.
 * @param numWeights The number of weights provided. Must match the number of edges in the graph.
 * @return true if the maximum spanning tree was successfully computed;
 *         false if:
 *           - the graph is disconnected,
 *           - bufferSize is too small,
 *           - numWeights does not match the number of edges in the graph.
 */
bool maxSpanningTree(MultiGraph::EdgeIndex* edgeIndicesBuffer, size_t bufferSize, const MultiGraph& graph, const EdgeWeight* weights, size_t numWeights);


