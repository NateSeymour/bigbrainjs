#include <bbjs/engine/Engine.h>
#include <gtest/gtest.h>

class ParserTestRunner : public ::testing::Test
{
protected:
    bbjs::Engine engine_;

    void Test(std::filesystem::path path)
    {
        auto es = this->engine_.Run(std::move(path));
        ASSERT_TRUE(es) << es.error().GetError();
    }
};

TEST_F(ParserTestRunner, ForLoop)
{
    this->Test("../test/parser/for-loop.mjs");
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
