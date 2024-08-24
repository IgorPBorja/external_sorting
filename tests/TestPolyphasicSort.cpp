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

TEST(test_polyphasic_sort, test_sort) {
    const int num_files = 3;
    const int mem_size = 3;
    const vector<int> data = {7, 1, 5, 6, 3, 8, 2, 10, 4, 9, 1, 3, 7, 4, 1, 2, 3};
    const vector<int> polyphasic_sorted_data = polyphasic_sort(data, num_files, mem_size);
    vector<int> sorted_data = data;
    sort(sorted_data.begin(), sorted_data.end());

    ASSERT_EQ(polyphasic_sorted_data, sorted_data);
}

TEST(test_polyphasic_sort, parametrized_large_random_test_sort) {
    for (int i = 0; i < 100; i++) {
        const int num_files = 2 * RandomDataFixture::randint(2, 30);
        const int mem_size = RandomDataFixture::randint( num_files + 1, std::min(num_files * 2 + 1, 70));
        const int size = RandomDataFixture::randint(5e3, 1e4);
        const vector<int> data = RandomDataFixture::random_vector(size, -1e5, +1e5);
        const vector<int> balanced_sorted_data = polyphasic_sort(data, num_files, mem_size, false);
        vector<int> expected_sorted_data = data;
        sort(expected_sorted_data.begin(), expected_sorted_data.end());

        if (balanced_sorted_data != expected_sorted_data) {
            std::cout << "Failed on testcase " << i << std::endl;
            if (expected_sorted_data.size() != balanced_sorted_data.size()) {
                std::cout << "Sizes differ: expected " << expected_sorted_data.size() << ", got " << balanced_sorted_data.size() << std::endl;
            } else {
                for (int j = 0; j < expected_sorted_data.size(); j++) {
                    if (expected_sorted_data[j] != balanced_sorted_data[j]) {
                        std::cout << "First difference in position " << j << ", expected " << expected_sorted_data[j] << ", got" << balanced_sorted_data[j] << std::endl;
                        break;
                    }
                }
            }
        }
        SCOPED_TRACE("FAILED TESTCASE " + std::to_string(i) + ":\nInput: " + Debugging::to_string(data) + "\nOutput: "
            + Debugging::to_string(balanced_sorted_data)
            + "\nExpected: " + Debugging::to_string(expected_sorted_data));
        ASSERT_EQ(
            balanced_sorted_data, expected_sorted_data
        );
    }
}

TEST(test_polyphasic_sort, parametrized_huge_random_test_sort) {
    for (int i = 0; i < 10; i++) {
        const int num_files = 2 * RandomDataFixture::randint(2, 10);
        const int mem_size = RandomDataFixture::randint(num_files + 1, 2 * num_files + 1);
        const int size = RandomDataFixture::randint(5e4, 1e5);
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
    for (int i = 0; i < 1000; i++) {
        const int num_files = 2 * RandomDataFixture::randint(2, 10);
        const int mem_size = RandomDataFixture::randint(num_files + 1, 2 * num_files + 1);
        const int size = RandomDataFixture::randint(1, 10);
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

int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
