#include "stoer_wagner.hpp"

#include <deque>
#include <list>
#include <limits>

Cut stoerWagner(const WeightedGraph& graph) {
    struct Neighbour {
        VertexID neighbour;
        EdgeWeight edgeWeight;
        size_t complementIndex;
    };

    std::vector<VertexID> minCut;
    EdgeWeight minCutWeight = std::numeric_limits<EdgeWeight>::max();

    std::vector<std::vector<EdgeWeight>> weights(graph.numVertices());
    for (std::vector<EdgeWeight>& weights0 : weights) {
        weights0.resize(graph.numVertices(), 0);
    }

    for (const WeightedEdge& edge : graph.edges()) {
        weights[edge.endpoint(0)][edge.endpoint(1)] = edge.weight();
        weights[edge.endpoint(1)][edge.endpoint(0)] = edge.weight();
    }

    std::vector<std::vector<VertexID>> contracted(graph.numVertices());
    for (VertexID vid = 0; vid < graph.numVertices(); vid++) {
        contracted[vid].push_back(vid);
    }

    std::vector<bool> vertexRemoved(graph.numVertices(), false);
    VertexID firstRemainingVertex = 0;

    for (size_t numRemainingVertices = graph.numVertices(); numRemainingVertices >= 2; numRemainingVertices--) {
        if (vertexRemoved[firstRemainingVertex]) firstRemainingVertex++;

        VertexID s = VertexID(-1);
        VertexID t = firstRemainingVertex;

        std::vector<bool> inA(graph.numVertices(), false);
        std::vector<EdgeWeight> connections2A = weights[t];
        inA[t] = true;

        for (size_t sizeOfA = 1; sizeOfA < numRemainingVertices; sizeOfA++) {
            VertexID mostConnected = VertexID(-1);
            EdgeWeight maxConnectionValue = -1.0;
            for (VertexID vid = 0; vid < graph.numVertices(); vid++) {
                if (inA[vid] || vertexRemoved[vid]) continue;
                if (connections2A[vid] > maxConnectionValue) {
                    maxConnectionValue = connections2A[vid];
                    mostConnected = vid;
                }
            }

            s = t;
            t = mostConnected;
            if (sizeOfA < numRemainingVertices - 1) {
                inA[t] = true;
                for (VertexID vid = 0; vid < graph.numVertices(); vid++) {
                    connections2A[vid] += weights[t][vid];
                }
            }
        }
        
        contracted[s].insert(contracted[s].end(), contracted[t].begin(), contracted[t].end());
        for (VertexID vid = 0; vid < graph.numVertices(); vid++) {
            weights[s][vid] += weights[t][vid];
            weights[vid][s] = weights[s][vid];
        }
        vertexRemoved[t] = true;

        EdgeWeight cutWeight = connections2A[t];
        if (cutWeight < minCutWeight) {
            minCut = std::move(contracted[t]);
            minCutWeight = cutWeight;
        }
    }

    return { minCut, minCutWeight };
}