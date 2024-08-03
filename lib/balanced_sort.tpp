//
// Created by igor-borja on 7/15/24.
//
#pragma once

#include <vector>
#include <algorithm>
#include <numeric>
#include <utility>

#include "utils.hpp"

using std::vector, std::sort, std::min, std::pair;

// creates the initial runs and distribute them into the `num_files` files
// in a alternating manner
// returns a vector of `num_files` elements of type vector<vector<T>> (vectors of runs)
template<typename T>
void p_way_merge(
    const vector<vector<vector<T>>> &left,
    vector<vector<vector<T>>> &right,
    uint mem_size
){
	assert(mem_size > 1);

	// special heap of marked values
	min_priority_queue<MarkedValue<T>> min_heap;
	vector<T> current_run;
	uint file_idx = 0, marked_cnt = 0;
	uint p = right.size();
	uint i = 0;
	AlternatingIterator<T> it(left);

	for (; !it.ended(); ++it, ++i){
		if (i < mem_size){
            min_heap.emplace(it.value());
            continue;
		}

        if (marked_cnt == mem_size) {
            // that means all values are marked, so unmark and start a new run
        	right[file_idx].emplace_back(current_run);
        	current_run = vector<T>();
            file_idx = (file_idx + 1) % p;
            unmark_all(min_heap);
        	marked_cnt = 0;
        }
		// make room for new data (removing 1 from heap)
        MarkedValue<T> min_key = min_heap.top();
        T curr_value = min_key.val;
        current_run.push_back(curr_value);
        min_heap.pop();
		// push new data (1 entry)
		if (it.value() < current_run.back()) {  // breaks order (smaller and cames after) -> push marked
			min_heap.emplace(it.value(), true);
			++marked_cnt;
		} else {
			min_heap.emplace(it.value(), false);
		}
	}

	// unmark leftover data
	// and reset current run (can't mix with marked data without breaking order)
	if (marked_cnt > 0) {
		right[file_idx].emplace_back(current_run);
		current_run = vector<T>();
		file_idx = (file_idx + 1) % p;
		unmark_all(min_heap);
		marked_cnt = 0;
	}
	// distribute leftover data
	while (!min_heap.empty()) {
		MarkedValue<T> min_key = min_heap.top();
		min_heap.pop();
		current_run.push_back(T(min_key));
	}
	if (!current_run.empty()) {
		// register last run
		right[file_idx].emplace_back(current_run);
	}
}

template<typename T>
vector<T> balanced_sort(
	const vector<T> data,
	const int num_files,
	const int mem_size,
	const bool verbose
){
	// TODO: allow other output streams?
	Observer watcher(std::cout);

    const uint left_files = (num_files + 1) / 2, right_files = num_files / 2;
	vector<vector<vector<T>>> left(left_files), right(right_files);
	vector<uint> left_idxs(left_files), right_idxs(right_files);
	std::iota(left_idxs.begin(), left_idxs.end(), 1);
	std::iota(right_idxs.begin(), right_idxs.end(), left_files + 1);

	left[0].push_back(vector<T>()); // first run
	for (const T x: data){
		left[0][0].emplace_back(x);
	}
	bool single_run;
	do {
		// initial runs is first iteration
		p_way_merge(left, right, mem_size);
		// empty left
		left.clear();
		left.resize(left_files);
		std::swap(left, right);
		std::swap(left_idxs, right_idxs);

		if (verbose) {
			// register step
			watcher.register_step(left, left_idxs, mem_size);
		}

		// verify if left has a single run (the first)
		single_run = left[0].size() == 1;
        for (uint i = 1; i < left.size() && single_run; i++){
			single_run = single_run && left[i].size() == 0;
        }
	} while (!single_run);

	vector<T> sorted_data(data.size());
	for (uint i = 0; i < data.size(); i++){
        sorted_data[i] = left[0][0][i];
	}
	return sorted_data;
}
