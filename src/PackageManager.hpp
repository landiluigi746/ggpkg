#pragma once

#include "Package.hpp"
#include "utils/Utils.hpp"

#include <array>
#include <cstdint>
#include <ranges>
#include <magic_enum/magic_enum.hpp>

namespace ggpkg
{
    enum class PackageManagerActionType : std::uint8_t
    {
        Install,
        Uninstall,
        Upgrade
    };

    struct PackageManagerAction
    {
        std::string cmd;
        bool batch;
    };

    using PackageManagerActions =
        std::array<PackageManagerAction, magic_enum::enum_count<PackageManagerActionType>()>;

    struct PackageManagerInfo
    {
        std::string cmd;
        std::string version;
        std::string update;
        std::string upgradeAll;
        PackageManagerActions actions;

        // std::string install;
        // std::string uninstall;
        // std::string upgrade;
        // bool installBatch;
        // bool uninstallBatch;
        // bool upgradeBatch;
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

    int PerformPackageManagerAction(PackageManagerActionType actionType,
                                    const PackageManagerInfo& packageManager,
                                    const AvailablePackages& availablePackages,
                                    const std::vector<std::string>& packageNames);

    // int InstallPackages(const PackageManagerInfo& packageManager,
    //                     const AvailablePackages& availablePackages,
    //                     const std::vector<std::string>& packageNames);

    // int UninstallPackages(const PackageManagerInfo& packageManager,
    //                       const AvailablePackages& availablePackages,
    //                       const std::vector<std::string>& packageNames);

    // int UpgradePackages(const PackageManagerInfo& packageManager,
    //                     const AvailablePackages& availablePackages,
    //                     const std::vector<std::string>& packageNames);
} // namespace ggpkg
