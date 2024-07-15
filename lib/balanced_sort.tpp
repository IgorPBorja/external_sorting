//
// Created by igor-borja on 7/15/24.
//
#include <vector>
#include <algorithm>
#include <queue>

using std::vector, std::sort, std::min;

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
void unmark_all(min_priority_queue<MarkedValue<T>> min_heap) {
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

// creates the initial runs and distribute them into the `num_files` files
// in a alternating manner
// returns a vector of `num_files` elements of type vector<vector<T>> (vectors of runs)
template<typename T>
vector<vector<vector<T>>> initial_runs(vector<T> data, uint num_files, uint mem_size){
	assert(mem_size > 1);

	// special heap of marked values
	min_priority_queue<MarkedValue<T>> min_heap;
	for (uint i = 0; i < min(mem_size, (uint)data.size()); i++) {
		min_heap.emplace(data[i]);
	}

	vector<vector<vector<T>>> files(num_files);
	vector<T> current_run;
	uint file_idx = 0, marked_cnt = 0;
	for (uint i = mem_size; i < data.size(); i++){
        if (marked_cnt == mem_size) {
            // that means all values are marked, so unmark and start a new run
        	files[file_idx].emplace_back(current_run);
        	current_run = vector<T>();
            file_idx = (file_idx + 1) % num_files;
            unmark_all(min_heap);
        	marked_cnt = 0;
        }
		// make room for new data (removing 1 from heap)
        MarkedValue<T> min_key = min_heap.top();
        T curr_value = min_key.val;
        current_run.push_back(T(min_key));
        min_heap.pop();
		// push new data (1 entry)
		if (data[i] < curr_value) {  // breaks order (smaller and cames after) -> push marked
			min_heap.emplace(data[i], true);
			++marked_cnt;
		} else {
			min_heap.emplace(data[i], false);
		}
    }

	// unmark leftover data
	if (marked_cnt > 0) {
		unmark_all(min_heap);
	}
	// distribute leftover data
	while (!min_heap.empty()) {
		MarkedValue<T> min_key = min_heap.top();
		min_heap.pop();
		current_run.push_back(T(min_key));
	}
	// register last run
	files[file_idx].emplace_back(current_run);
	return files;
}

template<typename T>
vector<T> balanced_sort(vector<T> data, int num_files, int mem_size){
	// TODO: implement
	sort(data.begin(), data.end());
    return data;
}
