#ifndef ENGINE_H
#define ENGINE_H

#include <buffalo/buffalo.h>
#include <string_view>
#include "FeatureFlags.h"
#include "bbjs/parser/Parser.h"

namespace bbjs
{
    class Engine
    {
        auto parser_ = bf::SLRParser<parser::GrammarType>::Build(parser::root);
        FeatureFlags flags_;

    public:
        void Run(std::string_view file);

        Engine(FeatureFlags flags);
        Engine(std::string_view flags);
        Engine() = default;
    };
} // namespace bbjs

#endif // ENGINE_H
