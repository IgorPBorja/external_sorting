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

// struct to represent a value with (possibly) a mark that makes it "act"
// like infinity in the underlying ordering of its type T
template<typename T>
struct MarkedValue{
    T val;
	bool marked;

	MarkedValue(T val, bool marked = false): val(val), marked(marked){}

	bool operator< (const MarkedValue<T>& other) const {
		if (marked and !other.marked){
			return false;
		} else if (!marked and other.marked){
		    return true;
		} else {
            return val < other.val;
        }
    }

	bool operator== (const MarkedValue<T>& other) const {
        return val == other.val and marked == other.marked;
    }

	bool operator> (const MarkedValue<T>& other) const {
        return other < *this;
    }

	// type cast to base type T
	operator T() const{
        return val;
    }

	void mark(){
        marked = true;
    }
};

// O(N log N) unmarking of the entire heap
template<typename T>
void unmark_all(min_priority_queue<MarkedValue<T>> &min_heap) {
	vector<T> values;
	while (!min_heap.empty()) {
		MarkedValue<T> min_key = min_heap.top();
		min_heap.pop();
        values.emplace_back(T(min_key));
	}
	for (const T key: values) {
		min_heap.push(MarkedValue<T>(key));
	}
}
#endif //UTILS_H
