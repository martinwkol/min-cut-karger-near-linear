#pragma once

#include <vector>
#include "types.hpp"

class SegmentTree {
public:
    typedef EdgeWeight DataType;

private:
    size_t mListSize;
    std::vector<DataType> mValues;
    std::vector<DataType> mLazy;

    void initTree(const DataType* list, size_t left, size_t right, size_t index);

public:
    explicit SegmentTree(size_t listSize);
    SegmentTree(const DataType* list, size_t listSize);

    size_t listSize() const { return mListSize; }
};