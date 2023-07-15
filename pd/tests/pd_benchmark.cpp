#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include <limits>
#include <vector>
#include <thread>
#include "pd/implementation/manual.hpp"
#include "pd/implementation/x_fast_trie.hpp"
#include "pd/implementation/y_fast_trie.hpp"

using ManualPD = ads_robert::ManualPD;
using XFastTrie = ads_robert::XFastTrie<std::unordered_map<ads_robert::Number, ads_robert::TrieNode, std::hash<ads_robert::Number>>>;
using YFastTrie = ads_robert::YFastTrie;
#ifdef MANUAL
#define PD ManualPD
#define CALL_TEST(testfunction, it) \
    testfunction<PD>("manual", it)
#define OUTPUT "manual.txt"
#endif

#ifdef X_FAST_TRIE
#define PD XFastTrie
#define CALL_TEST(testfunction, it) \
    testfunction<PD>("xfast", it)
#define OUTPUT "x_fast_trie.txt"
#endif

#ifdef Y_FAST_TRIE
#define PD YFastTrie
#define CALL_TEST(testfunction, it) \
    testfunction<PD>("yfast", it)
#define OUTPUT "y_fast_trie.txt"
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
template<class PD>
int test(std::string algo, std::size_t  it) {
    const std::string PREFIX = "../pd/data/";
    const std::string SUFFIX = ".txt";

    /* setup outputs ******************************************************/
    std::ofstream file(OUTPUT);
    print_headline(file);

    const std::vector<std::string> inputs = { "pd_n_1000000_q_1000000_p_64", "pd_n_2000000_q_1000000_p_64", "pd_n_4000000_q_1000000_p_64","pd_n_8000000_q_1000000_p_64" };
    for (std::size_t inp = 0; inp < 4; ++inp) {
        const auto& input_file = inputs[inp];
        const std::size_t input_size = 1UL << inp;
        ads_robert::PDInput pdInput = ads_robert::readPD(PREFIX + input_file + SUFFIX);
        for (std::size_t i = 0; i < it; ++i) {
            auto t0 = std::chrono::high_resolution_clock::now();
#ifdef X_FAST_TRIE
            PD pd(std::move(pdInput.numbers));
#endif
#ifndef X_FAST_TRIE
            PD pd(pdInput.numbers);
#endif
            auto t1 = std::chrono::high_resolution_clock::now();
            for (const auto& query : pdInput.queries) {
                pd.predecessor(query);
            }
            auto t2 = std::chrono::high_resolution_clock::now();

            auto time_constr = std::chrono::duration_cast<std::chrono::microseconds>
                (t1 - t0).count() / 1000.;
            auto time_query = std::chrono::duration_cast<std::chrono::microseconds>
                (t2 - t1).count() / 1000.;
            print_timing(file, algo, input_size, 64, pd.getSizeInBits(false), time_constr, i, time_query);
        }
    }
    const std::string input_name = "pd_n_8000000_q_1000000_p_";

    for (std::size_t inp = 32; inp < 64; inp += 8) {
        const auto& input_file = input_name + std::to_string(inp);
        ads_robert::PDInput pdInput = ads_robert::readPD(PREFIX + input_file + SUFFIX);
        for (std::size_t i = 0; i < it; ++i) {
            auto t0 = std::chrono::high_resolution_clock::now();
#ifdef X_FAST_TRIE
            PD pd(std::move(pdInput.numbers));
#endif
#ifndef X_FAST_TRIE
            PD pd(pdInput.numbers);
#endif
            auto t1 = std::chrono::high_resolution_clock::now();
            for (const auto& query : pdInput.queries) {
                pd.predecessor(query);
            }
            auto t2 = std::chrono::high_resolution_clock::now();

            auto time_constr = std::chrono::duration_cast<std::chrono::microseconds>
                (t1 - t0).count() / 1000.;
            auto time_query = std::chrono::duration_cast<std::chrono::microseconds>
                (t2 - t1).count() / 1000.;
            print_timing(file, algo, 8, inp, pd.getSizeInBits(false), time_constr, i, time_query);
        }
    }
    return 0;

}
}

int main(int argn, char** argc) {
    CALL_TEST(test_time::test, 5);
    return 0;
}