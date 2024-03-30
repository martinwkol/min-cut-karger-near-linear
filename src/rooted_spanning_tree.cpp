#include "rooted_spanning_tree.hpp"

#include <stack>

void RootedSpanningTree::initParentsChildren() {
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

void RootedSpanningTree::heavyLightDecomposition() {
    if (mGraph.numVertices() <= 1) return;

    std::vector<size_t> orderedEdgeSelection;
    orderedEdgeSelection.reserve(mEdgeSelection.size());    
    mHeavyPathStart.resize(mGraph.numVertices());

    // artificial stack to avoid stack overflow
    struct Parameters {
        VertexID vertex;
        size_t childIndex;
        size_t heavyChildIndex;
    };

    constexpr size_t HEAVY_CHILD_NOT_SEARCHED = size_t(-1);
    constexpr size_t HEAVY_CHILD_NOT_FOUND = size_t(-2);

    std::stack<Parameters, std::deque<Parameters>> stack;
    stack.push({ mRoot, 0, HEAVY_CHILD_NOT_SEARCHED });
    size_t currentHeavyPathStart = mChildren[mRoot].front().edgeIndex;
    while (!stack.empty()) {
        Parameters& p = stack.top();

        if (mChildren[p.vertex].empty()) {
            stack.pop();
            continue;
        }

        const std::vector<AdjacentVertex>& children = mChildren[p.vertex];
        if (p.heavyChildIndex == HEAVY_CHILD_NOT_SEARCHED) {
            // This is the case if this vertex is visited for the first time
            // Since we have to visit the heavy child first (if it exists),
            // we have to search for it

            size_t minHeavyChildSize = (mSubtreeSize[p.vertex] + 1) / 2;
            for (size_t i = 0; i < children.size(); i++) {
                if (mSubtreeSize[children[i].vertex] >= minHeavyChildSize) {
                    p.heavyChildIndex = i;
                    break;
                }
            }

            if (p.heavyChildIndex != HEAVY_CHILD_NOT_SEARCHED) {
                // Heavy child found
                const AdjacentVertex& heavyChild = children[p.heavyChildIndex];
                // New heavy path starts here
                currentHeavyPathStart = heavyChild.edgeIndex;
                mHeavyPathStart[heavyChild.vertex] = currentHeavyPathStart;
                // Visit heavy child first
                orderedEdgeSelection.push_back(heavyChild.edgeIndex);
                stack.push({ heavyChild.vertex, 0, HEAVY_CHILD_NOT_SEARCHED });
                continue;
            } else {
                // Heavy child not found
                p.heavyChildIndex = HEAVY_CHILD_NOT_FOUND;
            }
        }

        // If we reach this point, the heavy child has already been visited (if it exists).
        // Thus, we skip it here.
        if (p.childIndex == p.heavyChildIndex) p.childIndex++;

        if (p.childIndex >= children.size()) {
            // All children visited => this vertex is finished. Remove it from the stack and continue (with the parent vertex)
            stack.pop();
            continue;
        }

        // Visit child
        const AdjacentVertex& child = children[p.childIndex++];
        mHeavyPathStart[child.vertex] = currentHeavyPathStart;
        orderedEdgeSelection.push_back(child.edgeIndex);

        if (p.childIndex == children.size()) {
            // In this case, this is the last child of this vertex that we need to visit.
            // There is no need to come back to this vertex once the child has been examined
            // => Remove this vertex from the stack
            stack.pop();
        }

        stack.push({ child.vertex, 0, HEAVY_CHILD_NOT_SEARCHED });
    }

    mEdgeSelection = std::move(orderedEdgeSelection);
}

RootedSpanningTree::RootedSpanningTree(const WeightedGraph& graph, const EdgeSelection& edgeSelection, VertexID root) 
    : mGraph(graph), mEdgeSelection(edgeSelection), mRoot(root) {
    
    initParentsChildren();
    heavyLightDecomposition();
}

