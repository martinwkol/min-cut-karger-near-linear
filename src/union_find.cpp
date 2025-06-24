#include "union_find.hpp"

UnionFind::UnionFind(dtype n) : mHeight(n, 0) {
    mParents.reserve(n);
    for (dtype i = 0; i < n; i++) {
        mParents.push_back(i);
    }
}

UnionFind::dtype UnionFind::find(dtype x) {
    if (mParents[x] == x) return x;   
    dtype rep = find(mParents[x]);
    mParents[x] = rep;
    return rep;
}

bool UnionFind::unionSets(dtype x, dtype y) {
    dtype repX = find(x);
    dtype repY = find(y);

    if (repX == repY) return false;
    
    htype hX = mHeight[repX];
    htype hY = mHeight[repY];

    // Add smaller tree to larger tree to keep the height small

    if (hX > hY) {
        mParents[repY] = repX;
        return true;
    }

    if (hX < hY) {
        mParents[repX] = repY;
        return true;
    }

    mParents[repY] = repX;
    mHeight[repX]++;
    return true;
}
