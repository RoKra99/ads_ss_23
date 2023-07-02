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
        //std::cout << logn << std::endl;

        for (Number l = 1; l < logn; ++l) {
            auto& inner = query_results[l];
            inner.reserve(n);
            for (Number s = 0; s < n; ++s) {
                inner.push_back(accessQuery(l - 1, s));
            }
        }
    }

    inline Number rmq(const Number s, const Number e) const {
        //std::cout << "rmq: " << s << ", " << e << std::endl;
        //std::cout << _input.size() << std::endl;
        if (s - e < 2) {
            return argmin(_input, s, e);
        }
        const Number l = std::max(1.0, std::log2(e - s - 1));
        //std::cout << (e - s - 1) << " --> " <<  l << std::endl;
        const Number m1 = accessQuery(l - 1, s);
        //std::cout << m1 << "####################################################"<< std::endl;
        const Number m2 = accessQuery(l - 1, e - (1 << (l - 1))); 
        return argmin(_input, m1, m2);
    }

    inline std::size_t getSizeInBits() const {
        std::size_t result = 0;
        for (const auto& inner : query_results) {
            result += inner.size() * sizeof(Number);
        }
        return result * 8;
    }
private:

    inline Number accessQuery(Number l, Number idx) const {
        const Number idx2 = idx + (1 << l);
        //std::cout << l << ", " << idx << ", " << idx2 << std::endl;
        if (idx2 >= _input.size()) {
            //std::cout << query_results[l][idx] << std::endl;
            return query_results[l][idx];
        }
        //std::cout << query_results[l].size() << std::endl;
        const Number m1 = query_results[l][idx];
        const Number m2 = query_results[l][idx2];
        //std::cout << m1 << ", " << m2 << std::endl;
        //std::cout << _input.size() << std::endl;
        return argmin(_input, m1, m2);
    }

private:
    const Number n;
    // index one is length index two is start
    std::vector<std::vector<Number>> query_results;
    const std::vector<Number> _input;
};
}