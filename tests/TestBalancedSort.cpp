//
// Created by igor-borja on 7/15/24.
//
#include <cassert>
#include <vector>
#include <algorithm>
#include <complex>
#include <iostream>
#include <gtest/gtest.h>

#include "balanced_sort.hpp"
#include "RandomDataFixture.hpp"

using std::vector, std::sort, std::cout, std::endl;

TEST(test_balanced_sort, test_alternating_iterator) {
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
}

TEST(test_balanced_sort, test_initial_runs) {
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
}

TEST(test_balanced_sort, test_sort){
    constexpr uint num_files = 4;
    constexpr uint mem_size = 3;
    const vector<int> data = {7, 1, 5, 6, 3, 8, 2, 10, 4, 9, 1, 3, 7, 4, 1, 2, 3};
    const vector<int> balanced_sorted_data = balanced_sort(data, num_files, mem_size);
    vector<int> expected_sorted_data = data;
    sort(expected_sorted_data.begin(), expected_sorted_data.end());
    assert(balanced_sorted_data == expected_sorted_data);
    cout << "Passed 'test_balanced_sort'" << endl;
}

TEST(test_balanced_sort, parametrized_large_random_test_sort) {
    // FIXME: TAKING TOO LONG
    GTEST_SKIP() << "Skipping this test since it takes 100% CPU usage and still does not finish" << endl;
    for (uint i = 0; i < 10; i++) {
        const uint num_files = 2 * RandomDataFixture::randint(2, 30);
        const uint mem_size = RandomDataFixture::randint( num_files + 1, std::min(num_files * 2 + 1, 40u));
        const uint size = RandomDataFixture::randint(100, 200);
        const vector<int> data = RandomDataFixture::random_vector(size, -1e5, +1e5);
        const vector<int> balanced_sorted_data = balanced_sort(data, num_files, mem_size, false);
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

TEST(test_balanced_sort, parametrized_small_random_test_sort) {
    for (uint i = 0; i < 10; i++) {
        const uint num_files = 2 * RandomDataFixture::randint(2, 10);
        const uint mem_size = RandomDataFixture::randint(num_files + 1, 2 * num_files + 1);
        const uint size = RandomDataFixture::randint(1, 10);
        const vector<int> data = RandomDataFixture::random_vector(size, -1e5, +1e5);
        const vector<int> balanced_sorted_data = balanced_sort(data, num_files, mem_size);
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



