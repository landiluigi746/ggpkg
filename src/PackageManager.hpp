#pragma once

#include "utils/Utils.hpp"

namespace ggpkg
{
    struct PackageManagerInfo
    {
        std::string cmd;
        std::string version;
        std::string install;
    };

    Utils::Result<void> DetectPackageManager();
    Utils::Result<PackageManagerInfo> GetPackageManager();
} // namespace ggpkg
