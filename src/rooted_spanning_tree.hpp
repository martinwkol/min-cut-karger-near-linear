#pragma once

#include <vector>
#include <string>
#include "graph.hpp"

class RootedSpanningTree {
public:
    // A vertex v is adjacent to w if the graph contains an egde e = { v, w }
    // We can store AdjacentVertex(index_of(e), w)) for v and AdjacentVertex(index_of(e), v)) for w
    struct AdjacentVertex {
        size_t edgeIndex;
        VertexID vertex;
    };

    struct Interval {
        size_t start;
        size_t end;
    };

private:
    // Indixes selected edges
    typedef std::vector<size_t> EdgeSelection;    

    // mGraph is not necessarily a tree itself. The induced subgraph from mEdgeSelection is.
    const WeightedGraph& mGraph;
    EdgeSelection mEdgeSelection;

    VertexID mRoot;
    std::vector<AdjacentVertex> mParents;
    std::vector<std::vector<AdjacentVertex>> mChildren;
    std::vector<size_t> mSubtreeSize;

    std::vector<size_t> mHeavyPathStart;

    constexpr static AdjacentVertex NO_PARENT = { size_t(-1), VertexID(-1) };

    void initParentsChildren();
    void heavyLightDecomposition();

    std::vector<Interval> findVertex2RootSubsequences(VertexID vertex) const;

public:
    /**
     * Constructs a RootedSpanningTree
     * @param graph Supergraph
     * @param edgeSelection A vector of indixes of edges. The edge selection must induce a spanning tree.
     * @param root Root of the spanning tree.
     */
    RootedSpanningTree(const WeightedGraph& graph, const EdgeSelection& edgeSelection, VertexID root);

    const WeightedGraph& graph() const { return mGraph; }
    const EdgeSelection& edgeSelection() const { return mEdgeSelection; }
    VertexID root() const { return mRoot; }
    size_t numVertices() const { return mGraph.numVertices(); }
    size_t numEdges() const { return mEdgeSelection.size(); }
    const AdjacentVertex& parent(VertexID vertex) const { return mParents[vertex]; }
    const std::vector<AdjacentVertex>& children(VertexID vertex) const { return mChildren[vertex]; }
    size_t originalEdgeIndex(size_t spanningTreeEdgeIndex) const { return mEdgeSelection[spanningTreeEdgeIndex]; }
    const WeightedEdge& edge(size_t spanningTreeEdgeIndex) const { return mGraph.edge(originalEdgeIndex(spanningTreeEdgeIndex)); }

    std::vector<Interval> findVertex2VertexSubsequences(VertexID vertex1, VertexID vertex2) const;
    std::vector<VertexID> cutFromCrossingEdges(const std::vector<size_t>& crossingEdgeIndices, bool containsRoot = false) const;
};