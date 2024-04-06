#include "segment_tree.hpp"

void SegmentTree::initTree(const DataType* list, size_t left, size_t right, size_t valuesIndex) {
    if (left == right) {
        mValues[valuesIndex] = list[left];
        return;
    }

    size_t middle = (left + right) / 2;
    size_t indexLeftChild = valuesIndex * 2;
    size_t indexRightChild = valuesIndex * 2 + 1;
    
    initTree(list, left, middle, indexLeftChild);
    initTree(list, middle + 1, right, indexRightChild);
    mValues[valuesIndex] = std::min(mValues[indexLeftChild], mValues[indexRightChild]);
}

void SegmentTree::addToIntervalRecursive(DataType value2add, size_t intervalStart, size_t intervalEnd, size_t left, size_t right, size_t valuesIndex) {    
    DataType& value = mValues[valuesIndex];
    DataType& lazy = mLazy[valuesIndex];
    size_t indexLeftChild = valuesIndex * 2;
    size_t indexRightChild = valuesIndex * 2 + 1;
    
    // Update lazy
    if (lazy != 0) {
        value += lazy;
        if (left < right) {
            // Propagate to children
            mLazy[indexLeftChild] += lazy;
            mLazy[indexRightChild] += lazy;
        }
        lazy = 0;
    }

    // Interval range outside of the node's range
    if (intervalEnd < left || right < intervalStart) return;

    // Interval range completely covers node's range
    if (left <= intervalStart && intervalEnd <= right) {
        value += value2add;
        if (left < right) {
            // Propagate to children
            mLazy[indexLeftChild] += value;
            mLazy[indexRightChild] += value;
        }
        return;
    }

    size_t middle = (left + right) / 2;
    addToIntervalRecursive(value2add, intervalStart, intervalEnd, left, middle, indexLeftChild);
    addToIntervalRecursive(value2add, intervalStart, intervalEnd, middle + 1, right, indexRightChild);
    mValues[valuesIndex] = std::min(mValues[indexLeftChild], mValues[indexRightChild]);
}

SegmentTree::SegmentTree(size_t listSize) : mListSize(listSize), mValues(listSize * 4, 0), mLazy(listSize * 4, 0) {

}

SegmentTree::SegmentTree(const DataType* list, size_t listSize) : mListSize(listSize), mValues(listSize * 4, 0), mLazy(listSize * 4, 0) {
    initTree(list, 0, listSize - 1, 0);
}

size_t SegmentTree::minimumValueIndex() const {
    size_t left = 0;
    size_t right = mListSize - 1;
    size_t valuesIndex = 0;

    while (left < right) {
        size_t middle = (left + right) / 2;
        size_t indexLeft = valuesIndex * 2;
        size_t indexRight = valuesIndex * 2 + 1;

        if (mValues[indexLeft] < mValues[indexRight]) {
            right = middle;
        } else {
            left = middle + 1;
        }
    }

    return left;
}

void SegmentTree::addToInterval(DataType value, size_t intervalStart, size_t intervalEnd) {
    addToIntervalRecursive(value, intervalStart, intervalEnd, 0, mListSize - 1, 0);
}