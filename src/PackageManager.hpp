#pragma once

#include "Package.hpp"
#include "utils/Utils.hpp"

#include <ranges>

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

    inline bool IsPackageAvailable(const PackageInstallInfo& package,
                                   const PackageManagerInfo& packageManager)
    {
        return package.providers.contains(packageManager.cmd);
    }

    inline auto GetAvailablePackages(const Packages& packages, const PackageManagerInfo& packageManager)
    {
        return packages | std::views::filter([&packageManager](const auto& package) {
                   return IsPackageAvailable(package, packageManager);
               }) |
               std::views::transform([&packageManager](const auto& package) {
                   return std::make_pair(package.name, package.providers.at(packageManager.cmd));
               });
    }
} // namespace ggpkg
