#pragma once

#include <vector>
#include "types.hpp"
#include "edge.hpp"

class MultiGraph;

class WeightedGraph {
private:
    size_t mNumVertices;
    std::vector<WeightedEdge> mEdges;

public:
    WeightedGraph(size_t numVertices, const WeightedEdge* edges, size_t numEdges);
    WeightedGraph(size_t numVertices, std::vector<WeightedEdge>&& edges);

    size_t numVertices() const { return mNumVertices; }
    size_t numEdges() const { return mEdges.size(); }
    const WeightedEdge& edge(size_t index) const { return mEdges[index]; }
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
    MultiGraph(size_t numVertices, const MultiEdge* edges, size_t numEdges);
    MultiGraph(size_t numVertices, std::vector<MultiEdge>&& edges);

    size_t numVertices() const { return mNumVertices; }
    size_t numEdges() const { return mEdges.size(); }
    const MultiEdge& edge(size_t index) const { return mEdges[index]; }
    const std::vector<MultiEdge>& edges() const { return mEdges; }

    size_t numSimpleEdges() const;

    /**
     * Sample each edge with probability sampleProbability and cap the edge-multiplicities to maxEdgeMultiplicity
    */
    void sampleEdges(double sampleProbability, EdgeMultiplicity maxEdgeMultiplicity);
};