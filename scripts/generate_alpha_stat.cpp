// Script for generating random runs (of unit size of random size)
// and distributing them evenly accross files
#include <vector>
#include <algorithm>
#include <filesystem>
#include <string>

#include "cascade_sort.hpp"
#include "balanced_sort.hpp"
#include "polyphasic_sort.hpp"

using std::vector, std::string;

namespace RandomRuns {
    vector<int> random_vector(size_t size, int min_element, int max_element) {
        vector<int> gen(size);
        for (int i = 0; i < size; i++) {
            const int val = rand() % (max_element - min_element + 1);
            gen[i] = min_element + val;
        }
        return gen;
    }

    int randint(int min_element, int max_element) {
        return rand() % (max_element - min_element + 1) + min_element;
    }


    vector<vector<vector<int>>> random_runs(
        const int num_runs,
        const int num_files,
        const int max_run_size
    ){
        vector<vector<vector<int>>> files(num_files);
        for (int i = 0; i < num_runs; i++){
            vector<int> r = random_vector(randint(1, max_run_size), -(int)1e4, +(int)1e4);
            files[i % num_files].emplace_back(r);
        }
        return files;
    }

    vector<vector<vector<int>>> unit_random_runs(
        const int num_runs,
        const int num_files
    ){
        return random_runs(num_runs, num_files, 1);
    }
}

double exec_sorting_method(const string name, const int num_runs){
    const int num_files = 10, mem_size = 10;
    if (name == "cascade"){
        vector<vector<vector<int>>> files = RandomRuns::unit_random_runs(num_runs, num_files - 1);
        files.emplace_back(); // empty file for merge
        auto[sorted_data, avg_writes] = _cascade_sort_from_initial(
            files, mem_size, false
        );
        return avg_writes;
    } else if (name == "polyphasic"){
        vector<vector<vector<int>>> files = RandomRuns::unit_random_runs(num_runs, num_files - 1);
        vector<vector<int>> anchor_file;
        auto[sorted_data, avg_writes] = _polyphasic_sort_from_initial(
            files, anchor_file, mem_size, false
        );
        return avg_writes;
    } else if (name == "balanced"){
        vector<vector<vector<int>>> left = RandomRuns::unit_random_runs(num_runs, num_files / 2);
        vector<vector<vector<int>>> right(num_files / 2);
        auto[sorted_data, avg_writes] = _balanced_sort_from_initial(
            left, right, mem_size, false
        );
        return avg_writes;
    } else {
        throw std::invalid_argument("invalid sorting method");
    }
}

int main(){
    std::filesystem::create_directories("data");
    std::filesystem::create_directories("data/alpha");
    const string BASE_DIR = "data/alpha";
    auto methods = { "balanced" }; //, "polyphasic", "cascade" };

    // see docs for homework
    const int REPS = 10;
    auto k_values = {4, 6, 8, 10, 12};
    auto m_values = {3, 15, 30, 45, 60};
    vector<int> r_values;
    for (int i = 1; i <= 10; i++) {
        for (int j = 10; j <= 1000; j += 10) {
            if (i * j <= 5000) r_values.emplace_back(i * j);
        }
    }
    std::sort( r_values.begin(), r_values.end() );
    r_values.erase( std::unique( r_values.begin(), r_values.end() ), r_values.end() );

    for (const string& method: methods){
        for (const int k: k_values) {
            string filepath = BASE_DIR + "/" + method + "-" + std::to_string(k) + "-files.txt";
            freopen(filepath.c_str(), "w", stdout);
            for (const auto r: r_values){
                double avg_alpha = 0.0;
                for (auto m: m_values) {
                    for (int i = 0; i < REPS; i++){
                        avg_alpha += exec_sorting_method(method, r) / double(REPS * m_values.size());
                    }
                }
                std::cout << r << " : " << std::fixed << std::setprecision(4) << avg_alpha << std::endl;
            }
        }
    }
}