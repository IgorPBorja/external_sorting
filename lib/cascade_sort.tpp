//
// Created by igor-borja on 8/15/24.
//
#pragma once
#include <iostream>
#include <utility>
#include <vector>

#include "initial_distribution.tpp"
#include "utils.hpp"

using std::vector, std::pair, std::make_pair;

constexpr size_t INF = 2e9;

// merge a single run from each select file
// returns number of writes
template<typename T>
int merge_single_run(
    vector<vector<vector<T>>>& files,
    const vector<int> merge_ids,
    int output_id,
    const int mem_size
) {
    int writes = 0;
    min_priority_queue<pair<T, int>> min_heap;

    vector<T> run;
    // merge
    vector<int> ptrs(files.size(), 0);
    for (auto id: merge_ids) {
        T item = files[id][0][0];
        min_heap.push(make_pair(item, id));
        ++ptrs[id];
    }

    while (!min_heap.empty()) {
        auto[item, id] = min_heap.top();
        min_heap.pop();
        run.emplace_back(item);
        ++writes;
        if (ptrs[id] < files[id][0].size()) {
            T new_item = files[id][0][ptrs[id]];
            min_heap.push(make_pair(new_item, id));
            ++ptrs[id];
        }
    }
    files[output_id].emplace_back(run);

    for (auto id : merge_ids) {
        files[id].erase(files[id].begin());
    }
    return writes;
}

// single step
// returns number of writes
template<typename T>
int merge_step(
    vector<vector<vector<T>>>& files,
    const int mem_size
) {
    vector<int> merge_ids;
    int output_id = -1;
    size_t min_merge_steps = INF;
    for (size_t i = 0; i < files.size(); ++i) {
        if (files[i].size() == 0 && output_id == -1) {
            output_id = i;
        }
        else if (files[i].size() > 0) {
            merge_ids.emplace_back(i);
            min_merge_steps = std::min(min_merge_steps, files[i].size());
        }
    }

    int writes = 0;
    while (!merge_ids.empty()) {
        for (int i = 0; i < min_merge_steps; ++i) {
            writes += merge_single_run(files, merge_ids, output_id, mem_size);
        }
        vector<int> remaining_merge_ids;
        output_id = -1;
        min_merge_steps = INF;
        for (auto id: merge_ids) {
            if (files[id].size() > 0) {
                remaining_merge_ids.emplace_back(id);
                min_merge_steps = std::min(min_merge_steps, files[id].size());
            } else if (files[id].size() == 0 && output_id == -1) {
                output_id = id;
            }
        }
        merge_ids = remaining_merge_ids;
    }
    return writes;
}

template<typename T>
bool is_finished(const vector<vector<T>> &files) {
    bool finished = true;
    int num_runs = 0;
    for (size_t i = 0; i < files.size(); ++i) {
        num_runs += files[i].size();
    }
    return num_runs == 1;
}

// if there is a single file with many runs, returns its index, else 0
template<typename T>
int stuck_file(const vector<vector<T>> &files) {
    int idx = -1;
    for (size_t i = 0; i < files.size(); ++i) {
        if (files[i].size() == 1) {
            return -1;
        } else if (files[i].size() > 1) {
            if (idx != -1) {
                // multiple files
                return -1;
            }
            idx = i;
        }
    }
    return idx;
}

// returns (sorted_vec, avg writes)
template<typename T>
pair<vector<T>, double> _cascade_sort_from_initial(
    vector<vector<vector<T>>>& files,
    const int mem_size,
    const bool verbose
) {
    int writes = 0;
    int n = 0;
    for (const auto& file: files) {
        for (const auto& run: file) {
            n += run.size();
        }
    }
    const int num_files = files.size();
    Observer watcher(std::cout);

    while (!is_finished(files)) {
        writes += merge_step(files, mem_size);
        if (verbose) {
            watcher.register_step(files, mem_size);
        }
        int stuck_file_idx = stuck_file(files);
        if (stuck_file_idx >= 0) {
            // redistribute more or less evenly between the other num_files-1 files
            vector<vector<T>>& stuck_f = files[stuck_file_idx];
            std::reverse(stuck_f.begin(), stuck_f.end());
            const int base = stuck_f.size() / (num_files - 1), remainder = stuck_f.size() % (num_files - 1);
            int j = 0;
            for (int f_idx = 0; f_idx < files.size(); f_idx++) {
                if (f_idx == stuck_file_idx) {
                    continue;
                }
                for (int cnt = 0; cnt < base + (j < remainder); cnt++) {
                    files[f_idx].emplace_back(stuck_f.back());
                    stuck_f.pop_back();
                }
                ++j;
            }
            // all data was moved
            writes += n;
        }
    }
    // find last run
    for (int i = 0; i < num_files; ++i) {
        if (files[i].size() > 0) {
            return {files[i][0], double(writes) / double(n)};
        }
    }
}

template<typename T>
vector<T> cascade_sort(
    const vector<T> data, const int num_files,
    const int mem_size, const bool verbose
) {
    // initial runs
    vector<vector<vector<T>>> files(num_files - 1);
    Observer watcher(std::cout);
    perform_initial_distribution(data, files, mem_size);
    // add extra file for merging
    files.emplace_back();
    watcher.register_step(files, mem_size);

    auto[sorted_data, avg_writes] = _cascade_sort_from_initial(
        files, mem_size, writes, verbose
    );

    // print final average
    if (verbose) {
        std::cout << "final " << std::fixed << std::setprecision(2) << avg_writes << std::endl;
    }
    return sorted_data;
}
