#include "rooted_spanning_tree.hpp"

#include <stack>

void RootedSpanningTree::initParentsChildren() {
    mParents.resize(mGraph.numVertices(), NO_PARENT);
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

RootedSpanningTree::Interval* RootedSpanningTree::findVertex2RootSubsequences(Interval* buffer, Interval* bufferEnd, VertexID vertex) const {
    while (vertex != mRoot) {
        if (buffer == bufferEnd) return nullptr;
        buffer->end = mParents[vertex].edgeIndex;
        buffer->start = mHeavyPathStart[buffer->end];

        const WeightedEdge& edge = mGraph.edges()[buffer->start];
        vertex = edge.endpoint(0) ? mParents[edge.endpoint(1)].vertex == edge.endpoint(0) : edge.endpoint(1);
        buffer++;
    }
    return buffer;
}

RootedSpanningTree::RootedSpanningTree(const WeightedGraph& graph, const EdgeSelection& edgeSelection, VertexID root) 
    : mGraph(graph), mEdgeSelection(edgeSelection), mRoot(root) {
    
    initParentsChildren();
    heavyLightDecomposition();
}

RootedSpanningTree::Interval* RootedSpanningTree::findVertex2VertexSubsequences(Interval* intervals, size_t intervalsSize, VertexID vertex1, VertexID vertex2) const {
    size_t maxNumIntervals2Root = size_t(ceil(log2(mGraph.numVertices()))) + 2;
    std::vector<Interval> v1Intervals(maxNumIntervals2Root);
    std::vector<Interval> v2Intervals(maxNumIntervals2Root);
    
    findVertex2RootSubsequences(v1Intervals.data(), v1Intervals.data() + v1Intervals.size(), vertex1);
    findVertex2RootSubsequences(v2Intervals.data(), v2Intervals.data() + v2Intervals.size(), vertex1);

    while (!v1Intervals.empty() && !v2Intervals.empty() && v1Intervals.back().start == v2Intervals.back().start && v1Intervals.back().end == v2Intervals.back().end) {
        v1Intervals.pop_back();
        v2Intervals.pop_back();
    }

    if (v1Intervals.back().end < v2Intervals.back().end) {
        v2Intervals.back().start = v1Intervals.back().end + 1;
        v1Intervals.pop_back();
    } else {
        v1Intervals.back().start = v2Intervals.back().end + 1;
        v2Intervals.pop_back();
    }

    if (v1Intervals.size() + v2Intervals.size() > intervalsSize) return nullptr;

    std::reverse(v2Intervals.begin(), v2Intervals.end());

    Interval* v1IntStart = intervals;
    Interval* v2IntStart = intervals + v1Intervals.size();  
    std::memcpy(v1IntStart, v1Intervals.data(), v1Intervals.size() * sizeof(Interval));
    std::memcpy(v2IntStart, v2Intervals.data(), v2Intervals.size() * sizeof(Interval));
    
    return v2IntStart + v2Intervals.size();
}
