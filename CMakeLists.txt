cmake_minimum_required(VERSION 3.24)
project(knn LANGUAGES CXX)
set(RUNTIME_OUTPUT_NAME "knn")
set(CMAKE_BUILD_PARALLEL_LEVEL 32)

add_subdirectory(tests)
