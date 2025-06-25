#pragma once

#include <vector>

template<typename _EdgeIndexTy, typename _Ty>
class EdgeIndexVector {
public:
    typedef _EdgeIndexTy                            edge_index;
    typedef std::vector<_Ty>                        vector_type;
    typedef typename vector_type::value_type        value_type;
    typedef typename vector_type::reference         reference;
    typedef typename vector_type::const_reference   const_reference;
    typedef typename vector_type::pointer           pointer;
    typedef typename vector_type::const_pointer     const_pointer;
    
    typedef typename vector_type::size_type         size_type;
    typedef typename vector_type::iterator          iterator;
    typedef typename vector_type::const_iterator    const_iterator;

    EdgeIndexVector() = default;
    explicit EdgeIndexVector(const vector_type& vec)            : mData(vec) {}
    explicit EdgeIndexVector(vector_type&& vec)                 : mData(std::move(vec)) {}
    explicit EdgeIndexVector(size_type __n) : mData(__n)        {}
    EdgeIndexVector(size_type __n, const value_type& val)       : mData(__n, val) {}
    EdgeIndexVector(std::initializer_list<value_type> l)        : mData(l) {}
    template <typename _InputIterator>
    EdgeIndexVector(_InputIterator first, _InputIterator last)  : mData(first, last) {}

    reference operator[](edge_index idx)                { return mData[idx.get()]; }
    const_reference operator[](edge_index idx) const    { return mData[idx.get()]; }
    
    reference front()                   { return mData.front(); }
    const_reference front() const       { return mData.front(); }
    reference back()                    { return mData.back(); }
    const_reference back() const        { return mData.back(); }

    pointer data() {
        if constexpr (std::is_same_v<_Ty, bool>) {
            static_assert(!std::is_same_v<_Ty, bool>, "EdgeIndexVector<bool>::data() is unsafe. Use std::vector<char> or std::vector<uint8_t> instead.");
        } else {
            return mData.data(); 
        }
    }
    const_pointer data() const { 
        if constexpr (std::is_same_v<_Ty, bool>) {
            static_assert(!std::is_same_v<_Ty, bool>, "EdgeIndexVector<bool>::data() is unsafe. Use std::vector<char> or std::vector<uint8_t> instead.");
        } else {
            return mData.data(); 
        }
    }
    size_type size() const          { return mData.size(); }

    iterator begin()                { return mData.begin(); }
    iterator end()                  { return mData.end(); }
    const_iterator begin() const    { return mData.begin(); }
    const_iterator end() const      { return mData.end(); }

    void push_back(const value_type& val)   { mData.push_back(val); }
    void push_back(value_type&& val)        { mData.push_back(std::move(val)); }
    template <typename... Args>
    void emplace_back(Args&&... args)       { mData.emplace_back(std::forward<Args>(args)...); }
    void pop_back()                         { mData.pop_back(); }   

    void reserve(size_type __n)                         { mData.reserve(__n); }
    void resize(size_type __n)                          { mData.resize(__n); }
    void resize(size_type __n, const value_type& val)   { mData.resize(__n, val); }
    

private:
    vector_type mData;
};