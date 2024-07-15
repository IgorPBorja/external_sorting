//
// Created by igor-borja on 7/15/24.
//
#include <vector>
#include <algorithm>
#include <queue>
#include <utility>
#include <memory>
#include <exception>

using std::vector, std::sort, std::min, std::pair;

template<typename T>
using min_priority_queue = std::priority_queue<T, vector<T>, std::greater<T>>;

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
	// register last run
	right[file_idx].emplace_back(current_run);
}

template<typename T>
vector<T> balanced_sort(vector<T> data, const int num_files, const int mem_size){
    const uint left_files = (num_files + 1) / 2, right_files = num_files / 2;
	vector<vector<vector<T>>> left(left_files), right(right_files);
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
		swap(left, right);
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
