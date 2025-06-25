#pragma once

#include <vector>

template<typename _EdgeIndexTy, typename _Ty>
class EdgeIndexVector {
public:
    typedef _EdgeIndexTy edge_index;
    typedef _Ty value_type;
    typedef std::vector<value_type> vector_type;
    typedef typename vector_type::size_type size_type;
    typedef typename vector_type::iterator iterator;
    typedef typename vector_type::const_iterator const_iterator;

    EdgeIndexVector() = default;
    EdgeIndexVector(size_type __n) : mData(__n) {}
    EdgeIndexVector(size_type __n, const value_type& val) : mData(__n, val) {}
    EdgeIndexVector(std::initializer_list<value_type> l) : mData(l) {}
    template <typename _InputIterator>
    EdgeIndexVector(_InputIterator first, _InputIterator last) : mData(first, last) {}

    value_type& operator[](edge_index idx) { return mData[idx.val]; }
    const value_type& operator[](edge_index idx) const { return mData[idx.val]; }
    
    value_type* data() { return mData.data(); }
    const value_type* data() const { return mData.data(); }
    size_type size() const { return mData.size(); }

    iterator begin() { return mData.begin(); }
    iterator end() { return mData.end(); }
    const_iterator begin() const { return mData.begin(); }
    const_iterator end() const { return mData.end(); }

    void push_back(const value_type& val) { mData.push_back(val); }
    void push_back(value_type&& val) { mData.push_back(std::move(val)); }
    template <typename... Args>
    void emplace_back(Args&&... args) { mData.emplace_back(std::forward<Args>(args)...); }
    void pop_back() { mData.pop_back(); }   

    void reserve(size_type __n) { mData.reserve(__n); }
    void resize(size_type __n) { mData.resize(__n); }
    void resize(size_type __n, const value_type& val) { mData.resize(__n, val); }
    

private:
    vector_type mData;
};