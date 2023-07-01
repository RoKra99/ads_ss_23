#pragma once 

#include <cstdint>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

namespace ads_robert {
using Number = std::uint64_t;

struct PDInput {
    Number n;
    std::vector<Number> numbers;
    std::vector<Number> queries;
};
PDInput readPD(const std::string& file) {
    PDInput input;
    std::ifstream in(file);
    in >> input.n;
    input.numbers.reserve(input.n);
    Number x;
    for (Number i = 0; i < input.n; ++i) {
        in >> x;
        input.numbers.push_back(x);
    }
    while (in >> x) {
        input.queries.push_back(x);
    }
    return input;
}
}