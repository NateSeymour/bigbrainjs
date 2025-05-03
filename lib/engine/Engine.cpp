#include "v6turbo/engine/Engine.h"
#include <ctre.hpp>
#include <fstream>
#include <simdjson.h>

using namespace v6;

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
        else if (key == "type")
        {
            package.type = value.get_string().value();
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

std::expected<ExecutionSessionHandle, EngineError> Engine::Run(std::filesystem::path path)
{
    if (!std::filesystem::is_directory(path) || path.extension() != ".v6")
    {
        return std::unexpected<EngineError>{"Expected a directory with the extension \".v6\"."};
    }

    auto const package_path = path / "package.json";
    if (!std::filesystem::exists(package_path))
    {
        return std::unexpected<EngineError>{"Expected v6 directory to contain a top-level package.json."};
    }

    Package package = Engine::LoadPackageJson(package_path);
    if (!package.main.has_value())
    {
        return std::unexpected<EngineError>{"Expected v6 application to have a defined entrypoint."};
    }

    if (package.type != "module")
    {
        return std::unexpected<EngineError>{"v6turbo only supports ECMAScript modules and requires them to be explicitly defined in the package.json."};
    }

    ExecutionSessionHandle es = std::make_unique<ExecutionSession>(std::move(package));

    es->RunMain();

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
