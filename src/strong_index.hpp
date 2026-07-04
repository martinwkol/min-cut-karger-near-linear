#pragma once

#include <compare>
#include <cstddef>

#define DEFINE_EDGE_INDEX_CLASS(CLASS_NAME) \
class CLASS_NAME { \
public: \
    CLASS_NAME() = default;                         \
    constexpr explicit CLASS_NAME(size_t val) : mVal(val) {}  \
    \
    constexpr bool operator==(const CLASS_NAME& index) { return mVal == index.mVal; } \
    constexpr bool operator!=(const CLASS_NAME& index) { return mVal != index.mVal; } \
    constexpr bool operator<(const CLASS_NAME& index) { return mVal < index.mVal; }   \
    constexpr bool operator>(const CLASS_NAME& index) { return mVal > index.mVal; }   \
    constexpr bool operator<=(const CLASS_NAME& index) { return mVal <= index.mVal; } \
    constexpr bool operator>=(const CLASS_NAME& index) { return mVal >= index.mVal; } \
    \
    constexpr bool operator<(size_t v) { return mVal < v; }    \
    constexpr bool operator>(size_t v) { return mVal > v; }    \
    constexpr bool operator<=(size_t v) { return mVal <= v; }  \
    constexpr bool operator>=(size_t v) { return mVal >= v; }  \
    \
    constexpr CLASS_NAME operator+(size_t i) const { return CLASS_NAME(mVal + i); }   \
    constexpr CLASS_NAME& operator+=(size_t i) { mVal += i; return *this; }           \
    constexpr CLASS_NAME& operator++() { ++mVal; return *this; }                      \
    constexpr CLASS_NAME operator++(int) { auto tmp = *this; ++(*this); return tmp; } \
    \
    constexpr size_t get() const { return mVal; } \
    \
private: \
    size_t mVal; \
}

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