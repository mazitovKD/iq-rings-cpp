cmake_minimum_required(VERSION 3.10.2)
project(NativeRingsEngine) # IQRings

set(CMAKE_CXX_STANDARD 17)
include_directories(${PROJECT_SOURCE_DIR}/src/main/cpp/headers) # path to dir with C++ headers

add_library(${CMAKE_PROJECT_NAME} SHARED ${PROJECT_SOURCE_DIR}/src/main/cpp/ExerciseJNI.cpp) # path to JNI itnterface file

find_library(log-lib log)

target_link_libraries(${CMAKE_PROJECT_NAME} ${log-lib})
