#include "two_respecting_trees.hpp"

#include <vector>
#include <stdexcept>
#include <cmath>
#include "spanning_tree.hpp"
#include "tree_packing.hpp"

/**
 * Returns upper bound for the size of the minimum cut
*/
static EdgeMultiplicity mcwUpperBound(const MultiGraph& graph) {
    std::vector<MultiGraph::EdgeIndex> edgeIndices(graph.numVertices() - 1);
    maxSpanningTree(edgeIndices, graph);
    EdgeMultiplicity maxMultiplicity = 0;
    for (MultiGraph::EdgeIndex index : edgeIndices) {
        EdgeMultiplicity multiplicity = graph.edge(index).multiplicity();
        if (multiplicity > maxMultiplicity) {
            maxMultiplicity = multiplicity;
        }
    }
    return maxMultiplicity * graph.numVertices() * graph.numVertices();
}

TreePacking findTwoRespectingTrees(const WeightedGraph& graph, double d, double eps1, double eps2, double eps3) {
    if (d <= 0 || eps1 <= 0 || eps2 <= 0 || eps3 <= 0) {
        throw std::invalid_argument("d, eps1, eps2, or eps3 is not positive");
    }
    if ((1.0 - eps2) * (1.0 - eps3) < 2.0 / 3) {
        throw std::invalid_argument("eps2 and / or eps3 too large");
    }

    double f = 3/2.0 - ((2.0+eps1)*(1.0+eps2))/((2.0-eps1)*(1.0-eps3));
    if (f <= 0) {
        throw std::invalid_argument("eps1 and / or eps2 and / or eps3 too large");
    }

    MultiGraph multiGraphApprox = graph.approxAsMultiGraph(eps1);
    EdgeMultiplicity mcwGuess = mcwUpperBound(multiGraphApprox);
    double b = (d + 2) * std::log(graph.numVertices()) / (eps2 * eps2);
    EdgeMultiplicity maxEdgeMultiplicity = static_cast<EdgeMultiplicity>(std::ceil(24.0 * (1 + eps2) * b));
    bool lastIteration = false;

    while (true) {
        MultiGraph sampled = multiGraphApprox;
        double sampleProbability = 2.0 * b / mcwGuess;
        if (sampleProbability < 1.0) {
            sampled.sampleEdges(sampleProbability, maxEdgeMultiplicity);
        }

        TreePacking packing(sampled, eps3);
        if (lastIteration || sampleProbability >= 1.0) {
            packing.sampleTrees(static_cast<size_t>(std::ceil(-d * std::log(graph.numVertices()) / std::log(1 - f))));
            return packing;
        }

        if (packing.packingWeight() >= 2.0 * b / 3) {
            mcwGuess /= 6.0;
            lastIteration = true;
        } else {
            mcwGuess /= 2.0;
        }
    }
}