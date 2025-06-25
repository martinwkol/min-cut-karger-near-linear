#pragma once

#define DEFINE_EDGE_INDEX_STRUCT(STRUCT_NAME) \
struct STRUCT_NAME {    \
    size_t val;         \
    \
    STRUCT_NAME() = default;                            \
    explicit STRUCT_NAME(size_t _val) : val(_val) {}    \
    \
    bool operator==(const STRUCT_NAME& index) { return val == index.val; }  \
    bool operator!=(const STRUCT_NAME& index) { return val != index.val; }  \
    bool operator<(const STRUCT_NAME& index) { return val < index.val; }    \
    bool operator>(const STRUCT_NAME& index) { return val > index.val; }    \
    bool operator<=(const STRUCT_NAME& index) { return val <= index.val; }  \
    bool operator>=(const STRUCT_NAME& index) { return val >= index.val; }  \
    \
    bool operator<(size_t v) { return val < v; }    \
    bool operator>(size_t v) { return val > v; }    \
    bool operator<=(size_t v) { return val <= v; }  \
    bool operator>=(size_t v) { return val >= v; }  \
    \
    STRUCT_NAME operator+(size_t i) { return STRUCT_NAME(val + i); }        \
    STRUCT_NAME& operator+=(size_t i) { val += i; return *this; }           \
    STRUCT_NAME& operator++() { ++val; return *this; }                      \
    STRUCT_NAME operator++(int) { auto tmp = *this; ++(*this); return tmp; }\
}