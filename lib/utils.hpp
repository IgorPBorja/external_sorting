//
// Created by igor-borja on 7/28/24.
//

#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <queue>
#include <string>
#include <iomanip>
#include <cmath>

using std::vector;
template<typename T>
using min_priority_queue = std::priority_queue<T, vector<T>, std::greater<T>>;

template<typename T>
struct AlternatingIterator {
    const vector<vector<vector<T>>>& files_ref;
    int n;
    int curr_file = 0;
	// respectively: index of run, index inside run
    vector<int> run_index;
    vector<int> inner_index;
    int completed_files = 0;

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
			int skipped = 0;
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

struct Observer {
	explicit Observer(std::ostream& os) : step(0), os(os) {}

	template<typename T>
	static double avg_run_size(const vector<vector<vector<T>>>& active_files, const int mem_size) {
		int total_elements = 0, total_runs = 0;
		for (const vector<vector<T>>& file: active_files) {
			total_runs += file.size();
			for (const vector<T>& run: file) {
				total_elements += run.size();
			}
		}
		return static_cast<double>(total_elements) / static_cast<double>(total_runs * mem_size);
	}

	template<typename T>
	void register_step(
		const vector<vector<vector<T>>>& active_files,
		const vector<int>& file_idxs, const int mem_size
	) {
		// precision of two decimal digits (rounding, not truncating)
		os << std::fixed << std::setprecision(2);
		os << "fase " << step << " " << round(100.0 * avg_run_size(active_files, mem_size)) / 100.0 << std::endl;
		print_distribution(active_files, file_idxs);

		// increment step
		++step;
	}

	// only for cascade sort
	template<typename T>
	void register_step(
		const vector<vector<vector<T>>>& active_files,
		const int mem_size
	) {
		// precision of two decimal digits (rounding, not truncating)
		os << std::fixed << std::setprecision(2);
		os << "fase " << step << " " << round(100.0 * avg_run_size(active_files, mem_size)) / 100.0 << std::endl;
		print_distribution(active_files);

		// increment step
		++step;
	}

	void reset() {
		step = 0;
	}
private:
	int step;
	std::ostream& os;

	// Prints the runs distributed in the active files
	// file_idxs: vector of indices of the active files
	// active_files: vector of references to the active files (which are a vector<vector<T>> each)
	template<typename T>
	void print_distribution(const vector<vector<vector<T>>>& active_files, const vector<int>& file_idxs) {
		if (active_files.size() != file_idxs.size()) {
			const std::string error = (
				std::string("There should be the same number of indices as files, got ")
				+ std::to_string(active_files.size())
				+ std::string(" indices and ")
				+ std::to_string(file_idxs.size())
				+ std::string(" files")
			);
			throw std::invalid_argument(error);
		}
		for (int i = 0; i < active_files.size(); i++) {
			if (active_files[i].empty()) {
				continue;
			}
			os << file_idxs[i] << ": ";
			for (const vector<T>& run: active_files[i]) {
				os << "{";
				for (int j = 0; j < run.size(); j++) {
					os << run[j];
					if (j + 1 < run.size()) {
						os << " ";
					}
				}
				os << "}";
			}
			os << std::endl;
		}
	}

	// NOTE: for cascade sort only
	template<typename T>
	void print_distribution(const vector<vector<vector<T>>>& active_files) {
		for (int i = 0; i < active_files.size(); i++) {
			if (active_files[i].empty()) {
				continue;
			}
			os << i + 1 << ": ";
			for (const vector<T>& run: active_files[i]) {
				os << "{";
				for (int j = 0; j < run.size(); j++) {
					os << run[j];
					if (j + 1 < run.size()) {
						os << " ";
					}
				}
				os << "}";
			}
			os << std::endl;
		}
	}
};
#endif //UTILS_H
