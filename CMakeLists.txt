cmake_minimum_required(VERSION 3.10)

project(vstring VERSION 0.1)

enable_testing()

add_subdirectory(tests)

set(CMAKE_C_COMPILER "gcc")

add_compile_options(-std=c99 -Wall -Werror -pedantic -fstack-clash-protection -fstack-protector-all -fstack-protector-strong
    -Werror=format-security -Werror=implicit-function-declaration -pipe -g -O2)

add_library(vstring src/vstring.c)

