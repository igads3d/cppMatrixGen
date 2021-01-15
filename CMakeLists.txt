cmake_minimum_required(VERSION 3.17)
project(libmatrixgen)

set(CMAKE_CXX_STANDARD 17)

add_library(libmatrixgen library.cpp library.h libmatrixgen_common.h vector_matrix.cpp vector_matrix.h)