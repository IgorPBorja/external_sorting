//
// Created by igor-borja on 7/28/24.
//

#ifndef UTILS_H
#define UTILS_H

#include <vector>

using std::vector;
template<typename T>
using min_priority_queue = std::priority_queue<T, vector<T>, std::greater<T>>;

template<typename T>
struct AlternatingIterator {
    const vector<vector<vector<T>>>& files_ref;
    uint n;
    uint curr_file = 0;
	// respectively: index of run, index inside run
    vector<uint> run_index;
    vector<uint> inner_index;
    uint completed_files = 0;

	AlternatingIterator(
        const vector<vector<vector<T>>> &files
	)  : files_ref(files), n(files.size()) {
		run_index.assign(n, 0);
		inner_index.assign(n, 0);
		// empty files
		for (const auto& file: files){
			if (file.size() == 0){
				++completed_files;
			}
		}
	}

	T value() {
        return files_ref[curr_file][run_index[curr_file]][inner_index[curr_file]];
	}

	void operator++(){
        ++inner_index[curr_file];
        if (inner_index[curr_file] == files_ref[curr_file][run_index[curr_file]].size()){
			// ended run
			++run_index[curr_file];
			inner_index[curr_file] = 0;
        }

        // completed files with this?
        if (run_index[curr_file] == files_ref[curr_file].size()){
        	++completed_files;
        }
        if (!this->ended()){
			// TODO add exception if ended
			curr_file = (curr_file + 1) % n;
			// finished processing
			uint skipped = 0;
			while (run_index[curr_file] == files_ref[curr_file].size() && skipped < n){
				curr_file = (curr_file + 1) % n;
				++skipped;
			}
        }
	}

	T operator++(int) {
		T tmp = this->value();
		++(*this);
		return tmp;
	}

	bool ended(){
		return completed_files == n;
	}
};

#endif //UTILS_H
