#pragma once

#define DEFINE_EDGE_INDEX_STRUCT(STRUCT_NAME) \
class STRUCT_NAME {    \
public: \
    STRUCT_NAME() = default;                            \
    explicit STRUCT_NAME(size_t val) : mVal(val) {}    \
    \
    bool operator==(const STRUCT_NAME& index) { return mVal == index.mVal; }  \
    bool operator!=(const STRUCT_NAME& index) { return mVal != index.mVal; }  \
    bool operator<(const STRUCT_NAME& index) { return mVal < index.mVal; }    \
    bool operator>(const STRUCT_NAME& index) { return mVal > index.mVal; }    \
    bool operator<=(const STRUCT_NAME& index) { return mVal <= index.mVal; }  \
    bool operator>=(const STRUCT_NAME& index) { return mVal >= index.mVal; }  \
    \
    bool operator<(size_t v) { return mVal < v; }    \
    bool operator>(size_t v) { return mVal > v; }    \
    bool operator<=(size_t v) { return mVal <= v; }  \
    bool operator>=(size_t v) { return mVal >= v; }  \
    \
    STRUCT_NAME operator+(size_t i) const { return STRUCT_NAME(mVal + i); }  \
    STRUCT_NAME& operator+=(size_t i) { mVal += i; return *this; }           \
    STRUCT_NAME& operator++() { ++mVal; return *this; }                      \
    STRUCT_NAME operator++(int) { auto tmp = *this; ++(*this); return tmp; }\
    \
    constexpr size_t get() const { return mVal; } \
    \
private: \
    size_t mVal; \
}
