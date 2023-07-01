#pragma once 

#include <cstdint>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

namespace ads_robert {
using Number = std::uint64_t;

struct RMQ {
    Number s;
    Number e;
};

struct RMQInput {
    Number n;
    std::vector<Number> numbers;
    std::vector<RMQ> queries;
};

RMQInput readRMQ(const std::string& file) {
    RMQInput input;
    std::ifstream in(file);
    in >> input.n;
    input.numbers.reserve(input.n);
    Number x;
    for (Number i = 0; i < input.n; ++i) {
        in >> x;
        input.numbers.push_back(x);
    }
    Number s, e;
    char comma;
    while (in >> s >> comma >> e) {
        input.queries.push_back({ s, e });
    }
    return input;
}
}