#pragma once

#include <limits>
#include <cstddef>

typedef unsigned int uint;
typedef unsigned long long ull;

typedef unsigned int VertexID;
typedef double EdgeWeight;
typedef ull EdgeMultiplicity;

constexpr static EdgeWeight INFINITE_WEIGHT = std::numeric_limits<EdgeWeight>::max(); 