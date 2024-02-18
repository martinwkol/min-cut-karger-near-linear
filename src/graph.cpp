#include "graph.hpp"

WeightedGraph::WeightedGraph(size_t numVertices, const WeightedEdge* edges, size_t numEdges)
    : mNumVertices(numVertices), mEdges(edges, edges + numEdges)
{
}

WeightedGraph::WeightedGraph(size_t numVertices, std::vector<WeightedEdge>&& edges)
    : mNumVertices(numVertices), mEdges(std::move(edges))
{
}

MultiGraph::MultiGraph(size_t numVertices, const MultiEdge* edges, size_t numEdges)
    : mNumVertices(numVertices), mEdges(edges, edges + numEdges)
{
}

MultiGraph::MultiGraph(size_t numVertices, std::vector<MultiEdge>&& edges)
    : mNumVertices(numVertices), mEdges(std::move(edges))
{
}
