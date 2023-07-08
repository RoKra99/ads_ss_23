#include <iostream>
#include "rmq/implementation/naive.hpp"
#include "rmq/implementation/nlogn.hpp"
#include "rmq/implementation/linear.hpp"
#include "pd/implementation/manual.hpp"
#include "pd/implementation/y_fast_trie.hpp"
#include "utils.h"
#include <chrono>



int main(int argn, char** argc) {
    if (argn != 4) {
        std::cout << "Invalide Anzahl Argumente " << argn << std::endl;
        std::cout << "Bitte mit folgendem Schema ausführen: './ads programm [pd|rmq] eingabe_datei ausgabe_datei'" << std::endl;
        return -1;
    }
    std::string mode, input, output;
    mode = argc[1];
    input = argc[2];
    output = argc[3];
    std::vector<ads_robert::Number> results;
    if (!mode.compare("pd")) {
        auto t0 = std::chrono::high_resolution_clock::now();
        ads_robert::PDInput pdInput = ads_robert::readPD(input);
        ads_robert::YFastTrie yFastTrie(pdInput.numbers);
        const std::size_t n_queries = pdInput.queries.size();
        results.reserve(n_queries);
        for (const auto& query : pdInput.queries) {
            const auto res = yFastTrie.predecessor(query);
            results.push_back(res);
        }
        auto t1 = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration_cast<std::chrono::microseconds>
            (t1 - t0).count() / 1000.;
        ads_robert::print_result(mode, time, yFastTrie.getSizeInBits());


    } else if (!mode.compare("pd_manual")) {
        auto t0 = std::chrono::high_resolution_clock::now();
        ads_robert::PDInput pdInput = ads_robert::readPD(input);
        ads_robert::ManualPD PD(pdInput.numbers);
        const std::size_t n_queries = pdInput.queries.size();
        results.reserve(n_queries);
        for (const auto& query : pdInput.queries) {
            const auto res = PD.predecessor(query);
            results.push_back(res);
        }
        auto t1 = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration_cast<std::chrono::microseconds>
            (t1 - t0).count() / 1000.;
        ads_robert::print_result(mode, time, PD.getSizeInBits());

    } else if (!mode.compare("rmq_naive")) {
        auto t0 = std::chrono::high_resolution_clock::now();
        ads_robert::RMQInput rmqInput = ads_robert::readRMQ(input);
        ads_robert::NaiveRMQ naiveRMQ(rmqInput.numbers);
        const std::size_t n_queries = rmqInput.queries.size();
        results.reserve(n_queries);
        for (const auto& query : rmqInput.queries) {
            const auto res = naiveRMQ.rmq(query.s, query.e);
            results.push_back(res);
        }
        auto t1 = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration_cast<std::chrono::microseconds>
            (t1 - t0).count() / 1000.;
        ads_robert::print_result(mode, time, naiveRMQ.getSizeInBits());

    } else if (!mode.compare("rmq_nlogn")) {
        auto t0 = std::chrono::high_resolution_clock::now();
        ads_robert::RMQInput rmqInput = ads_robert::readRMQ(input);
        ads_robert::NlogNRMQ nlognRMQ(std::move(rmqInput.numbers));
        const std::size_t n_queries = rmqInput.queries.size();
        results.reserve(n_queries);
        for (const auto& query : rmqInput.queries) {
            const auto res = nlognRMQ.rmq(query.s, query.e);
            results.push_back(res);
        }
        auto t1 = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration_cast<std::chrono::microseconds>
            (t1 - t0).count() / 1000.;
        ads_robert::print_result(mode, time, nlognRMQ.getSizeInBits());
    } else if (!mode.compare("rmq")) {
        auto t0 = std::chrono::high_resolution_clock::now();
        ads_robert::RMQInput rmqInput = ads_robert::readRMQ(input);
        ads_robert::LinearRMQ linearRMQ(rmqInput.numbers);
        const std::size_t n_queries = rmqInput.queries.size();
        results.reserve(n_queries);
        for (const auto& query : rmqInput.queries) {
            const auto res = linearRMQ.rmq(query.s, query.e);
            results.push_back(res);
        }
        auto t1 = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration_cast<std::chrono::microseconds>
            (t1 - t0).count() / 1000.;
        ads_robert::print_result(mode, time, linearRMQ.getSizeInBits());
    } else if (!mode.compare("rmq_manual")) {
        auto t0 = std::chrono::high_resolution_clock::now();
        ads_robert::RMQInput rmqInput = ads_robert::readRMQ(input);
        const std::size_t n_queries = rmqInput.queries.size();
        results.reserve(n_queries);
        for (const auto& query : rmqInput.queries) {
            const auto res = std::min_element(rmqInput.numbers.begin() + query.s, rmqInput.numbers.begin() + query.e + 1) - rmqInput.numbers.begin();
            results.push_back(res);
        }
        auto t1 = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration_cast<std::chrono::microseconds>
            (t1 - t0).count() / 1000.;
        ads_robert::print_result(mode, time, 0);
    } else {
        std::cout << "Invalider Aufruf" << std::endl;
        std::cout << "Bitte mit folgendem Schema ausführen: './ads programm [pd|rmq] eingabe_datei ausgabe_datei'" << std::endl;
        return -1;
    }

    ads_robert::write_results_to_file(output, results);
    return 0;
}