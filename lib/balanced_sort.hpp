//
// Created by igor-borja on 7/15/24.
//

#ifndef BALANCED_SORT_H
#define BALANCED_SORT_H

#include <vector>

template<typename T>
std::vector<T> balanced_sort(std::vector<T> data, int num_files, int mem_size, bool verbose = true);

// include template implementations
#include "balanced_sort.tpp"

#endif //BALANCED_SORT_H
