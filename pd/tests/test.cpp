#include <gtest/gtest.h>
#include <numeric>
#include "../implementation/manual.hpp"
#include "../implementation/y_fast_trie.hpp"
#include "../implementation/x_fast_trie.hpp"
#include "../util/io.h"


const std::string input = "../pd/data/pd_10000.txt";

TEST(PDClassTest, compare_x) {
    ads_robert::PDInput pdInput = ads_robert::readPD(input);
    const auto min = *std::min_element(pdInput.numbers.begin(), pdInput.numbers.end());
    const auto max = *std::max_element(pdInput.numbers.begin(), pdInput.numbers.end());
    ads_robert::ManualPD manPD(pdInput.numbers);
    ads_robert::XFastTrie PD(pdInput.numbers);
    const std::size_t n_queries = pdInput.queries.size();
    for (const auto& query : pdInput.queries) {
        const auto res_ads = PD.predecessor(query);
        const auto res_man = manPD.predecessor(query);
        ASSERT_EQ(res_ads, res_man);
    }

    std::vector<ads_robert::Number> manual_queries = { min, max, 127 };
    for (const auto& query : manual_queries) {
        const auto res_ads = PD.predecessor(query);
        const auto res_man = manPD.predecessor(query);
        ASSERT_EQ(res_ads, res_man);
    }
}