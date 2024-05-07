#include "random.hpp"

#include <ctime>
#include <limits>
#include <vector>

std::default_random_engine randomGenerator(1514464);

ull binom(ull trials, double prob, ull ciel) {
    // use inverse transform sampling
    std::uniform_real_distribution<double> urd(0.0, 1.0);
    double uniformProb = urd(randomGenerator);

    double binomProb = pow(1 - prob, trials);
    double sumBinomProb = binomProb;
    for (ull em = 0; em <= ciel; em++) {
        if (sumBinomProb > uniformProb) return em;
        binomProb *= double(trials - em)/(em + 1) * prob / (1 - prob);
        sumBinomProb += binomProb;
    }
    return ciel;
}

WeightedGraph randomConnectedWeightedGraph(size_t numVertices, size_t numEdges, EdgeWeight minWeight, EdgeWeight maxWeight) {
    std::uniform_int_distribution<int> vertexDistr(0, std::numeric_limits<int>::max());
    std::uniform_real_distribution<double> weightDistr(minWeight, maxWeight);

    std::vector<WeightedEdge> edges;
    edges.reserve(numEdges);

    // Ensure that the graph is connected
    for (VertexID endpoint0 = 1; endpoint0 < numVertices; endpoint0++) {
        VertexID endpoint1 = vertexDistr(randomGenerator) % endpoint0;
        EdgeWeight weight = weightDistr(randomGenerator);
        edges.emplace_back(endpoint0, endpoint1, weight);
    }

    // Add remaining edges
    for (size_t i = numVertices; i < numEdges; i++) {
        VertexID endpoint0 = vertexDistr(randomGenerator) % numVertices;
        VertexID endpoint1 = vertexDistr(randomGenerator) % (numVertices - 1);
        if (endpoint1 >= endpoint0) endpoint1++;
        EdgeWeight weight = weightDistr(randomGenerator);
        edges.emplace_back(endpoint0, endpoint1, weight);
    }

    return WeightedGraph(numVertices, std::move(edges));
}