#include <gtest/gtest.h>
#include "../implementation/linear.hpp"
#include "../implementation/naive.hpp"
#include "../implementation/nlogn.hpp"
#include "../util/io.h"


const std::string input = "../rmq/data/rmq_10000.txt";

TEST(RMQClassTest, compare_naive) {
    ads_robert::RMQInput rmqInput = ads_robert::readRMQ(input);
    ads_robert::NaiveRMQ RMQ(rmqInput.numbers);
    const std::size_t n_queries = rmqInput.queries.size();
    for (const auto& query : rmqInput.queries) {
        const auto res_ads = RMQ.rmq(query.s, query.e);
        const auto res_man = std::min_element(rmqInput.numbers.begin() + query.s, rmqInput.numbers.begin() + query.e + 1) - rmqInput.numbers.begin();
        ASSERT_EQ(res_ads, res_man);
    }

    std::vector<ads_robert::RMQ> manual_queries = { {0, rmqInput.n - 1}, {0,0}, {rmqInput.n - 1, rmqInput.n - 1}, {0,1}, {rmqInput.n - 2, rmqInput.n - 1} };
    for (const auto& query : manual_queries) {
        const auto res_ads = RMQ.rmq(query.s, query.e);
        const auto res_man = std::min_element(rmqInput.numbers.begin() + query.s, rmqInput.numbers.begin() + query.e + 1) - rmqInput.numbers.begin();
        ASSERT_EQ(res_ads, res_man);
    }
}

TEST(RMQClassTest, compare_log) {
    ads_robert::RMQInput rmqInput = ads_robert::readRMQ(input);
    const auto numbers = rmqInput.numbers;
    ads_robert::NlogNRMQ RMQ(std::move(rmqInput.numbers));
    const std::size_t n_queries = rmqInput.queries.size();
    for (const auto& query : rmqInput.queries) {
        const auto res_ads = RMQ.rmq(query.s, query.e);
        const auto res_man = std::min_element(numbers.begin() + query.s, numbers.begin() + query.e + 1) - numbers.begin();
        ASSERT_EQ(res_ads, res_man);
    }

    std::vector<ads_robert::RMQ> manual_queries = { {0, rmqInput.n - 1}, {0,0}, {rmqInput.n - 1, rmqInput.n - 1}, {0,1}, {rmqInput.n - 2, rmqInput.n - 1} };
    for (const auto& query : manual_queries) {
        const auto res_ads = RMQ.rmq(query.s, query.e);
        const auto res_man = std::min_element(rmqInput.numbers.begin() + query.s, rmqInput.numbers.begin() + query.e + 1) - rmqInput.numbers.begin();
        ASSERT_EQ(res_ads, res_man);
    }
}

TEST(RMQClassTest, compare_linear) {
    ads_robert::RMQInput rmqInput = ads_robert::readRMQ(input);
    ads_robert::LinearRMQ RMQ(rmqInput.numbers);
    const std::size_t n_queries = rmqInput.queries.size();
    for (const auto& query : rmqInput.queries) {
        const auto res_ads = RMQ.rmq(query.s, query.e);
        const auto res_man = std::min_element(rmqInput.numbers.begin() + query.s, rmqInput.numbers.begin() + query.e + 1) - rmqInput.numbers.begin();
        ASSERT_EQ(res_ads, res_man);
    }

    std::vector<ads_robert::RMQ> manual_queries = { {0, rmqInput.n - 1}, {0,0}, {rmqInput.n - 1, rmqInput.n - 1}, {0,1}, {rmqInput.n - 2, rmqInput.n - 1} };
    for (const auto& query : manual_queries) {
        const auto res_ads = RMQ.rmq(query.s, query.e);
        const auto res_man = std::min_element(rmqInput.numbers.begin() + query.s, rmqInput.numbers.begin() + query.e + 1) - rmqInput.numbers.begin();
        ASSERT_EQ(res_ads, res_man);
    }
}