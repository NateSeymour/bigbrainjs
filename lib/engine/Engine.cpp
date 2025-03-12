#include "bbjs/engine/Engine.h"
#include <ctre.hpp>

using namespace bbjs;

void Engine::Run(std::string_view file) {}

Engine::Engine(FeatureFlags flags) : flags_(std::move(flags)) {}

Engine::Engine(std::string_view flags)
{
    auto result = ctre::tokenize<"--(?<flag>(?<name>([\\w\\-]+)(=\"(?<value>[^\"]+)\")?))">(flags);
    for (auto const &flag : result)
    {
        std::string_view name = flag.get<"name">();
        auto const &value = flag.get<"value">();

        if (value)
        {
            this->flags_[std::string(name)] = value.to_string();
        }
        else
        {
            this->flags_[std::string(name)] = true;
        }
    }
}
