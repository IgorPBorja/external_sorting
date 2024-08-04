//
// Created by igor-borja on 7/15/24.
//

#ifndef INITIAL_DISTRIBUTION_H
#define INITIAL_DISTRIBUTION_H

#include <vector>
#include <algorithm>

template<typename T>
void perform_initial_distribution(
	std::vector<T> data,
	std::vector<std::vector<std::vector<T>>> &main_files,
	int mem_size
);

// include template implementations
#include "initial_distribution.tpp"

#endif //INITIAL_DISTRIBUTION_H
