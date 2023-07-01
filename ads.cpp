#include <iostream>
#include "rmq/implementation/naive.hpp"
#include "rmq/implementation/nlogn.hpp"
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
        // do pd stuff
    } else if (!mode.compare("rmq_naive")) {
        // do rmd stuff
        auto t0 = std::chrono::high_resolution_clock::now();
        ads_robert::RMQInput rmqInput = ads_robert::readRMQ(input);
        ads_robert::NaiveRMQ naiveRMQ(rmqInput.numbers);
        const std::size_t n_queries = rmqInput.queries.size();
        results.reserve(n_queries);
        for (const auto& query : rmqInput.queries) {
            const auto res = naiveRMQ.rmq(query.s, query.e);
            std::cout << res << std::endl;

            results.push_back(res);
        }
        auto t1 = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration_cast<std::chrono::microseconds>
            (t1 - t0).count() / 1000.;
        ads_robert::print_result(mode, time, 0);

    } else if (!mode.compare("rmq")) {
        auto t0 = std::chrono::high_resolution_clock::now();
        ads_robert::RMQInput rmqInput = ads_robert::readRMQ(input);
        ads_robert::NlogNRMQ nlognRMQ(rmqInput.numbers);
        const std::size_t n_queries = rmqInput.queries.size();
        results.reserve(n_queries);
        for (const auto& query : rmqInput.queries) {
            // std::cout << query.s << ", "<<  query.e << std::endl;
            const auto res = nlognRMQ.rmq(query.s, query.e);
            //std::cout << res << std::endl;
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