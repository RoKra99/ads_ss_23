#pragma once
#include <vector>
#include <algorithm>
#include "../util/io.h"

namespace ads_robert {
class ManualPD {
public:
    ManualPD(const std::vector<Number>& input) : _input(input) {

    }

    inline Number predecessor(const Number x) const {
        if (x < _input[0]){
            return std::numeric_limits<Number>::max();
        }
        return *(std::lower_bound(_input.begin(), _input.end(), x, [](Number a, Number b) {
            return a <= b;
        })-1);
    }

    inline std::size_t getSizeInBits() const {
        return 0;
    }
private:
    const std::vector<Number>& _input;
};
}