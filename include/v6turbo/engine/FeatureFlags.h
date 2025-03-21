#ifndef FEATUREFLAGS_H
#define FEATUREFLAGS_H

#include <string>
#include <unordered_map>
#include <variant>

namespace v6
{
    using FlagType = std::variant<bool, std::string>;
    using FeatureFlags = std::unordered_map<std::string, FlagType>;
} // namespace v6

#endif // FEATUREFLAGS_H
