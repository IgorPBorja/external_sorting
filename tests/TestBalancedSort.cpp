//
// Created by igor-borja on 7/15/24.
//
#include <cassert>
#include <vector>
#include <algorithm>

#include "balanced_sort.hpp"

void test_1(){
    std::vector<int> data = {7, 1, 5, 6, 3, 8, 2, 10, 4, 9, 1, 3, 7, 4, 1, 2, 3};
    std::vector<int> balanced_sorted_data = balanced_sort(data);
    std::vector<int> expected_sorted_data = data;
    std::sort(expected_sorted_data.begin(), expected_sorted_data.end());
    assert(balanced_sorted_data == expected_sorted_data);
}

int main(){
    test_1();
}