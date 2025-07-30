#pragma once

#include <span>
#include "graph.hpp"

/**
 * Compute the minimum spanning tree (MST) of the given graph based on edge multiplicities.
 * @param edgeIndicesBuffer Pre-allocated buffer where the indices of the edges forming 
 *        the MST will be written. The buffer must have space for at least (numVertices - 1) edges.
 * @param graph The input MultiGraph from which the MST will be computed.
 * @return true if the spanning tree was successfully computed.
 *         false if:
 *           - the graph is disconnected,
 *           - size of edgeIndicesBuffer is too small.
 */
bool minSpanningTree(std::span<MultiGraph::EdgeIndex> edgeIndicesBuffer, const MultiGraph& graph);

/**
 * Compute the minimum spanning tree (MST) using explicit edge weights.
 *
 * @param edgeIndicesBuffer Pre-allocated buffer where the indices of the edges forming 
 *        the MST will be written. The buffer must have space for at least (numVertices - 1) edges.
 * @param graph The input MultiGraph to process.
 * @param weights Span of edge weights, where weights[i] corresponds to edge i in the graph.
 * @return true if the spanning tree was successfully computed;
 *         false if:
 *           - the graph is disconnected,
 *           - size of edgeIndicesBuffer is too small,
 *           - numWeights does not match the number of edges in the graph.
 */
bool minSpanningTree(std::span<MultiGraph::EdgeIndex> edgeIndicesBuffer, const MultiGraph& graph, std::span<const EdgeWeight> weights);

/**
 * Compute the maximum spanning tree (MaxST) of the graph using edge multiplicities.
 * 
 * @param edgeIndicesBuffer Pre-allocated buffer where the indices of the edges forming 
 *        the MaxST will be written. The buffer must have space for at least (numVertices - 1) edges.
 * @param bufferSize Number of entries available in edgeIndicesBuffer. Should be at least (numVertices - 1).
 * @param graph The input MultiGraph to process.
 * @return true if the maximum spanning tree was successfully computed;
 *         false if:
 *           - the graph is disconnected,
 *           - size of edgeIndicesBuffer is too small.
 */
bool maxSpanningTree(std::span<MultiGraph::EdgeIndex> edgeIndicesBuffer, const MultiGraph& graph);

/**
 * Compute the maximum spanning tree (MaxST) using explicit edge weights.
 *
 * @param edgeIndicesBuffer Pre-allocated buffer where the indices of the edges forming 
 *        the MaxST will be written. The buffer must have space for at least (numVertices - 1) edges.
 * @param graph The input MultiGraph.
 * @param weights Span of edge weights (type double), where weights[i] corresponds to edge i.
 * @return true if the maximum spanning tree was successfully computed;
 *         false if:
 *           - the graph is disconnected,
 *           - size of edgeIndicesBuffer is too small,
 *           - numWeights does not match the number of edges in the graph.
 */
bool maxSpanningTree(std::span<MultiGraph::EdgeIndex> edgeIndicesBuffer, const MultiGraph& graph, std::span<const EdgeWeight> weights);


