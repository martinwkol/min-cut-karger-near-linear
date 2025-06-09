#include "graph.hpp"
#include "rooted_spanning_tree.hpp"

WeightedGraph randomConnectedWeightedGraph(size_t numVertices, size_t numEdges, EdgeWeight minWeight, EdgeWeight maxWeight);
RootedSpanningTree randomSpanningTree(const WeightedGraph& graph);