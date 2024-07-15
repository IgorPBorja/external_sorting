//
// Created by igor-borja on 7/15/24.
//
#include <cassert>
#include <vector>
#include <algorithm>
#include <iostream>

#include "balanced_sort.hpp"

using std::vector, std::sort, std::cout, std::endl;

void test_initial_runs() {
    const uint num_files = 2;
    const uint mem_size = 3;
    const vector<int> data = {7, 1, 5, 6, 3, 8, 2, 10, 4, 9, 1, 3, 7, 4, 1, 2, 3};
    const vector<vector<vector<int>>> expected = {
        {
            {1, 5, 6, 7, 8}, {1, 3, 4, 7}
        },
        {
            {2, 3, 4, 9, 10}
        }
    };
    const vector<vector<vector<int>>> result = initial_runs(data, num_files, mem_size);
    assert(expected == result);

    cout << "Passed 'test_initial_runs'" << endl;
}

void test_balanced_sort(){
    const uint num_files = 4;
    const uint mem_size = 3;
    const vector<int> data = {7, 1, 5, 6, 3, 8, 2, 10, 4, 9, 1, 3, 7, 4, 1, 2, 3};
    const vector<int> balanced_sorted_data = balanced_sort(data, num_files, mem_size);
    vector<int> expected_sorted_data = data;
    sort(expected_sorted_data.begin(), expected_sorted_data.end());
    assert(balanced_sorted_data == expected_sorted_data);
    cout << "Passed 'test_balanced_sort'" << endl;
}

int main(){
    test_initial_runs();
    test_balanced_sort();
}