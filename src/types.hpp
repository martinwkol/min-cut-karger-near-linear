#pragma once

#include <limits>
#include <cstddef>
#include <vector>

typedef unsigned int uint;
typedef unsigned long long ull;

typedef unsigned int VertexID;
typedef double EdgeWeight;
typedef ull EdgeMultiplicity;

constexpr static EdgeWeight INFINITE_WEIGHT = std::numeric_limits<EdgeWeight>::max(); 

struct Interval {
    size_t start;
    size_t end;
};

struct Cut {
    std::vector<VertexID> vertices;
    EdgeWeight weight = INFINITE_WEIGHT;
};