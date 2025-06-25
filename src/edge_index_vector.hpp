#pragma once

#include <vector>
#include "graph.hpp"

template<typename _EdgeIndexTy, typename _Ty>
class EdgeIndexVector {
public:
    typedef _EdgeIndexTy edge_index;
    typedef _Ty value_type;
    typedef std::vector<value_type> vector_type;

    EdgeIndexVector() = default;
    EdgeIndexVector(vector_type::size_type size) : mData(size) {}
    EdgeIndexVector(vector_type::size_type size, const value_type& val) : mData(size, val) {}

    value_type& operator[](edge_index idx) { return mData[idx.val]; }
    const value_type& operator[](edge_index idx) const { return mData[idx.val]; }
    
    vector_type::size_type size() const { return mData.size(); }

    void push_back(const value_type& val) { mData.push_back(val); }
    void push_back(value_type&& val) { mData.push_back(std::move(val)); }
    template <typename... Args>
    void emplace_back(Args&&... args) { mData.emplace_back(std::forward<Args>(args)...); }
    void pop_back() { mData.pop_back(); }   

private:
    vector_type mData;
};