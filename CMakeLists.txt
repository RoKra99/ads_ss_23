cmake_minimum_required(VERSION 3.22)

project(ads VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(ads_programm ads.cpp
                rmq/util/io.h)
target_include_directories(ads_programm  PRIVATE .)
target_compile_features(ads_programm  PUBLIC cxx_std_20)
target_compile_options(ads_programm  PUBLIC -Wall -Wextra -Wpedantic -O3 -g -march=native)

