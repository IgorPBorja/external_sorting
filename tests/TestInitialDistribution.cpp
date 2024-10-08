//
// Created by igor-borja on 7/15/24.
//
#include <cassert>
#include <vector>
#include <algorithm>
#include <complex>
#include <iostream>
#include <gtest/gtest.h>

#include "initial_distribution.hpp"
#include "utils.hpp"

using std::vector, std::sort, std::cout, std::endl;

TEST(test_polyphasic_sort, test_initial_runs) {
    const int num_files = 3;
    const int mem_size = 3;
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
    ASSERT_EQ(files, expected);
}

int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}

