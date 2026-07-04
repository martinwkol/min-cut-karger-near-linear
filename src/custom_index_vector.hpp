#pragma once

#include <vector>

template <typename IndexT, typename T>
class CustomIndexVector : public std::vector<T> {
public:
    using std::vector<T>::vector;

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