#ifndef ENGINE_H
#define ENGINE_H

#include <buffalo/buffalo.h>
#include <expected>
#include <filesystem>
#include <string_view>
#include "FeatureFlags.h"
#include "Package.h"
#include "bbjs/parser/Parser.h"

namespace bbjs
{
    class ExecutionSession
    {
    public:
        ExecutionSession() = default;
    };

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
        std::expected<bf::SLRParser<parser::GrammarType>, bf::Error> parser_ = bf::SLRParser<parser::GrammarType>::Build(parser::root);
        FeatureFlags flags_;

    protected:
        [[nodiscard]] static std::optional<std::filesystem::path> LocatePackageJson(std::filesystem::path path);
        [[nodiscard]] static Package LoadPackageJson(std::filesystem::path path);

    public:
        [[nodiscard]] std::expected<ExecutionSession, EngineError> Run(std::filesystem::path path);

        Engine(FeatureFlags flags);
        Engine(std::string_view flags);
        Engine() = default;
    };
} // namespace bbjs

#endif // ENGINE_H
