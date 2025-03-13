#include <bbjs/engine/Engine.h>
#include <gtest/gtest.h>
#include <simdjson.h>

TEST(H3, Request)
{
    bbjs::Engine engine{};

    auto es = engine.Run(std::filesystem::path{"../test/h3/server.mjs"});

    ASSERT_TRUE(es) << es.error().GetError();
}
