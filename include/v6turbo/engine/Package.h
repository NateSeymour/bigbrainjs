#ifndef PACKAGE_H
#define PACKAGE_H

#include <map>
#include <optional>
#include <string>
#include <filesystem>
#include "SemanticVersion.h"

namespace v6
{
    struct Package
    {
        std::filesystem::path root;

        std::optional<std::string> name;
        std::optional<SemanticVersion> version;
        std::optional<std::string> type;
        std::optional<std::string> main;
        std::map<std::string, std::string> scripts;
        std::optional<std::string> author;
        std::optional<std::string> license;
        std::optional<std::string> description;
        std::map<std::string, SemanticVersion> dependencies;
    };
}

#endif //PACKAGE_H
