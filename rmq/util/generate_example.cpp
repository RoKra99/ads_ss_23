#include <iostream>
#include <vector>
#include <random>
#include <numeric>
#include <algorithm>

#include "../util/io.h"
#include "../../util/commandline.hpp"

inline void write_results_to_file(const std::string& output, const  ads_robert::Number n, const std::vector< ads_robert::Number>& numbers, const std::vector<std::pair<ads_robert::Number, ads_robert::Number>>& queries) {
    std::ofstream out(output);
    out << n << std::endl;
    for (const auto num : numbers) {
        out << num << std::endl;
    }
    for (const auto& q : queries) {
        out << q.first << "," << q.second << std::endl;
    }
}

int main(int argn, char** argc) {
    CommandLine c(argn, argc);
    ads_robert::Number n = c.longArg("-n", 10000);
    ads_robert::Number power = c.longArg("-max", 64);
    ads_robert::Number queryCount = c.longArg("-q", n);
    std::string output = c.strArg("-out", "../rmq/data/rmq_n_" + std::to_string(n) + "_q_" + std::to_string(queryCount) + "_p_" + std::to_string(power) + ".txt");

    const ads_robert::Number max = 1 << (power - 1);
    std::uniform_int_distribution<ads_robert::Number> dist(0, max);
    auto gen = std::mt19937_64{ 0 };

    std::vector< ads_robert::Number> numbers;
    std::vector<std::pair<ads_robert::Number, ads_robert::Number>> queries;
    numbers.reserve(n);
    queries.reserve(n);
    for (ads_robert::Number i = 0; i < n; ++i) {
        numbers.push_back(dist(gen));
    }
    std::uniform_int_distribution<ads_robert::Number> dist_q(0, n - 1);
    for (ads_robert::Number i = 0; i < queryCount; ++i) {
        const ads_robert::Number n1 = dist_q(gen);
        const ads_robert::Number n2 = dist_q(gen);
        queries.emplace_back(std::min(n1, n2), std::max(n1, n2));
    }
    write_results_to_file(output, n, numbers, queries);
}