#pragma once

#include <vector>
#include <string>
#include "graph.hpp"
#include "strong_index.hpp"
#include "custom_index_vector.hpp"

class RootedSpanningTree {
private:
    struct RSTEdgeIndexTag {};

public:
    using EdgeIndex = StrongIndex<RSTEdgeIndexTag>;

    struct EdgeInterval {
        EdgeIndex start;
        EdgeIndex end;
    };

    template <typename T>
    using EdgeVector = CustomIndexVector<EdgeIndex, T>;

    // A vertex v is adjacent to w if the graph contains an egde e = { v, w }
    // We can store AdjacentVertex(index_of(e), w)) for v and AdjacentVertex(index_of(e), v)) for w
    struct AdjacentVertex {
        EdgeIndex edgeIndex;
        VertexID vertex;
        AdjacentVertex() = default;
        AdjacentVertex(EdgeIndex _edgeIndex, VertexID _vertex) 
            : edgeIndex(_edgeIndex), vertex(_vertex) {}
    };
    

    /**
     * Constructs a RootedSpanningTree
     * @param graph Supergraph
     * @param edgeSelection A vector of indixes of edges. The edge selection must induce a spanning tree.
     * @param root Root of the spanning tree.
     */
    RootedSpanningTree(const WeightedGraph& graph, const std::vector<WeightedGraph::EdgeIndex>& edgeSelection, VertexID root);

    const WeightedGraph& getGraph() const { return mGraph; }
    const EdgeVector<WeightedGraph::EdgeIndex>& getEdgeSelection() const { return mEdgeSelection; }
    VertexID getRoot() const { return mRoot; }
    size_t getNumVertices() const { return mGraph.getNumVertices(); }
    size_t getNumEdges() const { return mEdgeSelection.size(); }

    const AdjacentVertex& getParent(VertexID vertex) const { return mParents[vertex]; }
    const std::vector<AdjacentVertex>& getChildren(VertexID vertex) const { return mChildren[vertex]; }
    
    WeightedGraph::EdgeIndex getOriginalEdgeIndex(RootedSpanningTree::EdgeIndex idx) const { return mEdgeSelection[idx]; }
    const WeightedEdge& getEdge(RootedSpanningTree::EdgeIndex idx) const { return mGraph.getEdge(getOriginalEdgeIndex(idx)); }

    std::vector<EdgeInterval> findVertex2VertexSubsequences(VertexID vertex1, VertexID vertex2) const;
    std::vector<VertexID> cutVerticesFromCrossingEdges(const std::vector<RootedSpanningTree::EdgeIndex>& crossingEdgeIndices, bool containsRoot = false) const;

    std::string toString() const;

private:
    // mGraph is not necessarily a tree itself. The induced subgraph from mEdgeSelection is.
    const WeightedGraph& mGraph;
    EdgeVector<WeightedGraph::EdgeIndex> mEdgeSelection;

    VertexID mRoot;
    std::vector<AdjacentVertex> mParents;
    std::vector<std::vector<AdjacentVertex>> mChildren;
    std::vector<size_t> mSubtreeSize;

    EdgeVector<RootedSpanningTree::EdgeIndex> mHeavyPathStart;
    
    void initParentsChildren();
    void heavyLightDecomposition();

    std::vector<EdgeInterval> findVertex2RootSubsequences(VertexID vertex) const;
};