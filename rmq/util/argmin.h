#pragma once
#include <vector>
#include "io.h"
namespace ads_robert {

inline Number argmin(const std::vector<Number>& input, Number idx1, Number idx2) {
    if (input[idx1] <= input[idx2]) {
        return idx1;
    } else {
        return idx2;
    }
}

inline Number argmin(const std::vector<Number>::const_iterator& begin, Number idx1, Number idx2) {
    if (*(begin + idx1) <= *(begin + idx2) ) {
        return idx1;
    } else {
        return idx2;
    }
}


}