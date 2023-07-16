# ads_ss_23

Project für Vorlesung "Fortgeschrittene Datenstrukturen" SS23

### Requirements:

- cmake Version >= 3.22
- compiler supporting c++20
- R (for creating plots)

### How to run:

- mkdir build && cd build && cmake .. && make
- run with ./ads_programm [pd|rmq] eingabe_datei ausgabe_datei

### How to run tests:

The tests compare the output of the implemented Data-structures/Algorithms to a straight forward algorithm preferrable using a standard library algorithm. For the predecessor queries the output is compared to "std::lower_bound" (see pd/implementation/manual.hpp). For the RMQ queries the output is compared to "std::min_element". To change the input for the test, change the input-variable at the top of the files and rebuild. Alternatively you can generate a smaller test input to run the tests for the naive implementation. I am using a smaller test instance for rmq, since the naive approach conusmes, too much memory for larger inputs. Additionaly "std::min_element" is very slow.

* build the projekt (see section above)
* navigate to build directory
* add predecessor_example_1.txt to pd/data (test instance for pd)
* run ./generate_rmq -n 10000 (test instance for rmq)
* run ./pd_test (for predecessor)
* run ./rmq_test (for range minimum queries)

### How to generate test instances:

All parameters have default values. The default output writes to [pd|rmq]/data/ respectively

* build the projekt (see section above)
* ./generate_pd (-n länge_der_eingabe) (-out output_datei) (-q anzahl_der_queries) (-max log(max der eingabe))
* ./generate_rmq (-n länge_der_eingabe) (-out output_datei) (-q anzahl_der_queries) (-max log(max der eingabe))

### How to run benchmarks:

* build the projekt
* navigate to build directory
* create the benchmark instances (see "Testinstances for evaluation")
* run ./benchmark_linear
* run ./benchmark_nlogn
* run ./benchmark_x_fast_trie
* run ./benchmark_y_fast_trie
* run ./benchmark_manual

### How to create plots:

* run the benchmarks
* create to directory "eval"
* run Rscript make_plots.R

### Testinstances for evaluation:

###### Increasing input size (PD)

* ./generate_pd -n 1000000 -q 1000000
* ./generate_pd -n 2000000 -q 1000000
* ./generate_pd -n 4000000 -q 1000000
* ./generate_pd -n 8000000 -q 1000000

As one command:

./generate_pd -n 1000000 -q 1000000 && ./generate_pd -n 2000000 -q 1000000 && ./generate_pd -n 4000000 -q 1000000 && ./generate_pd -n 8000000 -q 1000000

###### Increasing  maximum number (PD)

* ./generate_pd -n 8000000 -q 1000000 -max 56
* ./generate_pd -n 8000000 -q 1000000 -max 48
* ./generate_pd -n 8000000 -q 1000000 -max 40
* ./generate_pd -n 8000000 -q 1000000 -max 32

As one command:

./generate_pd -n 8000000 -q 1000000 -max 56 && ./generate_pd -n 8000000 -q 1000000 -max 48 && ./generate_pd -n 8000000 -q 1000000 -max 40 && ./generate_pd -n 8000000 -q 1000000 -max 32

###### Increasing input size (RMQ)

* ./generate_rmq -n 1000000 -q 10000000
* ./generate_rmq -n 2000000 -q 10000000
* ./generate_rmq -n 4000000 -q 10000000
* ./generate_rmq -n 8000000 -q 10000000

As one command:

./generate_rmq -n 1000000 -q 10000000 && ./generate_rmq -n 2000000 -q 10000000 && ./generate_rmq -n 4000000 -q 10000000 && ./generate_rmq -n 8000000 -q 10000000
