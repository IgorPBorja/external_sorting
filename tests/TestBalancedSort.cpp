//
// Created by igor-borja on 7/15/24.
//
#include <cassert>
#include <vector>
#include <algorithm>
#include <iostream>

#include "balanced_sort.hpp"

using std::vector, std::sort, std::cout, std::endl;

void test_alternating_iterator() {
    const vector<vector<vector<int>>> data = {
        {
            {1, 5, 6, 7, 8}, {1, 3, 4, 7}
        },
        {
            {2, 3, 4, 9, 10}
        }
    };
    vector<int> expected = {
        1, 2, 5, 3, 6, 4, 7, 9, 8, 10, 1, 3, 4, 7
    }, result;
    for (AlternatingIterator<int> it(data); !it.ended(); ++it) {
        result.push_back(it.value());
    }
    assert(expected == result);
    cout << "Passed 'test_alternating_iterator'" << endl;
}

void test_initial_runs() {
    constexpr uint num_files = 2;
    constexpr uint mem_size = 3;
    const vector<int> data = {7, 1, 5, 6, 3, 8, 2, 10, 4, 9, 1, 3, 7, 4, 1, 2, 3};
    vector<vector<vector<int>>> left(num_files), right(num_files);
    left[0].push_back(data);
    const vector<vector<vector<int>>> expected = {
        {
            {1, 5, 6, 7, 8, 10}, {1, 3, 4, 7}
        },
        {
            {2, 3, 4, 9}, {1, 2, 3}
        }
    };
    p_way_merge(left, right, mem_size);
    assert(right == expected);

    cout << "Passed 'test_initial_runs'" << endl;
}

void test_balanced_sort(){
    constexpr uint num_files = 4;
    constexpr uint mem_size = 3;
    const vector<int> data = {7, 1, 5, 6, 3, 8, 2, 10, 4, 9, 1, 3, 7, 4, 1, 2, 3};
    const vector<int> balanced_sorted_data = balanced_sort(data, num_files, mem_size);
    vector<int> expected_sorted_data = data;
    sort(expected_sorted_data.begin(), expected_sorted_data.end());
    assert(balanced_sorted_data == expected_sorted_data);
    cout << "Passed 'test_balanced_sort'" << endl;
}

int main(){
    test_alternating_iterator();
    test_initial_runs();
    test_balanced_sort();
}