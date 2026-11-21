#pragma once

#include "Package.hpp"
#include "utils/Utils.hpp"

#include <array>
#include <cstdint>
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

        bool HasAction(PackageManagerActionType actionType) const noexcept
        {
            return !actions[(size_t) actionType].cmd.empty();
        }
    };

    Utils::Result<PackageManagerInfo> DetectPackageManager();
    Utils::Result<PackageManagerInfo> GetPackageManager();

    void RemoveDuplicatePackages(std::vector<std::string>& packageNames);

    void EraseUnavailablePackages(PackageManagerActionType actionType, const Packages& packages,
                                  std::vector<std::string>& packageNames);

    int PerformPackageManagerAction(PackageManagerActionType actionType,
                                    const PackageManagerInfo& packageManager, const Packages& packages,
                                    const std::vector<std::string>& packageNames);
} // namespace ggpkg
