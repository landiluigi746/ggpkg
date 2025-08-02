#include "commands/Commands.hpp"
#include "Package.hpp"
#include "PackageManager.hpp"
#include "utils/Utils.hpp"

#include <algorithm>
#include <cstdlib>
#include <unordered_set>

namespace ggpkg::Commands
{
    static int DefaultInstall(const PackageManagerInfo& packageManager,
                              const std::vector<std::string>& packageNames)
    {
        std::string packagesStr;

        for (const std::string& packageName : packageNames)
            packagesStr += packageName + ' ';

        Utils::PrintPretty(Utils::MessageSeverity::OK,
                           std::format("The following packages will be installed: {}", packagesStr));

        if (packageManager.installBatch)
        {
            return Utils::System(
                std::format("{} {} {}", packageManager.cmd, packageManager.install, packagesStr));
        }
        else
        {
            int ret = 0;

            for (const std::string& packageName : packageNames)
            {
                ret += Utils::System(
                    std::format("{} {} {}", packageManager.cmd, packageManager.install, packageName));
            }

            return ret;
        }
    }

    void Install(std::vector<std::string>& packageNames)
    {
        if (packageNames.empty())
        {
            Utils::PrintPretty(Utils::MessageSeverity::ERROR, "No packages specified");
            std::exit(EXIT_FAILURE);
        }

        std::ranges::sort(packageNames);
        auto duplicates = std::ranges::unique(packageNames);
        packageNames.erase(std::ranges::begin(duplicates), std::ranges::end(duplicates));

        auto packageManager = ggpkg::GetPackageManager();
        auto packages = ggpkg::GetPackages();

        if (!packageManager)
        {
            Utils::PrintPretty(Utils::MessageSeverity::ERROR, packageManager.error());
            std::exit(EXIT_FAILURE);
        }

        if (!packages)
        {
            Utils::PrintPretty(Utils::MessageSeverity::ERROR, packages.error());
            std::exit(EXIT_FAILURE);
        }

        AvailablePackages availablePackages =
            GetAvailablePackages(packages.value(), packageManager.value());

        std::unordered_set<std::string> supportedPackages;
        supportedPackages.reserve(std::size(availablePackages));

        for (const auto& [name, _] : availablePackages)
            supportedPackages.emplace(name);

        std::erase_if(packageNames, [&supportedPackages](const std::string& packageName) {
            bool toErase = !supportedPackages.contains(packageName);

            if (toErase)
            {
                Utils::PrintPretty(
                    Utils::MessageSeverity::WARNING,
                    std::format("Package {} is not recognized by ggpkg. It will not be installed",
                                packageName));
            }

            return toErase;
        });

        if (packageNames.empty())
        {
            Utils::PrintPretty(Utils::MessageSeverity::ERROR,
                               "None of the specified package can be installed");
            std::exit(EXIT_FAILURE);
        }

        if (DefaultInstall(packageManager.value(), packageNames))
            std::exit(EXIT_FAILURE);

        std::exit(EXIT_SUCCESS);
    }
} // namespace ggpkg::Commands
