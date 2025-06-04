#pragma once

#include <random>
#include "types.hpp"
#include "graph.hpp"

extern std::default_random_engine randomGenerator;

ull binom(ull trials, double prob, ull ciel = ull(-1));