#pragma once

#include <vector>
#include "types.hpp"

/**
 * Union-Find datastructure on the set { 0, ..., n - 1 } for given integer n
*/
class UnionFind {
public:
    typedef VertexID dtype;

private:
    typedef unsigned char htype;

    std::vector<dtype> mParents;
    std::vector<htype> mHeight;

public:
    /**
     * Cunstructs Union-Find datastructure on the set { 0, ..., n - 1 }
    */
    explicit UnionFind(dtype n);

    /**
     * Finds the representative of the set containing x
    */
    dtype find(dtype x);

    /**
     * Merges the sets containing x and y respectively
     * @return 
     *      true if x and y were from different sets and the sets were unionized
     *      false if x and y already belong to the same set
    */
    bool unionSets(dtype x, dtype y);
};