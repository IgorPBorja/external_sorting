//
// Created by igor-borja on 7/28/24.
//

#ifndef RANDOMDATAFIXTURE_H
#define RANDOMDATAFIXTURE_H

#include <vector>
#include <string>


// namespace for random-data fixtures
namespace RandomDataFixture {
    std::vector<int> random_vector(size_t size, uint min_element, uint max_element);
    int randint(uint min_element, uint max_element);
}

namespace Debugging {
    std::string to_string(const std::vector<int>& v);
}


#endif //RANDOMDATAFIXTURE_H
