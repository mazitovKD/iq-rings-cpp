cmake_minimum_required(VERSION 3.10.2)
project(iq_rings)

set(CMAKE_CXX_STANDARD 17)

include_directories(cpp/src)

add_library(exercise_engine SHARED
    cpp/src/details.hpp
    cpp/src/elements.hpp
    cpp/src/exercise.hpp
    cpp/src/json.hpp
    cpp/src/utils.hpp
    cpp/src/ExerciseJNI.cpp  # JNI интерфейсный файл
)

find_library(log-lib log)

target_link_libraries(iq_rings ${log-lib})
