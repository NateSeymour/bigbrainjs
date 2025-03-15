cmake_minimum_required(VERSION 3.30)
project(bigbrainjs)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)

# BUFFALO
set(BUFFALO_ENABLE_TESTS OFF)
FetchContent_Declare(
        buffalo
        GIT_REPOSITORY https://github.com/NateSeymour/buffalo.git
        GIT_TAG v2.0.11
)
FetchContent_MakeAvailable(buffalo)

# CTRE
FetchContent_Declare(
        ctre
        GIT_REPOSITORY https://github.com/hanickadot/compile-time-regular-expressions.git
        GIT_TAG v3.9.0
)
FetchContent_MakeAvailable(ctre)

# SIMDJSON
FetchContent_Declare(
        simdjson
        GIT_REPOSITORY https://github.com/simdjson/simdjson.git
        GIT_TAG v3.12.2
)
FetchContent_MakeAvailable(simdjson)

# GOOGLETEST
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
        lib/parser/Parser.cpp
        include/bbjs/engine/Package.h
        include/bbjs/engine/SemanticVersion.h
        lib/engine/SemanticVersion.cpp)
target_link_libraries(bigbrainjs PUBLIC buffalo ctre::ctre simdjson)
target_include_directories(bigbrainjs PUBLIC include)

add_executable(bigbrainjs-test test/scripts.test.cpp
        test/h3.test.cpp
        test/parser.test.cpp)
target_link_libraries(bigbrainjs-test PRIVATE gtest_main bigbrainjs)
