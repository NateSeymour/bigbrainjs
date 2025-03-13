#include "bbjs/engine/SemanticVersion.h"
#include <ctre.hpp>

using namespace bbjs;

std::strong_ordering SemanticVersion::operator<=>(SemanticVersion const &other) const
{
    auto dmajor = this->major <=> other.major;
    if (dmajor != std::strong_ordering::equal) return dmajor;

    auto dminor = this->minor <=> other.minor;
    if (dminor != std::strong_ordering::equal) return dminor;

    return this->patch <=> other.patch;
}

SemanticVersion SemanticVersion::From(std::string_view raw)
{
    SemanticVersion version{};

    auto match = ctre::match<R"(v?(?<major>\d)(\.(?<minor>\d)(\.(?<patch>\d))?)?)">(raw);

    if (auto major = match.get<"major">())
    {
        version.major = std::stoi(major.to_string());
    }

    if (auto minor = match.get<"minor">())
    {
        version.minor = std::stoi(minor.to_string());
    }

    if (auto patch = match.get<"patch">())
    {
        version.patch = std::stoi(patch.to_string());
    }

    return version;
}
