//
// Created by igor-borja on 7/28/24.
//

#ifndef POLYPHASIC_SORT_HPP
#define POLYPHASIC_SORT_HPP

#include <vector>

template<typename T>
std::vector<T> polyphasic_sort(std::vector<T> data, int num_files, int mem_size, bool verbose = true);

// include template implementations
#include "polyphasic_sort.tpp"

#endif //POLYPHASIC_SORT_HPP
