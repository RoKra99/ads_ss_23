#pragma once

#include <iostream>
#include <vector>

namespace ads_robert {
using Number = std::uint64_t;

inline void print_result(const std::string& algo, double time, std::size_t space) {
    std::cout << "RESULT algo=" << algo << " namekrause_robert time=" << time << " space=" << space << std::endl;
}

inline void write_results_to_file(const std::string& output, const std::vector<Number>& results) {
    std::ofstream out(output);
    for (const auto res : results) {
        out << res << std::endl;
    }
}
}