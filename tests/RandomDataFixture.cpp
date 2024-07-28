//
// Created by igor-borja on 7/28/24.
//

#include <vector>
#include <algorithm>
#include "RandomDataFixture.hpp"

std::vector<int> RandomDataFixture::random_vector(size_t size, uint min_element, uint max_element) {
    std::vector<int> gen(size);
    for (uint i = 0; i < size; i++) {
        const int val = rand() % (max_element - min_element + 1);
        gen[i] = min_element + val;
    }
    return gen;
}

int RandomDataFixture::randint(uint min_element, uint max_element) {
    return rand() % (max_element - min_element + 1) + min_element;
}


std::string Debugging::to_string(const std::vector<int>& v){
    std::string s = "[";
    for (const auto& x: v){
        s += std::to_string(x) + " ";
    }
    s += "]";
    return s;
}


