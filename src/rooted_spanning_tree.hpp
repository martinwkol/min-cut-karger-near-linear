#pragma once

#include <vector>
#include <string>
#include "graph.hpp"
#include "edge_index_vector.hpp"

class RootedSpanningTree {
public:
    struct EdgeIndex {
        size_t val;

        EdgeIndex() = default;
        explicit EdgeIndex(size_t _val) : val(_val) {}
        
        bool operator==(const EdgeIndex& index) { return val == index.val; }
        bool operator!=(const EdgeIndex& index) { return val != index.val; }
        bool operator<(const EdgeIndex& index) { return val < index.val; }
        bool operator>(const EdgeIndex& index) { return val > index.val; }
        bool operator<=(const EdgeIndex& index) { return val <= index.val; }
        bool operator>=(const EdgeIndex& index) { return val >= index.val; }
        
        EdgeIndex operator+(size_t i) { return EdgeIndex(val + i); }
        EdgeIndex& operator+=(size_t i) { val += i; return *this; }
        EdgeIndex& operator++() { ++val; return *this; }
        EdgeIndex operator++(int) { auto tmp = *this; ++(*this); return tmp; }
    };

    struct EdgeInterval {
        EdgeIndex start;
        EdgeIndex end;
    };

    template <typename _Ty>
    using EdgeVector = EdgeIndexVector<EdgeIndex, _Ty>;

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
    RootedSpanningTree(const WeightedGraph& graph, const EdgeVector<WeightedGraph::EdgeIndex>& edgeSelection, VertexID root);

    const WeightedGraph& graph() const { return mGraph; }
    const EdgeVector<WeightedGraph::EdgeIndex>& edgeSelection() const { return mEdgeSelection; }
    VertexID root() const { return mRoot; }
    size_t numVertices() const { return mGraph.numVertices(); }
    size_t numEdges() const { return mEdgeSelection.size(); }
    const AdjacentVertex& parent(VertexID vertex) const { return mParents[vertex]; }
    const std::vector<AdjacentVertex>& children(VertexID vertex) const { return mChildren[vertex]; }
    WeightedGraph::EdgeIndex originalEdgeIndex(RootedSpanningTree::EdgeIndex idx) const { return mEdgeSelection[idx]; }
    const WeightedEdge& edge(RootedSpanningTree::EdgeIndex idx) const { return mGraph.edge(originalEdgeIndex(idx)); }

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