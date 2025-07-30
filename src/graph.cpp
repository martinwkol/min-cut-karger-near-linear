#include "graph.hpp"

#include <algorithm>
#include <numeric>
#include <cmath>
#include "random.hpp"

WeightedGraph::WeightedGraph(size_t numVertices, std::span<const WeightedEdge> edges)
    : mNumVertices(numVertices), mEdges(edges.begin(), edges.end())
{
}

WeightedGraph::WeightedGraph(size_t numVertices, EdgeVector<WeightedEdge>&& edges)
    : mNumVertices(numVertices), mEdges(std::move(edges))
{
}

EdgeWeight WeightedGraph::minEdgeWeight() const { 
    return std::min_element(mEdges.begin(), mEdges.end(), [](const WeightedEdge& e0, const WeightedEdge& e1) {
        return e0.weight() < e1.weight();
    })->weight();
}

EdgeWeight WeightedGraph::maxEdgeWeight() const {
    return std::max_element(mEdges.begin(), mEdges.end(), [](const WeightedEdge& e0, const WeightedEdge& e1) {
        return e0.weight() < e1.weight();
    })->weight();
}

MultiGraph WeightedGraph::approxAsMultiGraph(double epsilon) const {
    EdgeVector<MultiEdge> multiedges;
    multiedges.reserve(mEdges.size());

    EdgeWeight multiplicator = 1 / (minEdgeWeight() * epsilon);
    for (const WeightedEdge& edge : mEdges) {
        multiedges.emplace_back(edge.endpoint(0), edge.endpoint(1), std::ceil(edge.weight() * multiplicator));
    }

    return MultiGraph(mNumVertices, std::move(multiedges));    
}

MultiGraph::MultiGraph(size_t numVertices, std::span<const MultiEdge> edges)
    : mNumVertices(numVertices), mEdges(edges.begin(), edges.end())
{
}

MultiGraph::MultiGraph(size_t numVertices, EdgeVector<MultiEdge>&& edges)
    : mNumVertices(numVertices), mEdges(std::move(edges))
{
}

size_t MultiGraph::numSimpleEdges() const {
    return std::accumulate(mEdges.begin(), mEdges.end(), size_t(0), [](size_t m, const MultiEdge& edge) {
        return m + edge.multiplicity();
    });
}

void MultiGraph::sampleEdges(double sampleProbability, EdgeMultiplicity maxEdgeMultiplicity) {
    for (MultiEdge& edge : mEdges) {
        edge.setMultiplicity(binom(edge.multiplicity(), sampleProbability, maxEdgeMultiplicity));
    }
}
