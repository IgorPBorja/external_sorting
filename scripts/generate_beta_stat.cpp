//
// Created by igor-borja on 8/15/24.
//
#include <iostream>
#include <vector>

#include "initial_distribution.hpp"
#include "utils.hpp"

const int N = 1e6;
const int MAX_M = 100;
const int DELTA = 5;

std::vector<int> random_vector(size_t size, int min_element, int max_element) {
    std::vector<int> gen(size);
    for (int i = 0; i < size; i++) {
        const int val = rand() % (max_element - min_element + 1);
        gen[i] = min_element + val;
    }
    return gen;
}

int main(int argc, char* argv[]){
    // small output optimization
    std::ios_base::sync_with_stdio(false);

    freopen(argv[1], "w", stdout);

    std::vector<int> data[10];
    for (int i = 0; i < 10; i++){
        data[i] = random_vector(N, -1e9, 1e9);
    }

    for (int m = DELTA; m <= MAX_M; m += DELTA){
        double beta[10];
        std::cout << m << ":" << std::endl;
        for (int i = 0; i < 10; i++){
            // only 1 file is fine
            std::vector<std::vector<std::vector<int>>> files(1);
            perform_initial_distribution(data[i], files, m);
            // calculate beta
            double beta = Observer::avg_run_size(files, m);
            std::cout << std::fixed << std::setprecision(4) << beta << " ";
        }
        std::cout << std::endl;
    }
}