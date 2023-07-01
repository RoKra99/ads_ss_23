#pragma once 

#include "../util/io.h"
#include "../util/argmin.h"

#include <algorithm>

namespace ads_robert {

class NaiveRMQ {

public:
    NaiveRMQ(const std::vector<Number>& input) : n(input.size()) {
        const Number n = input.size();
        query_results.resize(n);
        //std::size_t i = 0;
        for (Number s = 0; s < n; ++s) {
            auto& inner = query_results[s];
            inner.reserve(n-s);
            for (Number e = s; e < n; ++e) {
                if (s == e) {
                    inner.push_back(s);
                } else {
                    const Number prev_min = inner.size() > 0 ? inner[inner.size() -1] : s;
                    inner.push_back(argmin(input, prev_min, e));
                }
                // ++i;
                // if (i % 1000000 == 0){
                //     std::cout << "i: " << i << std::endl;
                // }
            }
        }
        //std::cout << "done" << std::endl;
    }

    inline Number rmq(const Number s, const Number e) const {
        return query_results[s][e - s];
    }

    inline std::size_t getSizeInBits() const {
        std::size_t result = 0;
        for (const auto& inner : query_results) {
            result += inner.size() * sizeof(Number);
        }
        return result * 8;
    }

private:
    const Number n;
    std::vector<std::vector<Number>> query_results;
};
}