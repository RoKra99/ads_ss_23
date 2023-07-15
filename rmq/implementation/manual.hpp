#pragma once
#include <vector>
#include <algorithm>
#include "../util/io.h"

namespace ads_robert {
class ManualRMQ {
public:
    ManualRMQ(const std::vector<Number>& input) : _input(input) {

    }

    inline Number rmq(const Number s, const Number e) const {
        return std::min_element(_input.begin() + s, _input.begin() + e + 1) - _input.begin();
    }

    inline std::size_t getSizeInBits(bool includeInput = false) const {
        return 0;
    }
private:
    const std::vector<Number>& _input;
};
}