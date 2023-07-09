#include <iostream>
#include <vector>
#include <random>
#include <numeric>
#include <algorithm>

#include "../util/io.h"
#include "commandline.hpp"

inline void write_results_to_file(const std::string& output, const  ads_robert::Number n, const std::vector< ads_robert::Number>& numbers, const std::vector<ads_robert::Number>& queries) {
    std::ofstream out(output);
    out << n << std::endl;
    for (const auto num : numbers) {
        out << num << std::endl;
    }
    for (const auto& q : queries) {
        out << q << std::endl;
    }
}

int main(int argn, char** argc) {
    CommandLine c(argn, argc);
    ads_robert::Number n = c.intArg("-n", 10000);
    ads_robert::Number max = c.intArg("-max", 0);
    ads_robert::Number queryCount = c.intArg("-q", n);
    std::string output = c.strArg("-out", "../pd/data/pd_n_" + std::to_string(n) + "_q_" + std::to_string(queryCount) + ".txt");

    max = max == 0 ? std::numeric_limits<ads_robert::Number>::max() : max;
    std::uniform_int_distribution<ads_robert::Number> dist(0, max);
    auto gen = std::mt19937_64{ 0 };

    std::vector< ads_robert::Number> numbers;
    std::vector<ads_robert::Number> queries;
    numbers.reserve(n);
    queries.reserve(n);
    for (ads_robert::Number i = 0; i < n; ++i) {
        numbers.push_back(dist(gen));
    }
    std::sort(numbers.begin(), numbers.end());
    std::uniform_int_distribution<ads_robert::Number> dist2(numbers[0], max);
    for (ads_robert::Number i = 0; i < queryCount; ++i) {
        queries.push_back(dist2(gen));
    }
    write_results_to_file(output, n, numbers, queries);
}