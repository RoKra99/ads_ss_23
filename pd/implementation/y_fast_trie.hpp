#pragma once
#include "../util/io.h"
#include "x_fast_trie.hpp"

#include <vector>
#include <cmath>
#include <chrono>

namespace ads_robert {

class YFastTrie {

public:
    YFastTrie(const std::vector<Number>& input) : W(std::ceil(std::log2(input[input.size() - 1]))), _input(input), _representatives(calculateRepresentatives(input)), _min(input[0]) {}

    inline Number predecessor(const Number x) const {
        if (x < _min) return std::numeric_limits<Number>::max();
        const std::size_t blockIndex = _representatives.predecessorIndex(x);
        const auto begin = _input.begin() + blockIndex * W;
        const auto end = std::min(begin + W, _input.end());
        const auto result = *(std::lower_bound(begin, end, x, [](Number a, Number b) {
            return a <= b;
        }) - 1);
        return result;
    }

    inline std::size_t getSizeInBits(bool includeInput = false) const {
        std::size_t result = 0;
        result += _representatives.getSizeInBits();
        if (includeInput) {
            result += _input.size() * sizeof(Number) * 8;
        }
        return result;
    }

private:
    std::vector<Number> calculateRepresentatives(const std::vector<Number>& input) const {
        std::vector<Number> repr;
        repr.reserve(std::ceil(input.size() / W));
        for (std::size_t i = 0; i < input.size(); i += W) {
            repr.push_back(input[i]);
        }
        return repr;
    }
private:
    const std::size_t W;
    const std::vector<Number>& _input;
    // choose minimum as repr, so that the predecessor query to the xFastTrie
    // dircetly returns the blockIndex we are looking for
    const XFastTrie<std::unordered_map< Number, TrieNode, std::hash<Number>>> _representatives;
    const Number _min;
};
}