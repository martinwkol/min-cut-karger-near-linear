#pragma once

#include "types.hpp"

struct Edge {
    VertexID endpoints[2];
};

struct WeightedEdge : public Edge {
    EdgeWeight weight;
};

struct MultiEdge : public Edge {
    EdgeMultiplicity multiplicity;
};

