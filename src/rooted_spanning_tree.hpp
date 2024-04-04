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

    Interval* findVertex2RootSubsequences(Interval* buffer, Interval* bufferEnd, VertexID vertex) const;

public:
    RootedSpanningTree(const WeightedGraph& graph, const EdgeSelection& edgeSelection, VertexID root);

    const WeightedGraph& graph() const { return mGraph; }
    const EdgeSelection& edgeSelection() const { return mEdgeSelection; }
    VertexID root() const { return mRoot; }
    const std::vector<AdjacentVertex>& parents() const { return mParents; }
    const std::vector<std::vector<AdjacentVertex>>& children() const { return mChildren; }

    Interval* findVertex2VertexSubsequences(Interval* intervals, size_t intervalsSize, VertexID vertex1, VertexID vertex2) const;
};