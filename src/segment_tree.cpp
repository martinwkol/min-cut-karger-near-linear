#include "segment_tree.hpp"

void SegmentTree::initTree(const DataType* list, size_t left, size_t right, size_t index) {
    if (left == right) {
        mValues[index] = list[left];
        return;
    }

    size_t middle = (left + right) / 2;
    size_t indexLeft = index * 2;
    size_t indexRight = index * 2 + 1;
    
    initTree(list, left, middle, indexLeft);
    initTree(list, middle + 1, right, indexRight);
    mValues[index] = std::min(mValues[indexLeft], mValues[indexRight]);
}

SegmentTree::SegmentTree(size_t listSize) : mListSize(listSize), mValues(listSize * 4, 0), mLazy(listSize * 4, 0) {

}

SegmentTree::SegmentTree(const DataType* list, size_t listSize) : mListSize(listSize), mValues(listSize * 4, 0), mLazy(listSize * 4, 0) {
    initTree(list, 0, listSize - 1, 0);
}