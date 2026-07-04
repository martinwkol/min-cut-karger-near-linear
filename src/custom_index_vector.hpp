#pragma once

#include <vector>

template <typename IndexT, typename T>
class CustomIndexVector : private std::vector<T> {
public:
    using std::vector<T>::vector;

    using std::vector<T>::push_back;
    using std::vector<T>::emplace_back;
    using std::vector<T>::reserve;
    using std::vector<T>::resize;

    using std::vector<T>::front;
    using std::vector<T>::back;

    using std::vector<T>::begin;
    using std::vector<T>::end;
    using std::vector<T>::size;
    using std::vector<T>::capacity;

    explicit CustomIndexVector(const std::vector<T>& vec) : std::vector<T>(vec) {}

    explicit CustomIndexVector(std::vector<T>&& vec) noexcept : std::vector<T>(std::move(vec)) {}

    typename std::vector<T>::reference operator[](IndexT idx) {
        return std::vector<T>::operator[](idx.get());
    }

    typename std::vector<T>::const_reference operator[](IndexT idx) const {
        return std::vector<T>::operator[](idx.get());
    }

    typename std::vector<T>::reference at(IndexT idx) {
        return std::vector<T>::at(idx.get());
    }

    typename std::vector<T>::const_reference at(IndexT idx) const {
        return std::vector<T>::at(idx.get());
    }
};