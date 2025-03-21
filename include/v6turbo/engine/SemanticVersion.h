#ifndef SEMANTICVERSION_H
#define SEMANTICVERSION_H

#include <compare>
#include <string_view>

namespace v6
{
    struct SemanticVersion
    {
        int major = 0;
        int minor = 0;
        int patch = 0;

        std::strong_ordering operator<=>(SemanticVersion const &other) const;

        static SemanticVersion From(std::string_view raw);
    };
}

#endif //SEMANTICVERSION_H
