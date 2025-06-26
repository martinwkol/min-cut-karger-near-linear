#pragma once

#include "types.hpp"

class Edge {
public:
    Edge(VertexID endpoint0, VertexID endpoint1) {
        mEndpoints[0] = endpoint0;
        mEndpoints[1] = endpoint1;
    }

    VertexID endpoint(int index) const { return mEndpoints[index]; }
    void setEndpoint(int index, VertexID endpoint) { mEndpoints[index] = endpoint; }

protected:
    VertexID mEndpoints[2];
};

class WeightedEdge : public Edge {
public:
    WeightedEdge(VertexID endpoint0, VertexID endpoint1, EdgeWeight weight) : Edge(endpoint0, endpoint1), mWeight(weight) {}

    EdgeWeight weight() const { return mWeight; }
    void setWeight(EdgeWeight weight) { mWeight = weight; }
    
private:
    EdgeWeight mWeight;
};

class MultiEdge : public Edge {
public:
    MultiEdge(VertexID endpoint0, VertexID endpoint1, EdgeMultiplicity multiplicity) : Edge(endpoint0, endpoint1), mMultiplicity(multiplicity) {}

    EdgeMultiplicity multiplicity() const { return mMultiplicity; }
    void setMultiplicity(EdgeMultiplicity multiplicity) { mMultiplicity = multiplicity; }

private:
    EdgeMultiplicity mMultiplicity;
};

