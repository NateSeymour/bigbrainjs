#include "bbjs/engine/Engine.h"
#include <ctre.hpp>
#include <fstream>
#include <iostream>
#include <simdjson.h>

using namespace bbjs;

std::optional<std::filesystem::path> Engine::LocatePackageJson(std::filesystem::path path)
{
    if (!path.is_absolute())
    {
        path = std::filesystem::current_path() / path;
    }

    path.make_preferred();

    auto fs_root = path.root_path();
    while ((path = path.parent_path()) != fs_root)
    {
        if (!std::filesystem::is_directory(path))
        {
            continue;
        }

        std::filesystem::path package_json = path / "package.json";
        if (std::filesystem::exists(package_json))
        {
            return package_json;
        }
    }

    return std::nullopt;
}

Package Engine::LoadPackageJson(std::filesystem::path path)
{
    Package package{
            .root = path.parent_path(),
    };

    simdjson::ondemand::parser parser{};
    auto package_json = simdjson::padded_string::load(path.string());
    simdjson::ondemand::document doc = parser.iterate(package_json);

    auto root = doc.get_object();
    for (auto field : root.value())
    {
        std::string_view key = field.unescaped_key();
        auto value = field.value();

        if (key == "name")
        {
            package.name = value.get_string().value();
        }
        else if (key == "version")
        {
            package.version = SemanticVersion::From(value.get_string().value());
        }
        else if (key == "main")
        {
            package.main = value.get_string().value();
        }
        else if (key == "author")
        {
            package.author = value.get_string().value();
        }
        else if (key == "license")
        {
            package.license = value.get_string().value();
        }
        else if (key == "description")
        {
            package.description = value.get_string().value();
        }
    }

    return package;
}

std::expected<ExecutionSession, EngineError> Engine::Run(std::filesystem::path path)
{
    // Find and load the package.json
    auto package_path = Engine::LocatePackageJson(path);
    if (!package_path)
    {
        return std::unexpected<EngineError>{"Failed to locate package.json"};
    }

    Package package = Engine::LoadPackageJson(*package_path);

    // Initialize the ExecutionSession
    ExecutionSession es{};

    // Load program text
    std::ifstream script_stream(path.c_str());
    std::string script_text(std::istreambuf_iterator<char>{script_stream}, {});

    std::cout << script_text << std::endl;

    // Parse program text
    if (!this->parser_)
    {
        return std::unexpected<EngineError>{this->parser_.error().what()};
    }

    auto result = this->parser_->Parse(script_text);
    if (!result)
    {
        return std::unexpected<EngineError>{result.error().what()};
    }

    // Compile program text
    // Hand off assembly to ExecutionSession
    // Return ExecutionSession
    return std::move(es);
}

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
