#pragma once

#include <vector>
#include "types.hpp"
#include "edge.hpp"
#include "edge_index_makro.hpp"
#include "edge_index_vector.hpp"


// MultiGraph and WeightedGraph closely related and converted into each other
// => use shared index type
DEFINE_EDGE_INDEX_STRUCT(GraphEdgeIndex);

template <typename _Ty>
using GraphEdgeVector = EdgeIndexVector<GraphEdgeIndex, _Ty>;

class MultiGraph;

class WeightedGraph {
public:
    using EdgeIndex = GraphEdgeIndex;
    template <typename _Ty>
    using EdgeVector = GraphEdgeVector<_Ty>;

    WeightedGraph(size_t numVertices, const WeightedEdge* edges, size_t numEdges);
    WeightedGraph(size_t numVertices, EdgeVector<WeightedEdge>&& edges);

    size_t numVertices() const { return mNumVertices; }
    size_t numEdges() const { return mEdges.size(); }
    const WeightedEdge& edge(EdgeIndex index) const { return mEdges[index]; }
    const EdgeVector<WeightedEdge>& edges() const { return mEdges; }

    EdgeWeight minEdgeWeight() const;
    EdgeWeight maxEdgeWeight() const;

    MultiGraph approxAsMultiGraph(double epsilon) const;

private:
    size_t mNumVertices;
    EdgeVector<WeightedEdge> mEdges;
};


class MultiGraph {
public:
    using EdgeIndex = GraphEdgeIndex;
    template <typename _Ty>
    using EdgeVector = GraphEdgeVector<_Ty>;

    MultiGraph(size_t numVertices, const MultiEdge* edges, size_t numEdges);
    MultiGraph(size_t numVertices, EdgeVector<MultiEdge>&& edges);

    size_t numVertices() const { return mNumVertices; }
    size_t numEdges() const { return mEdges.size(); }
    const MultiEdge& edge(EdgeIndex index) const { return mEdges[index]; }
    const EdgeVector<MultiEdge>& edges() const { return mEdges; }

    size_t numSimpleEdges() const;

    /**
     * Sample each edge with probability sampleProbability and cap the edge-multiplicities to maxEdgeMultiplicity
    */
    void sampleEdges(double sampleProbability, EdgeMultiplicity maxEdgeMultiplicity);

private:
    size_t mNumVertices;
    EdgeVector<MultiEdge> mEdges;
};