#include "segment_tree.hpp"

void SegmentTree::initTree(const DataType* list, const Interval& interval, size_t valuesIndex) {
    if (interval.start == interval.end) {
        mValues[valuesIndex] = list[interval.start];
        return;
    }

    size_t middle = (interval.start + interval.end) / 2;
    size_t indexLeftChild = valuesIndex * 2;
    size_t indexRightChild = valuesIndex * 2 + 1;
    
    initTree(list, { interval.start, middle }, indexLeftChild);
    initTree(list, { middle + 1, interval.end }, indexRightChild);
    mValues[valuesIndex] = std::min(mValues[indexLeftChild], mValues[indexRightChild]);
}

void SegmentTree::updateLazy(size_t valuesIndex, const Interval& nodeInterval) {
    DataType& value = mValues[valuesIndex];
    DataType& lazy = mLazy[valuesIndex];

    if (lazy != 0) {
        value += lazy;
        if (nodeInterval.start < nodeInterval.end) {
            // Propagate to children
            mLazy[valuesIndex * 2] += lazy;
            mLazy[valuesIndex * 2 + 1] += lazy;
        }
        lazy = 0;
    }
}

void SegmentTree::addToIntervalRecursive(
    DataType value2add, 
    const Interval& addRange, 
    const Interval& nodeInterval, 
    size_t valuesIndex
) {    
    DataType& value = mValues[valuesIndex];
    size_t indexLeftChild = valuesIndex * 2;
    size_t indexRightChild = valuesIndex * 2 + 1;
    
    updateLazy(valuesIndex, nodeInterval);

    // Add range outside of the node's range
    if (addRange.end < nodeInterval.start || nodeInterval.end < addRange.start) return;

    // Add range completely covers node's range
    if (addRange.start <= nodeInterval.start && nodeInterval.end <= addRange.end) {
        value += value2add;
        if (nodeInterval.start < nodeInterval.end) {
            // Propagate to children
            mLazy[indexLeftChild] += value2add;
            mLazy[indexRightChild] += value2add;
        }
        return;
    }

    size_t middle = (nodeInterval.start + nodeInterval.end) / 2;
    addToIntervalRecursive(value2add, addRange, { nodeInterval.start, middle }, indexLeftChild);
    addToIntervalRecursive(value2add, addRange, { middle + 1, nodeInterval.end }, indexRightChild);
    mValues[valuesIndex] = std::min(mValues[indexLeftChild], mValues[indexRightChild]);
}

SegmentTree::SegmentTree(size_t listSize) : mListSize(listSize), mValues(listSize * 4, 0), mLazy(listSize * 4, 0) {

}

SegmentTree::SegmentTree(const DataType* list, size_t listSize) : mListSize(listSize), mValues(listSize * 4, 0), mLazy(listSize * 4, 0) {
    initTree(list, { 0, listSize - 1 }, 0);
}

size_t SegmentTree::minIndex() {
    Interval interval = { 0, mListSize - 1 };
    size_t valuesIndex = 0;

    while (interval.start < interval.end) {
        size_t middle = (interval.start + interval.end) / 2;
        size_t indexLeftChild = valuesIndex * 2;
        size_t indexRightChild = valuesIndex * 2 + 1;

        updateLazy(indexLeftChild, { interval.start, middle });
        updateLazy(indexRightChild, { middle + 1, interval.end });

        if (mValues[indexLeftChild] < mValues[indexRightChild]) {
            valuesIndex = indexLeftChild;
            interval.end = middle;
        } else {
            valuesIndex = indexRightChild;
            interval.start = middle + 1;
        }
    }

    return interval.start;
}

void SegmentTree::intervalAdd(DataType value, const Interval& addRange) {
    addToIntervalRecursive(value, addRange, { 0, mListSize - 1 }, 0);
}