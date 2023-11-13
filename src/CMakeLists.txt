cmake_minimum_required(VERSION 3.8)

project(trees LANGUAGES CXX)

add_executable(avl main.cc)
add_executable(compare main.cc)

target_compile_options(compare PUBLIC -DCOMPARE)
