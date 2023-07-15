#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include <limits>
#include <vector>
#include <thread>
#include "rmq/implementation/linear.hpp"
#include "rmq/implementation/naive.hpp"
#include "rmq/implementation/nlogn.hpp"


using NlogNRMQ = ads_robert::NlogNRMQ;
using LinearRMQ = ads_robert::LinearRMQ;

#ifdef NLOGN
#define RMQ NlogNRMQ
#define CALL_TEST(testfunction, it) \
    testfunction<RMQ>("nlogn", it)
#define OUTPUT "nlogn.txt"
#endif

#ifdef LINEAR
#define RMQ LinearRMQ
#define CALL_TEST(testfunction, it) \
    testfunction<RMQ>("linear", it)
#define OUTPUT "linear.txt"
#endif



/* MAIN TEST DESCRIPTION ******************************************************/
namespace test_time {
template <class T>
void print(std::ostream& out, const T& t, std::size_t  w) {
    out.width(w);
    out << t << " " << std::flush;
    std::cout.width(w);
    std::cout << t << " " << std::flush;
}

void print_headline(std::ostream& out) {
    print(out, "#algo", 8);
    print(out, "input", 8);
    print(out, "max", 8);
    print(out, "memory", 16);
    print(out, "constr", 8);
    print(out, "it", 4);
    print(out, "time", 8);
    out << std::endl;
    std::cout << std::endl;
}

void print_timing(std::ostream& out,
    std::string algo,
    std::size_t input,
    std::size_t max,
    std::size_t memory,
    double cons,
    std::size_t  it,
    double time
) {
    print(out, algo, 8);
    print(out, input, 8);
    print(out, max, 8);
    print(out, memory, 16);
    print(out, cons, 8);
    print(out, it, 4);
    print(out, time, 8);
    out << std::endl;
    std::cout << std::endl;
}
template<class RMQ>
int test(std::string algo, std::size_t  it) {
    const std::string PREFIX = "../rmq/data/";
    const std::string SUFFIX = "000000_q_10000000_p_64.txt";

    /* setup outputs ******************************************************/
    std::ofstream file(OUTPUT);
    print_headline(file);
    const std::string input_name = "rmq_n_";

    for (std::size_t inp = 1; inp <= 8; inp *= 2) {
        const auto& input_file = input_name + std::to_string(inp);
        ads_robert::RMQInput rmqInput = ads_robert::readRMQ(PREFIX + input_file + SUFFIX);
        for (std::size_t i = 0; i < it; ++i) {
            auto t0 = std::chrono::high_resolution_clock::now();
#ifdef NLOGN
            RMQ rmq(std::move(rmqInput.numbers));
#endif
#ifndef NLOGN
            RMQ rmq(rmqInput.numbers);
#endif
            auto t1 = std::chrono::high_resolution_clock::now();
            for (const auto& query : rmqInput.queries) {
                rmq.rmq(query.s, query.e);
            }
            auto t2 = std::chrono::high_resolution_clock::now();

            auto time_constr = std::chrono::duration_cast<std::chrono::microseconds>
                (t1 - t0).count() / 1000.;
            auto time_query = std::chrono::duration_cast<std::chrono::microseconds>
                (t2 - t1).count() / 1000.;
            print_timing(file, algo, inp, 64, rmq.getSizeInBits(false), time_constr, i, time_query);
        }
    }
    return 0;

}
}

int main(int argn, char** argc) {
    CALL_TEST(test_time::test, 5);
    return 0;
}