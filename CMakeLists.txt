cmake_minimum_required(VERSION 3.22)

project(ads VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)

# main program
add_executable(ads_programm ads.cpp
                rmq/util/io.h)
target_include_directories(ads_programm  PRIVATE .)
target_compile_features(ads_programm  PUBLIC cxx_std_20)
target_compile_options(ads_programm  PUBLIC -Wall -Wextra -Wpedantic -O3 -g -march=native)

# exe for generating rmq instances
add_executable(generate_rmq rmq/util/generate_example.cpp)
target_include_directories(generate_rmq  PRIVATE .)
target_compile_features(generate_rmq  PUBLIC cxx_std_20)
target_compile_options(generate_rmq  PUBLIC -Wall -Wextra -Wpedantic -O3 -g -march=native)

# exe for generating pd instances
add_executable(generate_pd pd/util/generate_example.cpp)
target_include_directories(generate_pd  PRIVATE .)
target_compile_features(generate_pd  PUBLIC cxx_std_20)
target_compile_options(generate_pd  PUBLIC -Wall -Wextra -Wpedantic -O3 -g -march=native)

# rmq tests
set(GTEST_ROOT /usr/src/googletest)
find_path(GTEST_ROOT googletest/CMakeLists.txt HINTS ${GTEST_ROOT})
if (NOT ${GTEST_ROOT} STREQUAL GTEST_ROOT-NOTFOUND)
    enable_testing()

    add_subdirectory(${GTEST_ROOT} ${CMAKE_BINARY_DIR}/googletest EXCLUDE_FROM_ALL)

    add_executable(rmq_test rmq/tests/test.cpp)
    target_include_directories(rmq_test PRIVATE .)
    target_link_libraries(rmq_test gtest_main -fsanitize=leak -fsanitize=address -fsanitize=undefined)

    include(GoogleTest)
    gtest_discover_tests(rmq_test)
endif()

# pd tests
if (NOT ${GTEST_ROOT} STREQUAL GTEST_ROOT-NOTFOUND)
    enable_testing()

    add_executable(pd_test pd/tests/test.cpp)
    target_include_directories(pd_test PRIVATE .)
    target_link_libraries(pd_test gtest_main -fsanitize=leak -fsanitize=address -fsanitize=undefined)

    include(GoogleTest)
    gtest_discover_tests(pd_test)
endif()

# pd benchmarks
set(PD_IMPLS "manual;x_fast_trie;y_fast_trie")
foreach(impl ${PD_IMPLS})
  string(TOUPPER ${impl} impl_uc)
  add_executable(benchmark_${impl}  pd/tests/pd_benchmark.cpp
                 pd/implementation/${impl}.hpp)
  target_include_directories(benchmark_${impl}  PRIVATE .)
  target_compile_definitions(benchmark_${impl}  PRIVATE -D ${impl_uc})
  target_compile_features(benchmark_${impl} PUBLIC cxx_std_20)
  target_compile_options(benchmark_${impl}  PUBLIC -Wall -Wextra -Wpedantic -O3 -g -march=native)
  target_link_libraries(benchmark_${impl}  PRIVATE)
endforeach()

#rmq benchmarks
set(RMQ_IMPLS "nlogn;linear")
foreach(impl ${RMQ_IMPLS})
  string(TOUPPER ${impl} impl_uc)
  add_executable(benchmark_${impl}  rmq/tests/rmq_benchmark.cpp
                 rmq/implementation/${impl}.hpp)
  target_include_directories(benchmark_${impl}  PRIVATE .)
  target_compile_definitions(benchmark_${impl}  PRIVATE -D ${impl_uc})
  target_compile_features(benchmark_${impl} PUBLIC cxx_std_20)
  target_compile_options(benchmark_${impl}  PUBLIC -Wall -Wextra -Wpedantic -O3 -g -march=native)
  target_link_libraries(benchmark_${impl}  PRIVATE)
endforeach()