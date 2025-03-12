cmake_minimum_required(VERSION 3.30)
project(bigbrainjs)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
set(BUFFALO_ENABLE_TESTS OFF)
FetchContent_Declare(
        buffalo
        GIT_REPOSITORY https://github.com/NateSeymour/buffalo.git
        GIT_TAG v1.1.2
)
FetchContent_MakeAvailable(buffalo)

FetchContent_Declare(
        ctre
        GIT_REPOSITORY https://github.com/hanickadot/compile-time-regular-expressions.git
        GIT_TAG v3.9.0
)
FetchContent_MakeAvailable(ctre)

FetchContent_Declare(
        gtest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG main
)
FetchContent_MakeAvailable(gtest)

add_library(bigbrainjs
        lib/engine/Engine.cpp
        include/bbjs/engine/Engine.h
        include/bbjs/engine/FeatureFlags.h
        include/bbjs/parser/Node.h
        include/bbjs/parser/Parser.h
        lib/parser/Parser.cpp)
target_link_libraries(bigbrainjs PRIVATE buffalo ctre::ctre)
target_include_directories(bigbrainjs PUBLIC include)

add_executable(bigbrainjs-test test/scripts.test.cpp)
target_link_libraries(bigbrainjs-test PRIVATE gtest_main bigbrainjs)
