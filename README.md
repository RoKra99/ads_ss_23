# ads_ss_23

Project für Vorlesung "Fortgeschrittene Datenstrukturen" SS23

### Requirements:

- cmake Version >= 3.22
- compiler supporting c++20

### How to run:

- mkdir build && cd build && cmake .. && make
- run with ./ads_programm [pd|rmq] eingabe_datei ausgabe_datei

### How to run tests:

The tests compare the output of the implemented Data-structures/Algorithms to a straight forward algorithm preferrable using a standard library algorithm. For the predecessor queries the output is compared to "std::lower_bound" (see pd/implementation/manual.hpp). For the RMQ queries the output is compared to "std::min_element". To change the input for the test, change the input-variable at the top of the files and rebuild. Compare_naive is commented out since it consumes too much memory for the rmq_example_1.txt input, but it is indirectly tested through use in the other data structures. Alternatively you can generate a smaller test input to run the tests for the naive implementation.

* build the projekt (see section above)
* ./pd_test (for predecessor)
* ./rmq_test (for range minimum queries)


### How to generate test instances:

All parameters have default values. The default output writes to [pd|rmq]/data/ respectively

* build the projekt (see section above)
* ./generate_pd (-n länge_der_eingabe) (-out output_datei) (-q anzahl_der_queries) (-max maximum in der eingabe)
* ./generate_rmq (-n länge_der_eingabe) (-out output_datei) (-q anzahl_der_queries) (-max maximum in der eingabe)
