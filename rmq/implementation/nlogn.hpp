#pragma once 

#include "../util/io.h"
#include "../util/argmin.h"

#include <math.h>
#include <algorithm>


namespace ads_robert {

class NlogNRMQ {


public:
    NlogNRMQ(std::vector<Number>&& input) : n(input.size()), _input(input) {
        const Number n = input.size();
        const Number logn = std::log2(n);
        query_results.resize(logn);
        auto& self_row = query_results[0];
        self_row.reserve(n);

        for (Number s = 0; s < n; ++s) {
            self_row.push_back(s);
        }

        for (Number l = 1; l < logn; ++l) {
            auto& inner = query_results[l];
            inner.reserve(n);
            for (Number s = 0; s < n; ++s) {
                inner.push_back(accessQuery(l - 1, s));
            }
        }
    }

    inline Number rmq(const Number s, const Number e) const {
        if (s - e < 2) {
            return argmin(_input, s, e);
        }
        const Number l = std::max(1.0, std::log2(e - s + 1));
        const Number m1 = accessQuery(l - 1, s);
        const Number m2 = accessQuery(l - 1, e - (1 << (l)) + 1);
        return argmin(_input, m1, m2);
    }

    inline std::size_t getSizeInBits(bool addInput = false) const {
        std::size_t result = 0;
        for (const auto& inner : query_results) {
            result += inner.size() * sizeof(Number);
        }
        if (addInput) {
            result += _input.size() * sizeof(Number);
        }
        return result * 8;
    }
private:

    inline Number accessQuery(Number l, Number idx) const {
        const Number idx2 = idx + (1 << l);
        if (idx2 >= _input.size()) {
            return query_results[l][idx];
        }
        const Number m1 = query_results[l][idx];
        const Number m2 = query_results[l][idx2];
        return argmin(_input, m1, m2);
    }

private:
    const Number n;
    // index one is length index two is start
    std::vector<std::vector<Number>> query_results;
    const std::vector<Number> _input;
};
}