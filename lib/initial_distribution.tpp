#pragma once

#include <vector>
#include <algorithm>
#include <cassert>
#include "utils.hpp"

using std::vector;

// do initial distribution of records
template<typename T>
void perform_initial_distribution(
	vector<T> data,
	vector<vector<vector<T>>> &main_files,
	const int mem_size
) {
	assert(mem_size > 1);

	// special heap of marked values
	min_priority_queue<MarkedValue<T>> min_heap;
	vector<T> current_run;
	int file_idx = 0, marked_cnt = 0;
	const int p = main_files.size();

	for (int i = 0; i < data.size(); i++){
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

