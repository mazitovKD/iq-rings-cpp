cmake_minimum_required(VERSION 3.12)
project(iq_rings)

set(CMAKE_CXX_STANDARD 17)
set(pybind11_DIR /home/kama/anaconda3/envs/rings/lib/python3.12/site-packages/pybind11/share/cmake/pybind11)

find_package(pybind11 REQUIRED)

add_library(iq_rings MODULE exercise_wrapper.cpp)

target_include_directories(iq_rings PRIVATE ${CMAKE_SOURCE_DIR}/cpp/src)
target_link_libraries(iq_rings PRIVATE pybind11::module)
