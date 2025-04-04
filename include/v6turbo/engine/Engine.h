#ifndef ENGINE_H
#define ENGINE_H

#include <buffalo/buffalo.h>
#include <expected>
#include <filesystem>
#include <string_view>
#include "FeatureFlags.h"
#include "Package.h"
#include "v6turbo/engine/ExecutionSession.h"
#include "v6turbo/parser/Parser.h"

namespace v6
{
    class EngineError
    {
        std::string error_;

    public:
        [[nodiscard]] char const *GetError() const
        {
            return this->error_.c_str();
        }

        EngineError(std::string error) : error_(std::move(error)) {}
    };

    class Engine
    {
        FeatureFlags flags_;

    protected:
        [[nodiscard]] static std::optional<std::filesystem::path> LocatePackageJson(std::filesystem::path path);
        [[nodiscard]] static Package LoadPackageJson(std::filesystem::path path);

    public:
        [[nodiscard]] std::expected<ExecutionSessionHandle, EngineError> Run(std::filesystem::path path);

        Engine(FeatureFlags flags);
        Engine(std::string_view flags);
        Engine() = default;
    };
} // namespace v6

#endif // ENGINE_H
