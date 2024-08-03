//
// Created by igor-borja on 7/15/24.
//
#include <cassert>
#include <vector>
#include <algorithm>
#include <iostream>
#include <gtest/gtest.h>

#include "polyphasic_sort.hpp"
#include "RandomDataFixture.hpp"

using std::vector, std::sort, std::cout, std::endl;

TEST(test_polyphasic_sort, test_initial_runs) {
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
    ASSERT_EQ(files, expected);
}

TEST(test_polyphasic_sort, test_sort) {
    const uint num_files = 3;
    const uint mem_size = 3;
    const vector<int> data = {7, 1, 5, 6, 3, 8, 2, 10, 4, 9, 1, 3, 7, 4, 1, 2, 3};
    const vector<int> polyphasic_sorted_data = polyphasic_sort(data, num_files, mem_size);
    vector<int> sorted_data = data;
    sort(sorted_data.begin(), sorted_data.end());

    ASSERT_EQ(polyphasic_sorted_data, sorted_data);
}

TEST(test_polyphasic_sort, parametrized_large_random_test_sort) {
    for (uint i = 0; i < 100; i++) {
        const uint num_files = 2 * RandomDataFixture::randint(2, 30);
        const uint mem_size = RandomDataFixture::randint( num_files + 1, std::min(num_files * 2 + 1, 70u));
        const uint size = RandomDataFixture::randint(5e3, 1e4);
        const vector<int> data = RandomDataFixture::random_vector(size, -1e5, +1e5);
        const vector<int> balanced_sorted_data = polyphasic_sort(data, num_files, mem_size, false);
        vector<int> expected_sorted_data = data;
        sort(expected_sorted_data.begin(), expected_sorted_data.end());

        SCOPED_TRACE("FAILED TESTCASE " + std::to_string(i) + ":\nInput: " + Debugging::to_string(data) + "\nOutput: "
            + Debugging::to_string(balanced_sorted_data)
            + "\nExpected: " + Debugging::to_string(expected_sorted_data));
        ASSERT_EQ(
            balanced_sorted_data, expected_sorted_data
        );
    }
}

TEST(test_polyphasic_sort, parametrized_huge_random_test_sort) {
    for (uint i = 0; i < 10; i++) {
        const uint num_files = 2 * RandomDataFixture::randint(2, 10);
        const uint mem_size = RandomDataFixture::randint(num_files + 1, 2 * num_files + 1);
        const uint size = RandomDataFixture::randint(1e5, 3e5);
        const vector<int> data = RandomDataFixture::random_vector(size, -1e9, +1e9);
        const vector<int> balanced_sorted_data = polyphasic_sort(data, num_files, mem_size, false);
        vector<int> expected_sorted_data = data;
        sort(expected_sorted_data.begin(), expected_sorted_data.end());

        SCOPED_TRACE("FAILED TESTCASE " + std::to_string(i) + ":\nInput: " + Debugging::to_string(data) + "\nOutput: "
            + Debugging::to_string(balanced_sorted_data)
            + "\nExpected: " + Debugging::to_string(expected_sorted_data));
        ASSERT_EQ(
            balanced_sorted_data, expected_sorted_data
        );
    }

}

TEST(test_polyphasic_sort, parametrized_small_random_test_sort) {
    for (uint i = 0; i < 1000; i++) {
        const uint num_files = 2 * RandomDataFixture::randint(2, 10);
        const uint mem_size = RandomDataFixture::randint(num_files + 1, 2 * num_files + 1);
        const uint size = RandomDataFixture::randint(1, 10);
        const vector<int> data = RandomDataFixture::random_vector(size, -1e5, +1e5);
        const vector<int> balanced_sorted_data = polyphasic_sort(data, num_files, mem_size);
        vector<int> expected_sorted_data = data;
        sort(expected_sorted_data.begin(), expected_sorted_data.end());

        SCOPED_TRACE("FAILED TESTCASE " + std::to_string(i) + ":\nInput: " + Debugging::to_string(data) + "\nOutput: "
            + Debugging::to_string(balanced_sorted_data)
            + "\nExpected: " + Debugging::to_string(expected_sorted_data));
        ASSERT_EQ(
            balanced_sorted_data, expected_sorted_data
        );
    }
}

int main() {
    return RUN_ALL_TESTS();
}
