#include "rooted_spanning_tree.hpp"

#include <stack>
#include <algorithm>
#include <cmath>
#include <sstream>

static const RootedSpanningTree::AdjacentVertex NO_PARENT = RootedSpanningTree::AdjacentVertex(RootedSpanningTree::EdgeIndex(size_t(-1)), VertexID(-1));

RootedSpanningTree::EdgeIndex& RootedSpanningTree::heavyPathStart(RootedSpanningTree::EdgeIndex idx) {
    return mHeavyPathStart[idx.val];
}

RootedSpanningTree::EdgeIndex RootedSpanningTree::heavyPathStart(RootedSpanningTree::EdgeIndex idx) const {
    return mHeavyPathStart[idx.val];
}

void RootedSpanningTree::initParentsChildren()
{
    mParents.resize(mGraph.numVertices(), NO_PARENT);
    mChildren.resize(mGraph.numVertices());
    mSubtreeSize.resize(mGraph.numVertices(), 1);

    std::vector<std::vector<AdjacentVertex>> adjacent(mGraph.numVertices());
    EdgeIndex edgeSelectionSize(mEdgeSelection.size());
    for (EdgeIndex i(0); i < edgeSelectionSize; ++i) {
        const WeightedEdge& edge = this->edge(i);
        adjacent[edge.endpoint(0)].push_back({ i, edge.endpoint(1) });
        adjacent[edge.endpoint(1)].push_back({ i, edge.endpoint(0) });
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

    std::vector<WeightedGraph::EdgeIndex> orderedEdgeSelection;
    orderedEdgeSelection.reserve(mEdgeSelection.size());    
    mHeavyPathStart.resize(mEdgeSelection.size());

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
    RootedSpanningTree::EdgeIndex currentHeavyPathStart(0);
    while (!stack.empty()) {
        Parameters& p = stack.top();

        if (mChildren[p.vertex].empty()) {
            stack.pop();
            continue;
        }

        std::vector<AdjacentVertex>& children = mChildren[p.vertex];
        RootedSpanningTree::EdgeIndex nextIdx(orderedEdgeSelection.size());
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
                AdjacentVertex& heavyChild = children[p.heavyChildIndex];

                // New heavy path starts here
                WeightedGraph::EdgeIndex originalEdgeIndex = this->originalEdgeIndex(heavyChild.edgeIndex);
                RootedSpanningTree::EdgeIndex orderedEdgeIndex = nextIdx;
                heavyChild.edgeIndex = orderedEdgeIndex;
                mParents[heavyChild.vertex].edgeIndex = orderedEdgeIndex;
                heavyPathStart(orderedEdgeIndex) = currentHeavyPathStart;
                // Visit heavy child first

                orderedEdgeSelection.push_back(originalEdgeIndex);
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

        // Visit light child
        AdjacentVertex& child = children[p.childIndex];
        WeightedGraph::EdgeIndex edgeIndexInGraph = originalEdgeIndex(child.edgeIndex);
        child.edgeIndex = nextIdx;
        mParents[child.vertex].edgeIndex = nextIdx;
        currentHeavyPathStart = nextIdx; // light child -> new heavy path
        heavyPathStart(nextIdx) = currentHeavyPathStart;
        orderedEdgeSelection.push_back(edgeIndexInGraph);

        p.childIndex++;
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

std::vector<RootedSpanningTree::EdgeInterval> RootedSpanningTree::findVertex2RootSubsequences(VertexID vertex) const {
    std::vector<EdgeInterval> subseq;
    subseq.reserve(2 * size_t(ceil(log2(mGraph.numVertices()))) + 2);
    while (vertex != mRoot && subseq.size() < 10) {
        EdgeInterval interval;
        interval.end = mParents[vertex].edgeIndex;
        interval.start = heavyPathStart(interval.end);

        const WeightedEdge& startEdge = this->edge(interval.start);
        vertex = mParents[startEdge.endpoint(1)].vertex == startEdge.endpoint(0) ? startEdge.endpoint(0) : startEdge.endpoint(1);
        subseq.push_back(interval);
    }
    return subseq;
}

RootedSpanningTree::RootedSpanningTree(const WeightedGraph& graph, const std::vector<WeightedGraph::EdgeIndex>& edgeSelection, VertexID root) 
    : mGraph(graph), mEdgeSelection(edgeSelection), mRoot(root) {
    
    initParentsChildren();
    heavyLightDecomposition();
}

std::vector<RootedSpanningTree::EdgeInterval> RootedSpanningTree::findVertex2VertexSubsequences(VertexID vertex1, VertexID vertex2) const {
    std::vector<EdgeInterval> v1Intervals = findVertex2RootSubsequences(vertex1);
    std::vector<EdgeInterval> v2Intervals = findVertex2RootSubsequences(vertex2);

    while (!v1Intervals.empty() && !v2Intervals.empty() && v1Intervals.back().start == v2Intervals.back().start && v1Intervals.back().end == v2Intervals.back().end) {
        v1Intervals.pop_back();
        v2Intervals.pop_back();
    }

    if (!v1Intervals.empty() && !v2Intervals.empty() && v1Intervals.back().start == v2Intervals.back().start) {
        if (v1Intervals.back().end < v2Intervals.back().end) {
            v2Intervals.back().start = v1Intervals.back().end + 1;
            v1Intervals.pop_back();
        } else {
            v1Intervals.back().start = v2Intervals.back().end + 1;
            v2Intervals.pop_back();
        }
    }

    std::reverse(v2Intervals.begin(), v2Intervals.end());
    v1Intervals.insert(v1Intervals.end(), v2Intervals.begin(), v2Intervals.end());

    return v1Intervals;
}

std::vector<VertexID> RootedSpanningTree::cutVerticesFromCrossingEdges(const std::vector<RootedSpanningTree::EdgeIndex>& crossingEdgeIndices, bool containsRoot) const {
    std::vector<bool> isCoossingEdge(numEdges(), false);
    for (EdgeIndex crossingEdgeIndex : crossingEdgeIndices) {
        isCoossingEdge[crossingEdgeIndex.val] = true;
    }

    std::vector<VertexID> cut;

    // artificial stack to avoid stack overflow
    struct Parameters {
        VertexID vertex;
        size_t childIndex;
        bool inCut;
    };
    
    std::stack<Parameters, std::deque<Parameters>> stack;
    stack.push({ mRoot, 0, containsRoot });
    while (!stack.empty()) {
        Parameters& p = stack.top();
        if (p.childIndex == 0 && p.inCut) {
            cut.push_back(p.vertex);
        }

        const auto& children = mChildren[p.vertex];
        if (p.childIndex == children.size()) {
            stack.pop();
            continue;
        }

        const AdjacentVertex& child = children[p.childIndex++];
        stack.push({ child.vertex, 0, isCoossingEdge[child.edgeIndex.val] ? !p.inCut : p.inCut });
    }

    return cut;
}

std::string RootedSpanningTree::toString() const {
    std::stringstream s;

    // artificial stack to avoid stack overflow
    struct Parameters {
        std::string prefix;
        VertexID vertex;
        size_t childIndex;
        bool isLast;
    };
    
    std::stack<Parameters, std::deque<Parameters>> stack;
    stack.push({ "", mRoot, 0, true });
    while (!stack.empty()) {
        Parameters& p = stack.top();
        if (p.childIndex == 0) {
            s << p.prefix;
            if (p.isLast)   s << "└──";
            else            s << "├──";
            s << p.vertex << '\n';
        }
        const auto& children = mChildren[p.vertex];
        if (p.childIndex == children.size()) {
            stack.pop();
            continue;
        }

        const AdjacentVertex& child = children[p.childIndex];
        std::string childPrefix = p.prefix + (p.isLast ? "    " : "│   ");
        bool childIsLast = p.childIndex == children.size() - 1;
        ++p.childIndex;
        stack.push({ std::move(childPrefix), child.vertex, 0, childIsLast });
    }
    

    return s.str();
}
