#include <benchmark/benchmark.h>
#include <v6turbo/engine/Engine.h>

static void BenchmarkParserH3(benchmark::State &state)
{
    for (auto _ : state)
    {
        v6::Engine engine{};

        auto res = engine.Run("../benchmark/h3/h3.mjs");

        if (!res.has_value())
        {
            throw std::runtime_error(res.error().GetError());
        }
    }
}
BENCHMARK(BenchmarkParserH3);
