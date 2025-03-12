#ifndef FEATUREFLAGS_H
#define FEATUREFLAGS_H

#include <string>
#include <unordered_map>
#include <variant>

namespace bbjs
{
    using FlagType = std::variant<bool, std::string>;
    using FeatureFlags = std::unordered_map<std::string, FlagType>;
} // namespace bbjs

#endif // FEATUREFLAGS_H
