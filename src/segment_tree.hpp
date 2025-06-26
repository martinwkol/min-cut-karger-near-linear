#pragma once

#include <vector>
#include "types.hpp"

class SegmentTree {
public:
    typedef EdgeWeight DataType;
    
    explicit SegmentTree(size_t listSize);
    SegmentTree(const DataType* list, size_t listSize);

    size_t listSize() const { return mListSize; }
    DataType minValue() const { return mValues[0]; }
    size_t minIndex();

    void intervalAdd(DataType value, const Interval& interval);

private: 
    size_t mListSize;
    std::vector<DataType> mValues;
    std::vector<DataType> mLazy;

    void initTree(const DataType* list, const Interval& interval, size_t valuesIndex);
    void updateLazy(size_t valuesIndex, const Interval& nodeInterval);
    void addToIntervalRecursive(DataType value2add, const Interval& addRange, const Interval& nodeInterval, size_t valuesIndex);
};