cmake_minimum_required(VERSION 3.12)
project(iq_rings)

set(CMAKE_CXX_STANDARD 17)
set(pybind11_DIR /home/kama/anaconda3/envs/rings/lib/python3.12/site-packages/pybind11/share/cmake/pybind11)

find_package(pybind11 REQUIRED)

add_library(${CMAKE_PROJECT_NAME} MODULE ${CMAKE_SOURCE_DIR}/py/exercise_wrapper.cpp)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/native_engine/src/headers)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE pybind11::module)
