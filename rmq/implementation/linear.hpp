#pragma once 

#include "../util/io.h"
#include "../util/argmin.h"
#include "nlogn.hpp"
#include "naive.hpp"

#include <algorithm>
#include <math.h>
#include <deque>
#include <memory>

namespace ads_robert {

struct Node {
    Number value;
    Node* left, * right, * parent;
};

class LinearRMQ {
    using Identifier = std::uint32_t;

public:
    LinearRMQ(const std::vector<Number>& input) :
        _input(input),
        _n(input.size()),
        _s(std::max(1.0, std::log2(_n) / 4)),
        _m(std::ceil(static_cast<double>(_n) / _s)),
        _B_dash(),
        _cartesien_tree_identifiers(),
        _cartesian_solutions(std::pow(2, 2 * _s)),
        _B(std::move(calculateBlockMinima(input))) {}

    inline Number rmq(const Number s, const Number e) const {
        //std::cout << "s: " << s << ", e: " << e << ", _s: " << _s << ", _m: " << _m << std::endl;
        const Number startBlock = s / _s;
        const Number endBlock = e / _s;
        const bool wholeStart = (s % _s) == 0;
        const bool wholeEnd = ((e + 1) % _s) == 0;
        const Number blockDiff = endBlock - startBlock;
        if (blockDiff) {
            const Number sWhole = wholeStart ? startBlock : startBlock + 1;
            const Number eWhole = wholeEnd ? endBlock : endBlock - 1;
            //std::cout << sWhole << ", " << eWhole << std::endl;
            //std::cout << _input.size() << std::endl;
            const Number minBlock = _B.rmq(sWhole, eWhole);
            const Number idxInMinBlock = _B_dash[minBlock];
            Number result = minBlock * _s + idxInMinBlock;
            // std::cout << "minBlock: " << minBlock << ", idx: " << idxInMinBlock << std::endl;
            // std::cout << result << std::endl;
            if (!wholeStart) {
                result = argmin(_input, partialBlockRmq(startBlock, s, (startBlock + 1) * _s - 1), result);
                // std::cout << result << std::endl;
            }
            if (!wholeEnd) {
                result = argmin(_input, result, partialBlockRmq(endBlock, endBlock * _s, e));
                // std::cout << result << std::endl;
            }
            return result;
        } else if (wholeStart && wholeEnd) {
            const auto result = startBlock * _s + _B_dash[startBlock];
            // std::cout << result << std::endl;
            return result;

        } else {
            const auto result = partialBlockRmq(startBlock, s, e);
            std::cout << result << std::endl;

            return result;
        }
    }

    inline std::size_t getSizeInBits() const {
        std::size_t result = 0;
        result += _B_dash.size() * sizeof(Number) * 8;
        result += _cartesien_tree_identifiers.size() * sizeof(Identifier) * 8;
        for (const auto& cartesian : _cartesian_solutions) {
            if (cartesian != nullptr) {
                result += cartesian->getSizeInBits();
            }
        }
        result += _B.getSizeInBits(true);
        return result;
    }
private:

    inline Number partialBlockRmq(const Number blockNumber, const Number s, const Number e) const {
        const Number blockStart = blockNumber * _s;
        const Identifier id = _cartesien_tree_identifiers[blockNumber];
        const Number result = _cartesian_solutions[id]->rmq(s - blockStart, e - blockStart);
        return blockStart + result;
    }

    inline std::vector<Number> calculateBlockMinima(const std::vector<Number>& input) {
        std::vector<Number> mins;
        mins.reserve(_m);
        _B_dash.reserve(_m);
        _cartesien_tree_identifiers.reserve(_m);
        //std::cout << "start" << std::endl;
        for (std::size_t i = 0; i < input.size(); i += _s) {
            const std::size_t end = std::min(i + _s, _n);

            //std::cout << "before calc " << i << ", " << _s << std::endl;
            const Identifier id = calculateCartesianIdentifier(input.begin() + i, input.begin() + end);
            //std::cout << "aftercalc " << i << std::endl;
            if (_cartesian_solutions[id] == nullptr) {
                _cartesian_solutions[id] = std::unique_ptr<NaiveRMQ>(new NaiveRMQ(input.begin() + i, input.begin() + end));
            }
            _cartesien_tree_identifiers.push_back(id);
            const Number index = _cartesian_solutions[id]->rmq(0, _s - 1);
            _B_dash.push_back(index);
            mins.push_back(input[i + index]);
            //std::cout << "id: " << id << ", index: " << index <<  ", minval: " << input[i + index] << std::endl;
        }
        //std::cout << "done" << std::endl;
        return mins;
    }

    inline Identifier calculateCartesianIdentifier(const std::vector<Number>::const_iterator& begin, const std::vector<Number>::const_iterator end) {
        //std::cout << "increaseing from " << std::endl;

        std::vector<Node> nodes(end - begin);
        auto it = begin;
        std::size_t i = 0;
        Node* root = &nodes[i];
        // std::cout << "hi" << std::endl;
        nodes[i].value = *it;
        // std::cout << "boy" << std::endl;

        nodes[i].parent = nullptr;
        nodes[i].left = nullptr;
        nodes[i].right = nullptr;
        ++it;
        ++i;
        // std::cout << "increaseing from " << i << std::endl;

        for (; it != end; ++it) {
            nodes[i].value = *it;
            auto current = &nodes[i - 1];
            while (current->value > *it && current->parent != nullptr) {
                current = current->parent;
            }
            if (current->value > *it) {
                nodes[i].left = current;
                nodes[i].right = nullptr;
                nodes[i].parent = nullptr;
                current->parent = &nodes[i];
                root = &nodes[i];
            } else {
                nodes[i].left = current->right;
                nodes[i].parent = current;
                nodes[i].right = nullptr;
                current->right = &nodes[i];
            }
            // std::cout << "increaseing from " << i << std::endl;
            ++i;
        }
        // Go through in BFS order
        // Every actual Node is represented as a 0 and every nullptr as a 1
        // resulting in 2n bits (because we ignore the root node, since its always there)
        Identifier id = 0;
        std::deque<Node*> queue;
        queue.push_back(root->left);
        queue.push_back(root->right);
        Identifier mask = 1;
        while (!queue.empty()) {
            auto current = queue.front();
            queue.pop_front();
            if (current == nullptr) {
                id |= mask;
            } else {
                queue.push_back(current->left);
                queue.push_back(current->right);
            }
            mask <<= 1;
        }
        return id;
    }

private:
    const std::vector<Number>& _input;
    // size of _input
    const Number _n;
    // block size
    const Number _s;
    // number of blocks
    const Number _m;
    // position of minimum in each block
    std::vector<Number> _B_dash;
    std::vector<Identifier> _cartesien_tree_identifiers;
    std::vector<std::unique_ptr<NaiveRMQ>> _cartesian_solutions;
    NlogNRMQ _B;

};
}