#pragma once
#include <sstream>
#include <vector>
#include "types.hpp"
#include "graph.hpp"

struct TestGraph {
    WeightedGraph graph;
    Cut minCut;
    bool minCutUnique;
};

namespace TestGraphs {
    extern TestGraph simple;
    extern TestGraph medium;
    extern TestGraph multipleMinCuts;
    extern TestGraph dense;
}

Cut cutFromVertices(const WeightedGraph& graph, std::vector<VertexID>&& vertices);
void requireSameWeight(const Cut& testResult, const Cut& referenceCut);
void requireEqual(const Cut& testResult, const Cut& referenceCut, size_t numVertices);
void checkMinCut(const TestGraph& testGraph, const Cut& testResult);
std::string graph2string(const WeightedGraph& graph);
std::string cut2string(const Cut& cut);