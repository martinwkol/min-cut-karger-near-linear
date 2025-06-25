#pragma once

#define DEFINE_EDGE_INDEX_CLASS(CLASS_NAME) \
class CLASS_NAME {    \
public: \
    CLASS_NAME() = default;                            \
    explicit CLASS_NAME(size_t val) : mVal(val) {}    \
    \
    bool operator==(const CLASS_NAME& index) { return mVal == index.mVal; }  \
    bool operator!=(const CLASS_NAME& index) { return mVal != index.mVal; }  \
    bool operator<(const CLASS_NAME& index) { return mVal < index.mVal; }    \
    bool operator>(const CLASS_NAME& index) { return mVal > index.mVal; }    \
    bool operator<=(const CLASS_NAME& index) { return mVal <= index.mVal; }  \
    bool operator>=(const CLASS_NAME& index) { return mVal >= index.mVal; }  \
    \
    bool operator<(size_t v) { return mVal < v; }    \
    bool operator>(size_t v) { return mVal > v; }    \
    bool operator<=(size_t v) { return mVal <= v; }  \
    bool operator>=(size_t v) { return mVal >= v; }  \
    \
    CLASS_NAME operator+(size_t i) const { return CLASS_NAME(mVal + i); }  \
    CLASS_NAME& operator+=(size_t i) { mVal += i; return *this; }           \
    CLASS_NAME& operator++() { ++mVal; return *this; }                      \
    CLASS_NAME operator++(int) { auto tmp = *this; ++(*this); return tmp; }\
    \
    constexpr size_t get() const { return mVal; } \
    \
private: \
    size_t mVal; \
}
