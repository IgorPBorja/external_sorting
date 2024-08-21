//
// Created by igor-borja on 8/3/24.
//

#include <vector>
#include <string>
#include <iostream>

#include "balanced_sort.hpp"
#include "cascade_sort.hpp"
#include "polyphasic_sort.hpp"

using std::vector, std::string, std::cin;

int main(){
    string mode;
    int m, k, r, n;
    vector<int> data;

    cin >> mode;
    cin >> m >> k >> r >> n;
    data.resize(n);
    for (int i = 0; i < n; i++) {
        cin >> data[i];
    }

    if (mode == "B") {
        balanced_sort(data, k, m);
    } else if (mode == "P") {
        polyphasic_sort(data, k, m);
    } else if (mode == "C") {
        cascade_sort(data, k, m);
    }
}