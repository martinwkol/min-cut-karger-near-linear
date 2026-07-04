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

    std::vector<std::vector<EdgeWeight>> weights(graph.getNumVertices());
    for (std::vector<EdgeWeight>& weights0 : weights) {
        weights0.resize(graph.getNumVertices(), 0);
    }

    for (const WeightedEdge& edge : graph.getEdges()) {
        weights[edge.endpoint(0)][edge.endpoint(1)] = edge.getWeight();
        weights[edge.endpoint(1)][edge.endpoint(0)] = edge.getWeight();
    }

    std::vector<std::vector<VertexID>> contracted(graph.getNumVertices());
    for (VertexID vid = 0; vid < graph.getNumVertices(); vid++) {
        contracted[vid].push_back(vid);
    }

    std::vector<bool> vertexRemoved(graph.getNumVertices(), false);
    VertexID firstRemainingVertex = 0;

    for (size_t numRemainingVertices = graph.getNumVertices(); numRemainingVertices >= 2; numRemainingVertices--) {
        while (vertexRemoved[firstRemainingVertex]) firstRemainingVertex++;

        VertexID s = VertexID(-1);
        VertexID t = firstRemainingVertex;

        std::vector<bool> inA(graph.getNumVertices(), false);
        std::vector<EdgeWeight> connections2A = weights[t];
        inA[t] = true;

        for (size_t sizeOfA = 1; sizeOfA < numRemainingVertices; sizeOfA++) {
            VertexID mostConnected = VertexID(-1);
            EdgeWeight maxConnectionValue = std::numeric_limits<EdgeWeight>::lowest();
            for (VertexID vid = 0; vid < graph.getNumVertices(); vid++) {
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
                for (VertexID vid = 0; vid < graph.getNumVertices(); vid++) {
                    connections2A[vid] += weights[t][vid];
                }
            }
        }
        
        contracted[s].insert(contracted[s].end(), contracted[t].begin(), contracted[t].end());
        for (VertexID vid = 0; vid < graph.getNumVertices(); vid++) {
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