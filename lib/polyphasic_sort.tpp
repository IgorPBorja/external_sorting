#pragma once

#include <vector>
#include <utility>
#include <algorithm>
#include <numeric>

#include "utils.hpp"

using std::pair, std::min;

template<typename T>
vector<T> merge_single_runs(
	vector<vector<T>> &runs,
	const uint mem_size
) {
	// assert it can hold at least one element from each run
	assert(mem_size >= runs.size());
	vector<T> result;
	vector<uint> ptrs(runs.size(), 0);

	min_priority_queue<pair<T, uint>> min_heap;
	for (uint i = 0; i < runs.size(); i++) {
		if (runs[i].size() > 0) {
			min_heap.emplace(runs[i][0], i);
			++ptrs[i];
		}
	}

	while (true) {
		// verify if all runs were fully consumed
		bool all_runs_consumed = true;
		for (uint i = 0; i < runs.size(); i++) {
			all_runs_consumed &= (ptrs[i] >= runs[i].size());
		}
		if (all_runs_consumed) {
			break;
		}

		// remove one element
		auto[value, idx] = min_heap.top();
		min_heap.pop();
		// add to merge
		result.emplace_back(value);

		// refill heap
		if (ptrs[idx] < runs[idx].size()) {
			min_heap.emplace(runs[idx][ptrs[idx]], idx);
			++ptrs[idx];
		}
	}

	// just remove from the min_heap and continue
	while (!min_heap.empty()) {
		auto[value, _] = min_heap.top();
		min_heap.pop();
		result.emplace_back(value);
	}
	return result;
}

template<typename T>
void polyphasic_merge(
	vector<vector<vector<T>>> &main_files,
	vector<vector<T>> &anchor,
	const uint mem_size
){
	assert(mem_size > 1);

	// list of pairs (i, j)
	// that means that run j of file i will be merged in this batch
	vector<pair<uint, uint>> active_run_indices;
	for (uint i = 0; i < main_files.size(); i++) {
		if (main_files[i].size() > 0) {
			active_run_indices.emplace_back(i, 0);
		}
	}

	while (!active_run_indices.empty()){
		vector<vector<T>> runs_to_merge;
		vector<pair<uint, uint>> new_active_run_indices;
		for (auto[i, j]: active_run_indices) {
			runs_to_merge.emplace_back(main_files[i][j]);
			if (j + 1 < main_files[i].size()) {
				new_active_run_indices.emplace_back(i, j + 1);
			}
		}
		if (runs_to_merge.size() > 1) {
			vector<T> new_merged_run = merge_single_runs(runs_to_merge, mem_size);
			anchor.emplace_back(new_merged_run);
		} else {
			anchor.emplace_back(runs_to_merge[0]);
		}
		active_run_indices = new_active_run_indices;
	}

	// clear all files now!
	for (vector<vector<T>>& file: main_files) {
		file.clear();
	}
}

// do initial distribution of records
template<typename T>
void perform_initial_distribution(
	vector<T> data,
	vector<vector<vector<T>>> &main_files,
	const uint mem_size
) {
	assert(mem_size > 1);

	// special heap of marked values
	min_priority_queue<MarkedValue<T>> min_heap;
	vector<T> current_run;
	uint file_idx = 0, marked_cnt = 0;
	const uint p = main_files.size();

	for (uint i = 0; i < data.size(); i++){
		const T& x = data[i];
		if (i < mem_size){
            min_heap.emplace(x);
            continue;
		}

        if (marked_cnt == mem_size) {
            // that means all values are marked, so unmark and start a new run
        	main_files[file_idx].emplace_back(current_run);
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
		if (x < current_run.back()) {  // breaks order (smaller and comes after) -> push marked
			min_heap.emplace(x, true);
			++marked_cnt;
		} else {
			min_heap.emplace(x, false);
		}
	}

	// unmark leftover data
	// and reset current run (can't mix with marked data without breaking order)
	if (marked_cnt > 0) {
		main_files[file_idx].emplace_back(current_run);
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
		main_files[file_idx].emplace_back(current_run);
	}
}

template<typename T>
vector<T> polyphasic_sort(
	vector<T> data,
	const int num_files,
	const int mem_size,
	const bool verbose
){
	// TODO: allow other output streams?
	Observer watcher(std::cout);
	vector<vector<vector<T>>> main_files(num_files - 1);
	vector<uint> main_idxs(num_files - 1);
	std::iota(main_idxs.begin(), main_idxs.end(), 1);
	vector<vector<T>> anchor_file;
	uint anchor_idx = num_files;

	perform_initial_distribution(data, main_files, mem_size);
	if (verbose) {
		watcher.register_step(main_files, main_idxs, mem_size);
	}

	auto remaining_runs = [&main_files]() {
		uint runs = 0;
		for (const auto& file: main_files) {
			runs += file.size();
		}
		return runs;
	};

	// Procedure:
	// If there is a single run, it will be in T[0] so return it and stop
	// else: merge (T[1],..., T[n-1]) completely into single tape T[n]
	// swap T[1] and T[n] (it is just a reference swap, inexpensive)
	// distribute floor(1/(n-1)) of the runs in T[1] to T[i] for all i=2...n-1
	while (remaining_runs() > 1) {
		polyphasic_merge(
			main_files,
			anchor_file,
			mem_size
		);

		std::swap(main_files[0], anchor_file);
		std::swap(main_idxs[0], anchor_idx);

		// so that pop_back() becomes pop_front()
		reverse(main_files[0].begin(), main_files[0].end());

		// distribute runs from main_files[0]
		// num_files is >= 3 so run_amount is == 0 when there is only a single run in main_files[0]
		// (process finished)
		const uint run_amount = main_files[0].size() / (num_files - 1);
		for (uint i = 1; i < main_files.size(); i++) {
			for (uint j = 0; j < run_amount; j++) {
				main_files[i].emplace_back(main_files[0].back());
				main_files[0].pop_back();
			}
		}

		// unreverse
		reverse(main_files[0].begin(), main_files[0].end());

		// register
		if (verbose) {
			watcher.register_step(main_files, main_idxs, mem_size);
		}
	}
	return main_files[0][0];
}
