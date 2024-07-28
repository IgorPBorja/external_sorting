//
// Created by igor-borja on 7/15/24.
//
#include <cassert>
#include <vector>
#include <algorithm>
#include <iostream>

#include "polyphasic_sort.hpp"

using std::vector, std::sort, std::cout, std::endl;

void test_initial_runs() {
    const uint num_files = 3;
    const uint mem_size = 3;
    const vector<int> data = {7, 1, 5, 6, 3, 8, 2, 10, 4, 9, 1, 3, 7, 4, 1, 2, 3};
    vector<vector<vector<int>>> files(num_files- 1);
    const vector<vector<vector<int>>> expected = {
        {
            {1, 5, 6, 7, 8, 10}, {1, 3, 4, 7}
        },
        {
            {2, 3, 4, 9}, {1, 2, 3}
        }
    };
    perform_initial_distribution(data, files, mem_size);
    assert(files == expected);
    cout << "Passed 'test_initial_runs'" << endl;
}

void test_polyphasic_sort() {
    const uint num_files = 3;
    const uint mem_size = 3;
    const vector<int> data = {7, 1, 5, 6, 3, 8, 2, 10, 4, 9, 1, 3, 7, 4, 1, 2, 3};
    const vector<int> polyphasic_sorted_data = polyphasic_sort(data, num_files, mem_size);
    vector<int> sorted_data = data;
    sort(sorted_data.begin(), sorted_data.end());

    assert(polyphasic_sorted_data == sorted_data);
    cout << "Passed 'test_polyphasic_sort'" << endl;
}

int main(){
    test_initial_runs();
    test_polyphasic_sort();
}
