#pragma once

#include <vector>
#include <utility>
#include <algorithm>
#include <numeric>
#include <iostream>

#include "initial_distribution.hpp"
#include "utils.hpp"

using std::pair, std::min;

template<typename T>
vector<T> merge_single_runs(
	vector<vector<T>> &runs,
	const int mem_size
) {
	vector<T> result;
	vector<int> ptrs(runs.size(), 0);

	min_priority_queue<pair<T, int>> min_heap;

	// fill heap
	int completed_runs = 0, i = 0;
	for (const auto &run : runs) {
		if (run.empty()) ++completed_runs;
	}
	while (completed_runs < runs.size() && min_heap.size() < mem_size) {
		if (ptrs[i] < runs[i].size()) {
			min_heap.emplace(runs[i][ptrs[i]], i);
			++ptrs[i];
			if (ptrs[i] == runs[i].size()) {
				++completed_runs;
			}
		}
		i = (i + 1) % runs.size();
	}

	while (true) {
		// verify if all runs were fully consumed
		bool all_runs_consumed = true;
		for (int i = 0; i < runs.size(); i++) {
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

// returns number of writes
template<typename T>
int polyphasic_merge(
	vector<vector<vector<T>>> &main_files,
	vector<vector<T>> &anchor,
	const int mem_size
){
	assert(mem_size > 1);
	assert(!main_files.empty());
	constexpr int INF = std::numeric_limits<int>::max();

	int writes = 0;

	// list of pairs (i, j)
	// that means that run j of file i will be merged in this batch
	vector<int> active_run_indices;
	int num_steps = INF;
	for (int i = 0; i < main_files.size(); i++) {
		if (main_files[i].size() > 0) {
			active_run_indices.emplace_back(i);
			num_steps = min(num_steps, (int)main_files[i].size());
		}
	}

	// for performance
	for (int i: active_run_indices) {
		std::reverse(main_files[i].begin(), main_files[i].end());
	}

	for (int _ = 0; _ < num_steps; _++) {
		vector<vector<T>> runs_to_merge;
		vector<pair<int, int>> new_active_run_indices;
		for (int i: active_run_indices) {
			runs_to_merge.emplace_back(main_files[i].back());
			main_files[i].pop_back();
		}
		if (runs_to_merge.size() > 1) {
			vector<T> new_merged_run = merge_single_runs(runs_to_merge, mem_size);
			writes += new_merged_run.size();
			anchor.emplace_back(new_merged_run);
		} else {
			writes += runs_to_merge[0].size();
			anchor.emplace_back(runs_to_merge[0]);
		}
	}

	// undo
	for (int i: active_run_indices) {
		std::reverse(main_files[i].begin(), main_files[i].end());
	}
	return writes;
}

template<typename T>
pair<vector<T>, double> _polyphasic_sort_from_initial(
	vector<vector<vector<T>>>& main_files,
	vector<vector<T>>& anchor_file,
	const int mem_size,
	const bool verbose
){
	Observer watcher(std::cout);
	watcher.step = 1;
	const int num_files = main_files.size() + 1;
	int anchor_idx = num_files;
	vector<int> main_idxs(num_files - 1);
	std::iota(main_idxs.begin(), main_idxs.end(), 1);

	int n = 0;
	for (const auto& file: main_files) {
		for (const auto& run: file) n += run.size();
	}
	int writes = 0;

	auto remaining_runs = [&main_files]() {
		int runs = 0;
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
		writes += polyphasic_merge(
			main_files,
			anchor_file,
			mem_size
		);

		// find first empty file
		int idx;
		for (int i = 0; i < main_files.size(); i++) {
			if (main_files[i].empty()) {
				std::swap(main_files[i], anchor_file);
				std::swap(main_idxs[i], anchor_idx);
				idx = i;
				break;
			}
		}

		// only main_files[idx] is occupied and with > 1 runs
		// Solution: distribute runs from main_files[idx]
		// num_files is >= 3 so run_amount is == 0 when there is only a single run in main_files[0]
		// (process finished)
		if (remaining_runs() == main_files[idx].size() && main_files[idx].size() > 1) {
			// so that pop_back() becomes pop_front()
			reverse(main_files[idx].begin(), main_files[idx].end());
			const int run_amount = main_files[idx].size() / (num_files);
			const int remainder = main_files[idx].size() % (num_files);
			for (int i = 0; i < main_files.size(); i++) {
				if (i == idx) continue;
				const int extra_run = (i < remainder) ? 1 : 0;
				for (int j = 0; j < run_amount + extra_run; j++) {
					writes += main_files[idx].back().size();
					main_files[i].emplace_back(main_files[idx].back());
					main_files[idx].pop_back();
				}
			}
			// unreverse
			reverse(main_files[0].begin(), main_files[0].end());
			// register
			if (verbose) {
				watcher.register_step(main_files, main_idxs, mem_size);
			}
		}
		// register
		if (verbose) {
			watcher.register_step(main_files, main_idxs, mem_size);
		}
	}
	return {main_files[0][0], double(writes) / double(n)};
}

int calculateTotalSize(const std::vector<std::vector<std::vector<int>>>& v) {
    int total_size = 0;
    for (const auto& outer : v) {
        for (const auto& inner : outer) {
            total_size += inner.size();
        }
    }
    return total_size;
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
	vector<int> main_idxs(num_files - 1);
	std::iota(main_idxs.begin(), main_idxs.end(), 1);
	vector<vector<T>> anchor_file;

	perform_initial_distribution(data, main_files, mem_size);
	if (verbose) {
		watcher.register_step(main_files, main_idxs, mem_size);
	}

	auto[sorted_data, avg_writes] = _polyphasic_sort_from_initial(
		main_files, anchor_file, mem_size, verbose
	);

	if (verbose){
		std::cout << "final " << std::fixed << std::setprecision(2) << avg_writes << std::endl;
	}
	return sorted_data;
}