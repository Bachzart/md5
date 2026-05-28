cmake_minimum_required(VERSION 3.29)

project(md5 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(md5 STATIC
    src/md5.cpp
    src/md5.h
)

target_include_directories(md5 PUBLIC src)

add_executable(test_md5 test/test.cpp)

target_link_libraries(test_md5 PRIVATE md5)