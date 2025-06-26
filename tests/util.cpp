#include "util.hpp"
#include "types.hpp"
#include "graph.hpp"
#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"
#include <sstream>
#include <vector>
#include <cassert>

Cut cutFromVertices(const WeightedGraph& graph, std::vector<VertexID>&& vertices) {
    EdgeWeight weight = 0.0;
    std::vector<bool> inVertices(graph.numVertices(), false);
    for (VertexID vertex : vertices) {
        inVertices[vertex] = true;
    }
    for (const WeightedEdge& edge : graph.edges()) {
        bool e0InVertices = inVertices[edge.endpoint(0)];
        bool e1InVertices = inVertices[edge.endpoint(1)];
        if ((e0InVertices && !e1InVertices) || (!e0InVertices && e1InVertices)) {
            weight += edge.weight();
        }
    }
    return { std::move(vertices), weight };
}

void requireEqual(const Cut& testResult, const Cut& referenceCut, size_t numVertices) {
    assert(numVertices > 0);
    INFO(cut2string(testResult));
    REQUIRE_THAT(testResult.weight, Catch::Matchers::WithinRel(referenceCut.weight, 0.0001));
    std::vector<bool> inC1(numVertices, false);
    std::vector<bool> inC2(numVertices, false);
    for (VertexID vertex : testResult.vertices) inC1[vertex] = true;
    for (VertexID vertex : referenceCut.vertices) inC2[vertex] = true;
    bool sameSide = (inC1[0] && inC2[0]) || (!inC1[0] && !inC2[0]);
    for (VertexID vertex = 1; vertex < numVertices; ++vertex) {
        bool eq = (inC1[vertex] && inC2[vertex]) || (!inC1[vertex] && !inC2[vertex]);
        bool consistent = (sameSide && eq) || (!sameSide && !eq);
        REQUIRE(consistent);
    }
}

std::string graph2string(const WeightedGraph& graph) {
    std::stringstream s;
    s << "Num Vertices: " << graph.numVertices() << '\n';
    s << "Edges:\n";
    for (const WeightedEdge& edge : graph.edges()) {
        s << '(' << edge.endpoint(0) << ", " << edge.endpoint(1) << "): " << edge.weight() << '\n';
    } 
    return s.str();
}

std::string cut2string(const Cut& cut) {
    std::stringstream s;
    s << "Weight: " << cut.weight << '\n';
    s << "Vertices: ";
    for (VertexID vertex : cut.vertices) {
        s << vertex << ", ";
    }
    s << '\n';
    return s.str();
}
