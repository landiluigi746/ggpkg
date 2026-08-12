#include "PackageManager.hpp"
#include "Config.hpp"
#include "utils/Utils.hpp"

#include <filesystem>
#include <magic_enum/magic_enum.hpp>
#include <print>
#include <system_error>
#include <glaze/json/read.hpp>
#include <glaze/json/write.hpp>

namespace ggpkg
{
    static bool TestPackageManager(const PackageManagerInfo& packageManager)
    {
        std::print("Testing {} ... ", packageManager.cmd);

        if (Utils::SilentSystem(std::format("{} {}", packageManager.cmd, packageManager.version)))
        {
            std::println("Failed");
            return false;
        }

        std::println("Passed !");
        return true;
    }

    static Utils::Result<void> SavePackageManager(const PackageManagerInfo& packageManager)
    {
        std::string buffer;

        std::error_code dirEc;
        std::filesystem::create_directories(Config::PACKAGE_MANAGER_CONFIG_PATH.parent_path(), dirEc);

        if (dirEc)
            return Utils::Error(dirEc.message());

        if (auto ec = glz::write_file_json(packageManager, Config::PACKAGE_MANAGER_CONFIG_PATH.string(),
                                           buffer);
            ec)
        {
            return Utils::Error("Failed to write package manager configuration");
        }

        return Utils::Result<void>();
    }

    Utils::Result<void> DetectPackageManager()
    {
        auto options = std::to_array<PackageManagerInfo>({
#if defined(_WIN32)
            PackageManagerInfo{
                .cmd = "winget",
                .version = "-v",
                .update = "source update",
                .upgradeAll = "upgrade --all",
                .actions =
                    PackageManagerActions{
                        PackageManagerAction{
                            .cmd = "install -e --id",
                            .batch = false,
                        },
                        PackageManagerAction{
                            .cmd = "uninstall",
                            .batch = false,
                        },
                        PackageManagerAction{
                            .cmd = "upgrade",
                            .batch = false,
                        },
                    },
            },
#elif defined(__linux__)
            PackageManagerInfo{
                .cmd = "pacman",
                .version = "-V",
                .update = "-Sy",
                .upgradeAll = "-Syu",
                .actions =
                    PackageManagerActions{
                        PackageManagerAction{
                            .cmd = "-S",
                            .batch = true,
                        },
                        PackageManagerAction{
                            .cmd = "-R",
                            .batch = true,
                        },
                        PackageManagerAction{
                            .cmd = "",
                            .batch = false,
                        },
                    },
            },
            PackageManagerInfo{
                .cmd = "apt",
                .version = "-v",
                .update = "update",
                .upgradeAll = "upgrade",
                .actions =
                    PackageManagerActions{
                        PackageManagerAction{
                            .cmd = "install",
                            .batch = true,
                        },
                        PackageManagerAction{
                            .cmd = "remove",
                            .batch = true,
                        },
                        PackageManagerAction{
                            .cmd = "install --only-upgrade",
                            .batch = true,
                        },
                    },
            }
#endif
        });

        for (const auto& option : options)
        {
            if (!TestPackageManager(option))
                continue;

            return SavePackageManager(option);
        }

        return Utils::Error("None of the supported package managers is available");
    }

    Utils::Result<PackageManagerInfo> GetPackageManager()
    {
        PackageManagerInfo packageManagerInfo;
        const std::string pathStr = Config::PACKAGE_MANAGER_CONFIG_PATH.string();

        if (!std::filesystem::is_regular_file(Config::PACKAGE_MANAGER_CONFIG_PATH))
            return Utils::Error(std::format("Package manager config file ({}) does not exist. "
                                            "Configure it automatically using `ggpkg configure`",
                                            pathStr));

        std::string buffer;
        if (auto ec = glz::read_file_json(packageManagerInfo, pathStr, buffer); ec)
            return Utils::Error("Failed to read package manager info from configuration");

        if (auto ec = TestPackageManager(packageManagerInfo); !ec)
            return Utils::Error(std::format(
                "The package manager in your configuration file ({}) did not pass the test", pathStr));

        return packageManagerInfo;
    }

    void RemoveDuplicatePackages(std::vector<std::string>& packageNames)
    {
        std::ranges::sort(packageNames);
        auto duplicates = std::ranges::unique(packageNames);
        packageNames.erase(std::ranges::begin(duplicates), std::ranges::end(duplicates));
    }

    void FilterPackages(PackageManagerActionType actionType, const AvailablePackages& availablePackages,
                        std::vector<std::string>& packageNames)
    {
        std::erase_if(packageNames, [&availablePackages, &actionType](std::string& packageName) {
            bool toErase = !availablePackages.contains(packageName);

            if (toErase)
            {
                Utils::PrintPretty(Utils::MessageSeverity::WARNING,
                                   "Package {} is not available for your package manager. "
                                   "It will not be {}ed",
                                   packageName, magic_enum::enum_name(actionType));
            }

            return toErase;
        });
    }

    int PerformPackageManagerAction(PackageManagerActionType actionType,
                                    const PackageManagerInfo& packageManager,
                                    const AvailablePackages& availablePackages,
                                    const std::vector<std::string>& packageNames)
    {
        std::string packagesStr;

        for (const std::string& packageName : packageNames)
            packagesStr += packageName + ' ';

        if (!magic_enum::enum_contains(actionType))
        {
            Utils::PrintPretty(Utils::MessageSeverity::ERROR, "Action {} does not exist",
                               (int) actionType);
            return -1;
        }

        Utils::PrintPretty(Utils::MessageSeverity::OK, "The following packages will be {}ed: {}",
                           magic_enum::enum_name(actionType), packagesStr);

        const PackageManagerAction& action = packageManager.actions[(size_t) actionType];

        if (action.batch)
        {
            packagesStr.clear();

            for (const std::string& packageName : packageNames)
                packagesStr += availablePackages.at(packageName) + ' ';

            return Utils::System(std::format("{} {} {}", packageManager.cmd, action.cmd, packagesStr));
        }
        else
        {
            int ret = 0;

            for (const std::string& packageName : packageNames)
            {
                ret += Utils::System(std::format("{} {} {}", packageManager.cmd, action.cmd,
                                                 availablePackages.at(packageName)));
            }

            return ret;
        }
    }
} // namespace ggpkg
