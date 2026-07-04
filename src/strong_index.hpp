#pragma once

#include <compare>
#include <cstddef>

template <typename T>
class StrongIndex {
public:
    StrongIndex() = default;
    constexpr explicit StrongIndex(size_t val) : mVal(val) {}

    constexpr auto operator<=>(const StrongIndex<T>& other) const = default;
    constexpr auto operator==(const StrongIndex<T>& other) const { return mVal == other.mVal; }

    constexpr auto operator<=>(size_t val) const { return mVal <=> val; }
    constexpr auto operator==(size_t val) const { return mVal == val; }

    constexpr StrongIndex<T> operator+(size_t val) const { return StrongIndex<T>(mVal + val); }
    constexpr StrongIndex<T>& operator+=(size_t val) { mVal += val; return *this; }

    constexpr StrongIndex<T>& operator++() { ++mVal; return *this; }
    constexpr StrongIndex<T> operator++(int) { auto tmp = *this; ++mVal; return tmp; }
    
    constexpr size_t get() const { return mVal; }

private:
    size_t mVal;
};