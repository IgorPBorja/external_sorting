#pragma once

#include <vector>
#include <utility>
#include <algorithm>
#include <numeric>
#include <iostream>

#include "initial_distribution.hpp"
#include "cascade_sort.hpp"
#include "utils.hpp"

using std::pair, std::min;

template<typename T>
pair<vector<T>, double> _polyphasic_sort_from_initial(
	vector<vector<vector<T>>>& main_files,
	const int mem_size,
	const bool verbose
){
	constexpr int INF = std::numeric_limits<int>::max();

	Observer watcher(std::cout);
	watcher.step = 1;
	const int num_files = main_files.size();
	int anchor_idx = num_files;

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
		// find minimum of runs on non-empty, and first empty file
		int num_steps = INF, idx = -1;
		vector<int> merge_ids;
		for (int i = 0; i < main_files.size(); i++) {
			if (!main_files[i].empty()) {
				num_steps = min(num_steps, (int)main_files[i].size());
				merge_ids.emplace_back(i);
			} else if (idx == -1) {
				idx = i;
			}
		}

		// merge
		for (int i = 0; i < num_steps; i++) {
			writes += merge_single_run(main_files, merge_ids, idx, mem_size);
		}

		// HACK: so it considers empty anchor file when redistributing
		// does not need to count those writes as they don't really exist
		writes += redistribute_if_needed(main_files);

		// register
		if (verbose) {
			watcher.register_step(main_files, mem_size);
		}
	}
	// find non-empty file, guaranteed to be the single run of the dataset
	int final_file = -1;
	for (int i = 0; i < main_files.size(); i++) {
		if (!main_files[i].empty()) {
			final_file = i;
			break;
		}
	}
	return {main_files[final_file][0], double(writes) / double(n)};
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
	vector<vector<vector<T>>> files(num_files - 1);

	perform_initial_distribution(data, files, mem_size);
	if (verbose) {
		watcher.register_step(files, mem_size);
	}
	// add file for merging
	files.emplace_back();

	auto[sorted_data, avg_writes] = _polyphasic_sort_from_initial(
		files, mem_size, verbose
	);

	if (verbose){
		std::cout << "final " << std::fixed << std::setprecision(2) << avg_writes << std::endl;
	}
	return sorted_data;
}