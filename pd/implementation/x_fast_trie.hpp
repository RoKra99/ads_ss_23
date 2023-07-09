#pragma once
#include "../util/io.h"
#include <vector>
#include <array>
#include <unordered_map>
#include <chrono>
#include <cmath>

namespace ads_robert {

const Number FIRST_DIGIT_MASK = 1;
// no need to actually point at subtrees since we use the hash_map for traversing
struct TrieNode {
    // index in _leaves to maximum in left subtree (smaller than searched element)
    std::size_t max_left;
    // indexin _leaves to minimum in right subtree (larger than searched element)
    std::size_t min_right;
};
template<class HashMap>
class XFastTrie {
public:
    XFastTrie(std::vector<Number>&& input) : W(std::ceil(std::log2(input[input.size() - 1]))), _hash_table(W + 1), _leaves(input) {
        for (std::size_t i = 0; i < W + 1; ++i) {
            _hash_table[i].reserve(std::min(_leaves.size(), 1UL << (W - i)));
        }
        for (std::size_t i = 0; i < _leaves.size(); ++i) {
            Number currentPrefix = _leaves[i];
            std::size_t lvl = 1;
            // overflow is unrealisic because of memory constraints
            // underflow should not be a problem since it is not a valid request
            _hash_table[0].insert({ currentPrefix, { i - 1, i + 1 } });
            while (lvl != W + 1) {
                bool goRight = currentPrefix & FIRST_DIGIT_MASK;
                // if I am on the right choose me, if I am to the left choose me because I am predecessor
                std::size_t min_right = goRight ? i : NOT_SET;
                // if I am on the left choose me, if I am to the right choose my predecessor
                std::size_t max_left = goRight ? NOT_SET : i;
                currentPrefix >>= 1;
                auto it = _hash_table[lvl].find(currentPrefix);
                if (it == _hash_table[lvl].end()) {
                    _hash_table[lvl].insert({ currentPrefix, { max_left, min_right } });
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
        const Number X = W < 64 ? std::min((1UL << W) - 1UL, x) : x;

        std::size_t low = W + 1;
        std::size_t high = 0;
        std::size_t middle;
        Number currentPrefix;
        TrieNode found = _hash_table[W].find(0)->second;
        std::size_t lvl = W;
        while (low - high > 1) {
            middle = (high + low) >> 1;
            currentPrefix = X >> middle;
            auto it = _hash_table[middle].find(currentPrefix);
            if (it == _hash_table[middle].end()) {
                high = middle;
            } else {
                low = middle;
                found = it->second;
                lvl = middle;
                //std::cout << "Found lvl: " << lvl << ", left: " << found.max_left << ", right: " << found.min_right << std::endl;
            }
        }

        if (lvl == 0) {
            return _leaves[found.max_left + 1];
        }
        const bool isOne = (x >> (lvl - 1)) & FIRST_DIGIT_MASK;
        if (isOne) {
            const auto min_right = found.min_right == NOT_SET ? std::min(found.max_left + 1, _leaves.size() - 1) : found.min_right;
            if (X < _leaves[min_right]) {
                return _leaves[min_right - 1];
            }
            return _leaves[min_right];
        } else {
            const auto max_left = found.max_left == NOT_SET ? std::max(0UL, found.min_right - 1) : found.max_left;
            return _leaves[max_left];
        }
    }

    inline std::size_t predecessorIndex(const Number x) const {
        //auto t0 = std::chrono::high_resolution_clock::now();
        const Number X = W < 64 ? std::min((1UL << W) - 1UL, x) : x;

        std::size_t low = W + 1;
        std::size_t high = 0;
        std::size_t middle;
        Number currentPrefix;
        auto found = _hash_table[W].find(0);
        std::size_t lvl = W;
        while (low - high > 1) {
            middle = (high + low) >> 1;
            currentPrefix = X >> middle;
            auto it = _hash_table[middle].find(currentPrefix);
            if (it == _hash_table[middle].end()) {
                high = middle;
            } else {
                low = middle;
                found = it;
                lvl = middle;
            }
        }
        //auto t1 = std::chrono::high_resolution_clock::now();

        const auto& node = found->second;
        if (lvl == 0) {
            // auto t2 = std::chrono::high_resolution_clock::now();
            // auto time_h = (t1 - t0).count();
            // auto time_r = (t2 - t1).count();
            // time_hash += time_h;
            // time_rest += time_r;
            return node.max_left + 1;
        }
        const bool isOne = (X >> (lvl - 1)) & FIRST_DIGIT_MASK;
        if (isOne) {
            const auto min_right = node.min_right == NOT_SET ? std::min(node.max_left + 1, _leaves.size() - 1) : node.min_right;
            if (x < _leaves[min_right]) {
                // auto t2 = std::chrono::high_resolution_clock::now();
                // auto time_h = (t1 - t0).count();
                // auto time_r = (t2 - t1).count();
                // time_hash += time_h;
                // time_rest += time_r;
                return min_right - 1;
            }
            // auto t2 = std::chrono::high_resolution_clock::now();
            // auto time_h = (t1 - t0).count();
            // auto time_r = (t2 - t1).count();
            // time_hash += time_h;
            // time_rest += time_r;
            return min_right;
        } else {
            const auto max_left = node.max_left == NOT_SET ? std::max(0UL, node.min_right - 1) : node.max_left;
            // auto t2 = std::chrono::high_resolution_clock::now();
            // auto time_h = (t1 - t0).count();
            // auto time_r = (t2 - t1).count();
            // time_hash += time_h;
            // time_rest += time_r;
            return max_left;
        }
    }

    inline std::size_t getSizeInBits() const {
        std::size_t result = 0;
        for (const auto& h : _hash_table) {
            result += h.size() * sizeof(*h.begin()) * 8;
        }
        result += _leaves.size() * sizeof(Number) * 8;
        return result;
    }
    // inline void printTimes() const {
    //     std::cout << "Hash: " << (time_hash/ 1000000.) << std::endl;
    //     std::cout << "Rest: " << (time_rest / 1000000.) << std::endl;
    // }
private:
    const std::size_t W;
    static const std::size_t NOT_SET = std::numeric_limits<std::size_t>::max();
    std::vector<HashMap> _hash_table;
    const std::vector<Number> _leaves;
    // int64_t time_hash;
    // int64_t time_rest;
};
}