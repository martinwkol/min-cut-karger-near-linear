#pragma once

#include <vector>
#include "graph.hpp"

class RootedSpanningTree {
public:
    struct AdjacentVertex {
        size_t edgeIndex;
        VertexID vertex;
    };

    struct Interval {
        size_t start;
        size_t end;
    };

private:
    typedef std::vector<size_t> EdgeSelection;    

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
    RootedSpanningTree(const WeightedGraph& graph, const EdgeSelection& edgeSelection, VertexID root);

    const WeightedGraph& graph() const { return mGraph; }
    const EdgeSelection& edgeSelection() const { return mEdgeSelection; }
    VertexID root() const { return mRoot; }
    const AdjacentVertex& parent(VertexID vertex) const { return mParents[vertex]; }
    const std::vector<AdjacentVertex>& children(VertexID vertex) const { return mChildren[vertex]; }
    size_t originalEdgeIndex(size_t spanningTreeEdgeIndex) const { return mEdgeSelection[spanningTreeEdgeIndex]; }
    const WeightedEdge& edge(size_t spanningTreeEdgeIndex) const { return mGraph.edge(originalEdgeIndex(spanningTreeEdgeIndex)); }

    std::vector<Interval> findVertex2VertexSubsequences(VertexID vertex1, VertexID vertex2) const;
};