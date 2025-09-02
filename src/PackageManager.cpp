#include "PackageManager.hpp"
#include "Config.hpp"
#include "utils/Utils.hpp"

#include <filesystem>
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
        std::string errorStr;

        auto options = std::to_array<PackageManagerInfo>({
#if defined(_WIN32)
            PackageManagerInfo{
                .cmd = "winget",
                .version = "-v",
                .install = "install -e --id",
                .uninstall = "uninstall",
                .update = "source update",
                .upgradeAll = "upgrade --all",
                .upgrade = "upgrade -e --id",
                .installBatch = false,
                .uninstallBatch = false,
                .upgradeBatch = false,
            },
#elif defined(__linux__)
            PackageManagerInfo{
                .cmd = "pacman",
                .version = "-V",
                .install = "-S",
                .uninstall = "-R",
                .update = "-Sy",
                .upgradeAll = "-Syu",
                .upgrade = "",
                .installBatch = true,
                .uninstallBatch = true,
                .upgradeBatch = false,
            },
            PackageManagerInfo{
                .cmd = "apt",
                .version = "-v",
                .install = "install",
                .uninstall = "remove",
                .update = "update",
                .upgradeAll = "upgrade",
                .upgrade = "install --only-upgrade",
                .installBatch = true,
                .uninstallBatch = true,
                .upgradeBatch = true,
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

    int InstallPackages(const PackageManagerInfo& packageManager,
                        const AvailablePackages& availablePackages,
                        const std::vector<std::string>& packageNames)
    {
        std::string packagesStr;

        for (const std::string& packageName : packageNames)
            packagesStr += packageName + ' ';

        Utils::PrintPretty(Utils::MessageSeverity::OK,
                           std::format("The following packages will be installed: {}", packagesStr));

        if (packageManager.installBatch)
        {
            packagesStr.clear();

            for (const std::string& packageName : packageNames)
                packagesStr += availablePackages.at(packageName) + ' ';

            return Utils::System(
                std::format("{} {} {}", packageManager.cmd, packageManager.install, packagesStr));
        }
        else
        {
            int ret = 0;

            for (const std::string& packageName : packageNames)
            {
                ret += Utils::System(std::format("{} {} {}", packageManager.cmd, packageManager.install,
                                                 availablePackages.at(packageName)));
            }

            return ret;
        }
    }

    int UninstallPackages(const PackageManagerInfo& packageManager,
                          const AvailablePackages& availablePackages,
                          const std::vector<std::string>& packageNames)
    {
        std::string packagesStr;

        for (const std::string& packageName : packageNames)
            packagesStr += packageName + ' ';

        Utils::PrintPretty(Utils::MessageSeverity::OK,
                           std::format("The following packages will be uninstalled: {}", packagesStr));

        if (packageManager.uninstallBatch)
        {
            packagesStr.clear();

            for (const std::string& packageName : packageNames)
                packagesStr += availablePackages.at(packageName) + ' ';

            return Utils::System(
                std::format("{} {} {}", packageManager.cmd, packageManager.uninstall, packagesStr));
        }
        else
        {
            int ret = 0;

            for (const std::string& packageName : packageNames)
            {
                ret +=
                    Utils::System(std::format("{} {} {}", packageManager.cmd, packageManager.uninstall,
                                              availablePackages.at(packageName)));
            }

            return ret;
        }
    }

    int UpgradePackages(const PackageManagerInfo& packageManager,
                        const AvailablePackages& availablePackages,
                        const std::vector<std::string>& packageNames)
    {
        std::string packagesStr;

        for (const std::string& packageName : packageNames)
            packagesStr += packageName + ' ';

        Utils::PrintPretty(Utils::MessageSeverity::OK,
                           std::format("The following packages will be upgraded: {}", packagesStr));

        if (packageManager.upgradeBatch)
        {
            packagesStr.clear();

            for (const std::string& packageName : packageNames)
                packagesStr += availablePackages.at(packageName) + ' ';

            return Utils::System(
                std::format("{} {} {}", packageManager.cmd, packageManager.upgrade, packagesStr));
        }
        else
        {
            int ret = 0;

            for (const std::string& packageName : packageNames)
            {
                ret += Utils::System(std::format("{} {} {}", packageManager.cmd, packageManager.upgrade,
                                                 availablePackages.at(packageName)));
            }

            return ret;
        }
    }
} // namespace ggpkg
