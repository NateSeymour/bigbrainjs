#include <gtest/gtest.h>
#include <v6turbo/engine/Engine.h>

class ParserTestRunner : public ::testing::Test
{
protected:
    v6::Engine engine_;

    void Test(std::filesystem::path path)
    {
        auto es = this->engine_.Run(std::move(path));
        ASSERT_TRUE(es) << es.error().GetError();

        auto result = es.value()->GetResult();
        ASSERT_EQ(result.get(), 0);
    }
};

TEST_F(ParserTestRunner, ForLoop)
{
    this->Test("../test/parser/for-loop.mjs");
}

TEST_F(ParserTestRunner, Functions)
{
    this->Test("../test/parser/functions.mjs");
}

TEST_F(ParserTestRunner, MultiComment)
{
    this->Test("../test/parser/multi-comment.mjs");
}

TEST_F(ParserTestRunner, SimpleHash)
{
    this->Test("../test/parser/simple-hash.mjs");
}

TEST_F(ParserTestRunner, SingleComment)
{
    this->Test("../test/parser/single-comment.mjs");
}

TEST_F(ParserTestRunner, Variables)
{
    this->Test("../test/parser/variables.mjs");
}
