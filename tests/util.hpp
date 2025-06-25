#pragma once
#include <sstream>
#include <vector>
#include "types.hpp"
#include "graph.hpp"

Cut cutFromVertices(const WeightedGraph& graph, std::vector<VertexID>&& vertices);
std::string graph2string(const WeightedGraph& graph);
std::string cut2string(const Cut& cut);