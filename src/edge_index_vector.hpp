#pragma once

#include <vector>

template<typename _EdgeIndexTy, typename _Ty>
class EdgeIndexVector {
public:
    typedef _EdgeIndexTy edge_index;
    typedef _Ty value_type;
    typedef std::vector<value_type> vector_type;

    EdgeIndexVector() = default;
    EdgeIndexVector(vector_type::size_type size) : mData(size) {}
    EdgeIndexVector(vector_type::size_type size, const value_type& val) : mData(size, val) {}
    EdgeIndexVector(std::initializer_list<value_type> l) : mData(l) {}
    template <typename _InputIterator>
    EdgeIndexVector(_InputIterator first, _InputIterator last) : mData(first, last) {}

    value_type& operator[](edge_index idx) { return mData[idx.val]; }
    const value_type& operator[](edge_index idx) const { return mData[idx.val]; }
    
    vector_type::size_type size() const { return mData.size(); }

    vector_type::iterator begin() { return mData.begin(); }
    vector_type::iterator end() { return mData.end(); }
    vector_type::const_iterator begin() const { return mData.begin(); }
    vector_type::const_iterator end() const { return mData.end(); }

    void push_back(const value_type& val) { mData.push_back(val); }
    void push_back(value_type&& val) { mData.push_back(std::move(val)); }
    template <typename... Args>
    void emplace_back(Args&&... args) { mData.emplace_back(std::forward<Args>(args)...); }
    void pop_back() { mData.pop_back(); }   

    void reserve(vector_type::size_type size) { mData.reserve(size); }
    void resize(vector_type::size_type size) { mData.resize(size); }
    void resize(vector_type::size_type size, const value_type& val) { mData.resize(size, val); }
    

private:
    vector_type mData;
};