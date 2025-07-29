#pragma once

#include "utils/Utils.hpp"
#include "utils/StringHash.hpp"

#include <string>
#include <unordered_map>

namespace ggpkg
{
    struct PackageInstallInfo
    {
        std::string name;
        std::unordered_map<std::string, std::string, Utils::StringHash, std::equal_to<>> providers;
    };

    using Packages = std::vector<PackageInstallInfo>;

    Utils::Result<Packages> GetPackages();
}