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
    ASSERT_EQ(expected, result);
}

TEST(test_balanced_sort, test_sort){
    constexpr int num_files = 4;
    constexpr int mem_size = 3;
    const vector<int> data = {7, 1, 5, 6, 3, 8, 2, 10, 4, 9, 1, 3, 7, 4, 1, 2, 3};
    const vector<int> balanced_sorted_data = balanced_sort(data, num_files, mem_size);
    vector<int> expected_sorted_data = data;
    sort(expected_sorted_data.begin(), expected_sorted_data.end());
    ASSERT_EQ(balanced_sorted_data, expected_sorted_data);
    cout << "Passed 'test_balanced_sort'" << endl;
}

TEST(test_balanced_sort, parametrized_small_random_test_sort) {
    for (int i = 0; i < 10; i++) {
        const int num_files = 2 * RandomDataFixture::randint(2, 10);
        const int mem_size = RandomDataFixture::randint(num_files + 1, 2 * num_files + 1);
        const int size = RandomDataFixture::randint(1, 10);
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

TEST(test_balanced_sort, parametrized_large_random_test_sort) {
    for (int i = 0; i < 100; i++) {
        const int num_files = 2 * RandomDataFixture::randint(2, 30);
        const int mem_size = RandomDataFixture::randint( num_files + 1, std::min(num_files * 2 + 1, 70));
        const int size = RandomDataFixture::randint(1e4, 5e4);
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

TEST(test_balanced_sort, parametrized_huge_random_test_sort) {
    for (int i = 0; i < 10; i++) {
        const int num_files = 2 * RandomDataFixture::randint(2, 30);
        const int mem_size = RandomDataFixture::randint( num_files + 1, std::min(num_files * 2 + 1, 70));
        const int size = RandomDataFixture::randint(1e5, 3e5);
        const vector<int> data = RandomDataFixture::random_vector(size, -1e9, +1e9);
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

TEST(test_balanced_sort, parametrized_mostly_equal_huge_random_test_sort) {
    for (int i = 0; i < 10; i++) {
        const int num_files = 2 * RandomDataFixture::randint(2, 30);
        const int mem_size = RandomDataFixture::randint( num_files + 1, std::min(num_files * 2 + 1, 70));
        const int size = RandomDataFixture::randint(1e5, 3e5);
        const vector<int> data = RandomDataFixture::random_vector(size, -100, +100);
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

int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}