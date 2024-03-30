#pragma once

#include <vector>
#include "graph.hpp"

class RootedSpanningTree {
public:
    struct AdjacentVertex {
        size_t edgeIndex;
        VertexID vertex;
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

    void initParentsChildren();
    void heavyLightDecomposition();

public:
    RootedSpanningTree(const WeightedGraph& graph, const EdgeSelection& edgeSelection, VertexID root);

    const WeightedGraph& graph() const { return mGraph; }
    const EdgeSelection& edgeSelection() const { return mEdgeSelection; }
    VertexID root() const { return mRoot; }
    const std::vector<AdjacentVertex>& parents() const { return mParents; }
    const std::vector<std::vector<AdjacentVertex>>& children() const { return mChildren; }
};