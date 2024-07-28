#pragma once

#include <vector>
#include <utility>
#include <algorithm>
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
	// merge min(n_i) complete runs from each array
	// where n_i is the number of runs in the i-th file
	std::size_t min_size = main_files[0].size();
	for (uint i = 1; i < main_files.size(); i++) {
		min_size = min(min_size, main_files[i].size());
	}

	// special heap of marked values
	min_priority_queue<MarkedValue<T>> min_heap;
	vector<T> current_run;

	// so deleting at the start becomes popping back
	reverse(main_files.begin(), main_files.end());

	for (uint i = 0; i < min_size; i++) {
		vector<vector<T>> runs_to_merge;
		for (uint j = 0; j < main_files.size(); j++) {
			runs_to_merge.emplace_back(main_files[j].back());
			main_files[j].pop_back();
		}
		vector<T> new_merged_run = merge_single_runs(runs_to_merge, mem_size);
		anchor.emplace_back(new_merged_run);
	}
	// undo reverse
	reverse(main_files.begin(), main_files.end());
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
vector<T> polyphasic_sort(vector<T> data, const int num_files, const int mem_size){
	vector<vector<vector<T>>> main_files(num_files - 1);
	vector<vector<T>> anchor_file;

	perform_initial_distribution(data, main_files, mem_size);

	auto remaining_unmerged_runs = [&main_files]() {
		uint remaining_runs = 0;
		for (const auto& file: main_files) {
			remaining_runs += file.size();
		}
		return remaining_runs;
	};

	while (remaining_unmerged_runs() > 0 || anchor_file.size() != 1) {
		polyphasic_merge(
			main_files,
			anchor_file,
			mem_size
		);
		// find the first empty file post-merge
		for (uint i = 0; i < main_files.size(); i++) {
			if (main_files[i].size() == 0) {
				swap(main_files[i], anchor_file);
				break;
			}
		}
	}
	return anchor_file[0];
}
