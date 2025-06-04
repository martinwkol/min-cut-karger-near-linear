#include <stdexcept>
#include <iostream>
#include "graph_generation.hpp"
#include "stoer_wagner.hpp"
#include "find_mincut.hpp"

int main() {
    WeightedGraph graph = randomConnectedWeightedGraph(100, 1000, 2.0, 5.0);
    Cut stoerWagnerResult = stoerWagner(graph);
    Cut nearLinearResult = findLikelyMinCut(graph, 1.0);
    if (stoerWagnerResult.weight != nearLinearResult.weight) {
        char errorMessage[256];
        snprintf(errorMessage, 256, "Expected weight %f but got %f!\n", stoerWagnerResult.weight, nearLinearResult.weight);
        throw std::runtime_error(errorMessage);
    }
    return 0;
}