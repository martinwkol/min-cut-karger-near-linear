#include "rooted_spanning_tree.hpp"

#include <stack>

RootedSpanningTree::RootedSpanningTree(const WeightedGraph& graph, const EdgeSelection& edgeSelection, VertexID root) 
    : mGraph(graph), mEdgeSelection(edgeSelection), mRoot(root) {
    
    mParents.resize(mGraph.numVertices(), { size_t(-1), VertexID(-1) });
    mChildren.resize(mGraph.numVertices());
    mSubtreeSize.resize(mGraph.numVertices(), 1);

    std::vector<std::vector<AdjacentVertex>> adjacent(mGraph.numVertices());
    for (size_t edgeIndex : mEdgeSelection) {
        const WeightedEdge& edge = mGraph.edges()[edgeIndex];
        adjacent[edge.endpoint(0)].push_back({ edgeIndex, edge.endpoint(1) });
        adjacent[edge.endpoint(1)].push_back({ edgeIndex, edge.endpoint(0) });
    }

    // artificial stack to avoid stack overflow
    struct Parameters {
        VertexID vertex;
        size_t adjacentIndex;
    };

    std::stack<Parameters, std::deque<Parameters>> stack;
    stack.push({ mRoot, 0 });
    while (!stack.empty()) {
        Parameters& p = stack.top();
        if (p.adjacentIndex < adjacent[p.vertex].size()) {
            AdjacentVertex adj = adjacent[p.vertex][p.adjacentIndex];
            if (adj.vertex != mParents[p.vertex].vertex) {
                mParents[adj.vertex].edgeIndex = adj.edgeIndex;
                mParents[adj.vertex].vertex = p.vertex;

                mChildren[p.vertex].push_back(adj);
                stack.push({ adj.vertex, 0 });
            }
            p.adjacentIndex++;
        } else {
            for (const AdjacentVertex& child : mChildren[p.vertex]) {
                mSubtreeSize[p.vertex] += mSubtreeSize[child.vertex];
            }
            stack.pop();
        }
    }
}