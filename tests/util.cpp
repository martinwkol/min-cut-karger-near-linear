#include "util.hpp"
#include "types.hpp"
#include "graph.hpp"
#include <sstream>
#include <vector>

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