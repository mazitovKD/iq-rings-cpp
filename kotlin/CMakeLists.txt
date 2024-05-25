cmake_minimum_required(VERSION 3.10.2)
project(iq_rings)

set(CMAKE_CXX_STANDARD 17)
include_directories(cpp/src) # path to dir with C++ headers

add_library(iq_rings SHARED kotlin/ExerciseJNI.cpp) # path to JNI itnterface file

find_library(log-lib log)

target_link_libraries(iq_rings ${log-lib})
