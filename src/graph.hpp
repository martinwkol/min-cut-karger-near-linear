#pragma once

#include <vector>
#include "types.hpp"
#include "edge.hpp"


// MultiGraph and WeightedGraph closely related and converted into each other
// => use shared index type
struct GraphEdgeIndex {
    size_t val;

    GraphEdgeIndex() = default;
    explicit GraphEdgeIndex(size_t _val) : val(_val) {}
    
    bool operator==(const GraphEdgeIndex& index) { return val == index.val; }
    bool operator!=(const GraphEdgeIndex& index) { return val != index.val; }
    bool operator<(const GraphEdgeIndex& index) { return val < index.val; }
    bool operator>(const GraphEdgeIndex& index) { return val > index.val; }
    bool operator<=(const GraphEdgeIndex& index) { return val <= index.val; }
    bool operator>=(const GraphEdgeIndex& index) { return val >= index.val; }
    
    GraphEdgeIndex operator+(size_t i) { return GraphEdgeIndex(val + i); }
    GraphEdgeIndex& operator+=(size_t i) { val += i; return *this; }
    GraphEdgeIndex& operator++() { ++val; return *this; }
    GraphEdgeIndex operator++(int) { auto tmp = *this; ++(*this); return tmp; }
};

class MultiGraph;

class WeightedGraph {
private:
    size_t mNumVertices;
    std::vector<WeightedEdge> mEdges;

public:
    using EdgeIndex = GraphEdgeIndex;

    WeightedGraph(size_t numVertices, const WeightedEdge* edges, size_t numEdges);
    WeightedGraph(size_t numVertices, std::vector<WeightedEdge>&& edges);

    size_t numVertices() const { return mNumVertices; }
    size_t numEdges() const { return mEdges.size(); }
    const WeightedEdge& edge(EdgeIndex index) const { return mEdges[index.val]; }
    const std::vector<WeightedEdge>& edges() const { return mEdges; }

    EdgeWeight minEdgeWeight() const;
    EdgeWeight maxEdgeWeight() const;

    MultiGraph approxAsMultiGraph(double epsilon) const;
};


class MultiGraph {
private:
    size_t mNumVertices;
    std::vector<MultiEdge> mEdges;

public:
    using EdgeIndex = GraphEdgeIndex;

    MultiGraph(size_t numVertices, const MultiEdge* edges, size_t numEdges);
    MultiGraph(size_t numVertices, std::vector<MultiEdge>&& edges);

    size_t numVertices() const { return mNumVertices; }
    size_t numEdges() const { return mEdges.size(); }
    const MultiEdge& edge(EdgeIndex index) const { return mEdges[index.val]; }
    const std::vector<MultiEdge>& edges() const { return mEdges; }

    size_t numSimpleEdges() const;

    /**
     * Sample each edge with probability sampleProbability and cap the edge-multiplicities to maxEdgeMultiplicity
    */
    void sampleEdges(double sampleProbability, EdgeMultiplicity maxEdgeMultiplicity);
};