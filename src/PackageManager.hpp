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
        std::string uninstall;
        std::string update;
        std::string upgradeAll;
        bool installBatch;
        bool uninstallBatch;
    };

    Utils::Result<void> DetectPackageManager();
    Utils::Result<PackageManagerInfo> GetPackageManager();

    inline AvailablePackages GetAvailablePackages(const Packages& packages,
                                                  const PackageManagerInfo& packageManager)
    {
        auto view = packages | std::views::filter([&packageManager](const PackageInstallInfo& package) {
                        return package.providers.contains(packageManager.cmd);
                    }) |
                    std::views::transform([&packageManager](const PackageInstallInfo& package) {
                        return std::make_pair(package.name, package.providers.at(packageManager.cmd));
                    });

        return AvailablePackages(std::ranges::begin(view), std::ranges::end(view));
    }

    int InstallPackages(const PackageManagerInfo& packageManager,
                        const AvailablePackages& availablePackages,
                        const std::vector<std::string>& packageNames);

    int UninstallPackages(const PackageManagerInfo& packageManager,
                          const AvailablePackages& availablePackages,
                          const std::vector<std::string>& packageNames);
} // namespace ggpkg
