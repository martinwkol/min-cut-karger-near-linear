#pragma once

#include <vector>
#include <span>
#include "types.hpp"
#include "edge.hpp"
#include "strong_index.hpp"
#include "custom_index_vector.hpp"


// MultiGraph and WeightedGraph are closely related and converted into each other
// => use shared index type
struct GraphEdgeIndexTag {};
using GraphEdgeIndex = StrongIndex<GraphEdgeIndexTag>;

template <typename T>
using GraphEdgeVector = CustomIndexVector<GraphEdgeIndex, T>;

class MultiGraph;

class WeightedGraph {
public:
    using EdgeIndex = GraphEdgeIndex;
    template <typename T>
    using EdgeVector = GraphEdgeVector<T>;

    WeightedGraph(size_t numVertices, std::span<const WeightedEdge> edges);
    WeightedGraph(size_t numVertices, EdgeVector<WeightedEdge>&& edges);

    size_t getNumVertices() const { return mNumVertices; }
    size_t getNumEdges() const { return mEdges.size(); }
    const WeightedEdge& getEdge(EdgeIndex index) const { return mEdges[index]; }
    const EdgeVector<WeightedEdge>& getEdges() const { return mEdges; }

    EdgeWeight getMinEdgeWeight() const;
    EdgeWeight getMaxEdgeWeight() const;

    MultiGraph approxAsMultiGraph(double epsilon) const;

private:
    size_t mNumVertices;
    EdgeVector<WeightedEdge> mEdges;
};


class MultiGraph {
public:
    using EdgeIndex = GraphEdgeIndex;
    template <typename T>
    using EdgeVector = GraphEdgeVector<T>;

    MultiGraph(size_t numVertices, std::span<const MultiEdge> edges);
    MultiGraph(size_t numVertices, EdgeVector<MultiEdge>&& edges);

    size_t getNumVertices() const { return mNumVertices; }
    size_t getNumEdges() const { return mEdges.size(); }
    const MultiEdge& getEdge(EdgeIndex index) const { return mEdges[index]; }
    const EdgeVector<MultiEdge>& getEdges() const { return mEdges; }

    size_t getNumSimpleEdges() const;

    /**
     * Sample each edge with probability sampleProbability and cap the edge-multiplicities to maxEdgeMultiplicity
    */
    void sampleEdges(double sampleProbability, EdgeMultiplicity maxEdgeMultiplicity);

private:
    size_t mNumVertices;
    EdgeVector<MultiEdge> mEdges;
};