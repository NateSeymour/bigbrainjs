#include <gtest/gtest.h>
#include <v6turbo/engine/Engine.h>

class EngineTest : public ::testing::Test
{
protected:
    v6::Engine engine_;

    void Test(std::filesystem::path path)
    {
        auto es = this->engine_.Run(std::move(path));
        ASSERT_TRUE(es) << es.error().GetError();
    }
};

TEST_F(EngineTest, ActivityMonitor)
{
    this->Test("test/activity-monitor.v6");
}
