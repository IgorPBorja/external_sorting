//
// Created by igor-borja on 8/15/24.
//

#ifndef CASCADE_SORT_HPP
#define CASCADE_SORT_HPP

#include <vector>

template<typename T>
std::vector<T> cascade_sort(std::vector<T> data, int num_files, int mem_size, bool verbose = true);

// include template implementations
#include "cascade_sort.tpp"

#endif //CASCADE_SORT_HPP
