cmake_minimum_required(VERSION 3.6)
project(threads_project_2)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES example.cpp)
add_executable(threads_project_2 ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(threads_project_2 pthread)
