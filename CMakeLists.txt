cmake_minimum_required(VERSION 3.30)
project(v6turbo)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)

# BUFFALO
set(BUFFALO_ENABLE_TESTS OFF)
FetchContent_Declare(
        buffalo
        GIT_REPOSITORY https://github.com/NateSeymour/buffalo.git
        GIT_TAG v3.0.4
)
FetchContent_MakeAvailable(buffalo)

# CITY
set(CITY_ENABLE_TESTS OFF)
FetchContent_Declare(
        city
        GIT_REPOSITORY https://github.com/NateSeymour/city.git
        GIT_TAG v0.4.4
)
FetchContent_MakeAvailable(city)

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

# GOOGLE TEST
FetchContent_Declare(
        gtest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG main
)
FetchContent_MakeAvailable(gtest)

# GOOGLE BENCHMARK
FetchContent_Declare(
        benchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG main
)
FetchContent_MakeAvailable(benchmark)

add_library(v6turbo
        lib/engine/Engine.cpp
        include/v6turbo/engine/Engine.h
        include/v6turbo/engine/FeatureFlags.h
        include/v6turbo/parser/Node.h
        include/v6turbo/parser/Parser.h
        lib/parser/Parser.cpp
        include/v6turbo/engine/Package.h
        include/v6turbo/engine/SemanticVersion.h
        lib/engine/SemanticVersion.cpp
        include/v6turbo/engine/ExecutionSession.h
        lib/engine/ExecutionSession.cpp
        lib/compiler/Compiler.cpp
        lib/compiler/Compiler.h
)
target_link_libraries(v6turbo PUBLIC buffalo city ctre::ctre simdjson)
target_include_directories(v6turbo PUBLIC include)

add_executable(vroom
        src/main.cpp
)
target_link_libraries(vroom PUBLIC v6turbo)

add_executable(v6turbo-test
        test/engine.test.cpp)
target_link_libraries(v6turbo-test PRIVATE gtest_main v6turbo)
