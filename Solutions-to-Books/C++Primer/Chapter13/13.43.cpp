// Exercise 13.43
// Rewrite the free member to use for_each and a lambda in place of the for loop
// to destroy the elements.
// Written based on 13.40.cpp
// Xiaoyan Wang 12/30/2016
#include <algorithm>  // for for_each
#include "13.39.h"
using std::uninitialized_copy;

StrVec::StrVec(const StrVec &s) {
    auto newdata = alloc_n_copy(s.begin(), s.end());
    elements     = newdata.first;
    first_free = cap = newdata.second;
}

StrVec::StrVec(const std::initializer_list<std::string> &il) {
    auto newdata = alloc_n_copy(il.begin(), il.end());
    elements     = newdata.first;
    first_free = cap = newdata.second;
}

StrVec::~StrVec() {
    free();
}

StrVec &StrVec::operator=(const StrVec &rhs) {
    if(this == &rhs)
        return *this;

    auto data = alloc_n_copy(rhs.begin(), rhs.end());
    free();
    elements   = data.first;
    first_free = cap = data.second;
    return *this;
}

void StrVec::push_back(const string &s) {
    chk_n_alloc();  // ensure that there is room for another element
    alloc.construct(first_free++, s);
}

void StrVec::reserve(size_t n) {
    if(n < capacity())
        return;
    auto newdata = alloc.allocate(n);
    // move the data from the olde memory to the new
    auto dest = newdata;
    auto elem = elements;
    for(size_t i = 0; i != size(); ++i)
        alloc.construct(dest++, std::move(*elem++));
    free();
    // update our data structure to point to the new elements
    elements   = newdata;
    first_free = dest;
    cap        = elements + n;
}

void StrVec::resize(size_t n) {
    if(n > size()) {
        if(n < capacity())
            reserve(n);
        for(auto iter = first_free; iter - elements != n; ++iter)
            alloc.construct(iter);  // using default constructor
    } else {
        for(auto iter = first_free; iter - elements != n)
            alloc.destroy(--iter);
    }
    first_free = elements + n;
}

inline void StrVec::chk_n_alloc() {
    if(size() == capacity())
        reallocate();
}

inline std::pair<std::string *, std::string *> alloc_n_copy(
    const std::string *, const std::string *) {
    // allocate space to hold as many elements as in the range
    auto data = alloc.allocate(e - b);
    // initialize and return a pair constructed from data and the value returned
    // by uninitialized_copy
    return {data, uninitialized_copy(b, e, data)};
}

inline void StrVec::free() {
    if(!elements)
        return;
    // destroy the old element in reverse order
    // NEW: using for_each and a lambda
    std::for_each(elements, first_free, [this](std::string &rhs) {
        alloc.destroy(&rhs);
    });
    alloc.deallocate(elements, cap - elements);
}

void StrVec::reallocate() {
    auto newcapacity = size() ? 2 * size() : 1;
    // allocate new memory
    auto newdata = alloc.allocate(newcapacity);
    // move the data from the olde memory to the new
    auto dest = newdata;
    auto elem = elements;
    for(size_t i = 0; i != size(); ++i)
        alloc.construct(dest++, std::move(*elem++));
    free();
    // update our data structure to point to the new elements
    elements   = newdata;
    first_free = dest;
    cap        = elements + newcapacity;
}