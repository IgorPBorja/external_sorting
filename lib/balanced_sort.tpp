//
// Created by igor-borja on 7/15/24.
//
#pragma once

#include <vector>
#include <algorithm>
#include <numeric>
#include <utility>
#include <iostream>

#include "initial_distribution.hpp"
#include "utils.hpp"

using std::vector, std::sort, std::min, std::pair;

// creates the initial runs and distribute them into the `num_files` files
// in a alternating manner
// returns the number of writes to file
template<typename T>
int p_way_merge(
    const vector<vector<vector<T>>> &left,
    vector<vector<vector<T>>> &right
){
	// min heap of pair (value, file index)
	min_priority_queue<pair<T, int>> min_heap;
	vector<int> internal_run_ptr(left.size(), 0);  // ptr to next element in run for file i
	int writes = 0;  // writes to file/disk
	int write_file_idx = 0;
	int max_file_size = 0;
	for (const auto& file: left) {
		max_file_size = std::max(max_file_size, static_cast<int>(file.size()));
	}

	for (int run_idx = 0; run_idx < max_file_size; run_idx++){
		// min heap and run always start empty here
		vector<T> current_run;

		fill(internal_run_ptr.begin(), internal_run_ptr.end(), 0);
		int finished_runs = 0;

		// add initial elements to heap
		for (int i = 0; i < left.size(); i++) {
			if (left[i].size() > run_idx) {
				min_heap.emplace(left[i][run_idx][0], i);
				// consumed this element
				++internal_run_ptr[i];
				if (internal_run_ptr[i] == left[i][run_idx].size()) {
					++finished_runs;
				}
			} else {
				++finished_runs;
			}
		}
		while (finished_runs < left.size()) {
			auto[value, i] = min_heap.top();
			min_heap.pop();
			current_run.emplace_back(value);
			++writes;
			if (internal_run_ptr[i] < left[i][run_idx].size()) {
				min_heap.emplace(left[i][run_idx][internal_run_ptr[i]], i);
				++internal_run_ptr[i];
				if (internal_run_ptr[i] == left[i][run_idx].size()) {
					++finished_runs;
				}
			}
		}

		while (!min_heap.empty()) {
			auto[value, _] = min_heap.top();
			min_heap.pop();
			current_run.emplace_back(value);
			++writes;
		}

		right[write_file_idx].emplace_back(current_run);
		write_file_idx = (write_file_idx + 1) % right.size();
	}
	return writes;
}

template<typename T>
pair<vector<T>, double> _balanced_sort_from_initial(
	vector<vector<vector<T>>>& left,
	vector<vector<vector<T>>>& right,
	const int mem_size,
	const bool verbose
){
	// TODO: allow other output streams?
	Observer watcher(std::cout);
	const int left_files = left.size(), right_files = right.size();
	vector<int> left_idxs(left_files), right_idxs(right_files);
	std::iota(left_idxs.begin(), left_idxs.end(), 1);
	std::iota(right_idxs.begin(), right_idxs.end(), left_files + 1);

	int n = 0;
	for (const auto& file: left) {
		for (const auto& run: file) {
			n += run.size();
		}
	}
	int writes = 0;

	auto is_single_run = [](const vector<vector<vector<T>>>& left) {
		// verify if left has a single run (the first)
		bool single_run = left[0].size() == 1;
        for (int i = 1; i < left.size(); i++){
			single_run = single_run && (left[i].size() == 0);
        }
		return single_run;
	};
	while (!is_single_run(left)){
		// initial runs is first iteration
		writes += p_way_merge(left, right);
		// empty left
		left.clear();
		left.resize(left_files);
		std::swap(left, right);
		std::swap(left_idxs, right_idxs);

		if (verbose) {
			// register step
			watcher.register_step(left, left_idxs, mem_size);
		}
	}
	return {left[0][0], double(writes) / double(n)};
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

    const int left_files = (num_files + 1) / 2, right_files = num_files / 2;
	vector<vector<vector<T>>> left(left_files), right(right_files);
	vector<int> left_idxs(left_files), right_idxs(right_files);
	std::iota(left_idxs.begin(), left_idxs.end(), 1);
	std::iota(right_idxs.begin(), right_idxs.end(), left_files + 1);

	// perform initial distribution into left half
	perform_initial_distribution(data, left, mem_size);
	if (verbose) {
		watcher.register_step(left, left_idxs, mem_size);
	}

	auto[sorted_data, avg_writes] = _balanced_sort_from_initial(
		left, right, mem_size, verbose
	);
	if (verbose){
		std::cout << "final " << std::fixed << std::setprecision(2) << avg_writes << std::endl;
	}
	return sorted_data;
}