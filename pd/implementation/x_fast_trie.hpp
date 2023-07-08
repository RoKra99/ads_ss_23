#pragma once
#include "../util/io.h"
#include <vector>
#include <array>
#include <unordered_map>

namespace ads_robert {

const Number FIRST_DIGIT_MASK = 1;
// no need to actually point at subtrees since we use the hash_map for traversing
struct TrieNode {
    // index in _leaves to maximum in left subtree (smaller than searched element)
    std::size_t max_left;
    // indexin _leaves to minimum in right subtree (larger than searched element)
    std::size_t min_right;
};

class XFastTrie {
public:
    XFastTrie(const std::vector<Number>& input) : _leaves(input) {
        for (auto a : _hash_table){
            a.reserve(_leaves.size());
        }
        for (std::size_t i = 0; i < _leaves.size(); ++i) {
            Number currentPrefix = _leaves[i];
            std::size_t lvl = 1;
            // overflow is unrealisic because of memory constraints
            // underflow should not be a problem since it is not a valid request
            _hash_table[0][currentPrefix] = { i - 1, i + 1 };
            while (lvl != W + 1) {
                bool goRight = currentPrefix & FIRST_DIGIT_MASK;
                // if I am on the right choose me, if I am to the left choose me because I am predecessor
                std::size_t min_right = goRight ? i : NOT_SET;
                // if I am on the left choose me, if I am to the right choose my predecessor
                std::size_t max_left = goRight ? NOT_SET : i;
                currentPrefix >>= 1;
                auto it = _hash_table[lvl].find(currentPrefix);
                if (it == _hash_table[lvl].end()) {
                    _hash_table[lvl][currentPrefix] = { max_left, min_right };
                } else if (goRight && (it->second.min_right > i || it->second.min_right == NOT_SET)) {
                    it->second.min_right = i;
                } else if (!goRight && (it->second.max_left < i || it->second.min_right == NOT_SET)) {
                    it->second.max_left = i;
                }
                ++lvl;
            }
        }
    }

    inline void print() const {
        for (std::size_t i = 1; i < W + 1; ++i) {
            for (const auto& entry : _hash_table[i]) {
                std::cout << "LVL: " << i << ", " << entry.second.max_left << ", " << entry.second.min_right << std::endl;
            }
        }
    }

    inline Number predecessor(const Number x) const {
        std::size_t low = W + 1;
        std::size_t high = 0;
        std::size_t middle;
        Number currentPrefix;
        TrieNode found = _hash_table[W].find(0)->second;
        std::size_t lvl = W;
        while (low - high > 1) {
            middle = (high + low) >> 1;
            currentPrefix = x >> middle;
            auto it = _hash_table[middle].find(currentPrefix);
            if (it == _hash_table[middle].end()) {
                high = middle;
            } else {
                low = middle;
                found = it->second;
                lvl = middle;
            }
        }

        if (lvl == 0) {
            return _leaves[found.max_left + 1];
        }
        const bool isOne = (x >> (lvl - 1)) & FIRST_DIGIT_MASK;
        if (isOne) {
            const auto min_right = found.min_right == NOT_SET ? std::min(found.max_left + 1, _leaves.size() - 1) : found.min_right;
            if (x < _leaves[min_right]) {
                return _leaves[min_right - 1];
            }
            return _leaves[min_right];
        } else {
            const auto max_left = found.max_left == NOT_SET ? std::max(0UL, found.min_right - 1) : found.max_left;
            return _leaves[max_left];
        }
    }

    inline std::size_t getSizeInBits() const {
        return 0;
    }
private:
    static const std::size_t W = 64;
    static const std::size_t NOT_SET = std::numeric_limits<std::size_t>::max();
    std::array<std::unordered_map< Number, TrieNode>, W + 1> _hash_table;
    const std::vector<Number>& _leaves;


};
}