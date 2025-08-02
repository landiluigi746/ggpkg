#include "commands/Commands.hpp"
#include "Package.hpp"
#include "PackageManager.hpp"
#include "utils/Utils.hpp"

#include <algorithm>
#include <cstdlib>
#include <unordered_set>

namespace ggpkg::Commands
{
    static int DefaultUninstall(const PackageManagerInfo& packageManager,
                                const std::vector<std::string>& packageNames)
    {
        std::string packagesStr;

        for (const std::string& packageName : packageNames)
            packagesStr += packageName + ' ';

        Utils::PrintPretty(Utils::MessageSeverity::OK,
                           std::format("The following packages will be uninstalled: {}", packagesStr));

        if (packageManager.uninstallBatch)
        {
            return Utils::System(
                std::format("{} {} {}", packageManager.cmd, packageManager.uninstall, packagesStr));
        }
        else
        {
            int ret = 0;

            for (const std::string& packageName : packageNames)
            {
                ret += Utils::System(
                    std::format("{} {} {}", packageManager.cmd, packageManager.uninstall, packageName));
            }

            return ret;
        }
    }

    void Uninstall(std::vector<std::string>& packageNames)
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
                Utils::PrintPretty(Utils::MessageSeverity::WARNING,
                                   std::format("Package {} is not available for your package manager. "
                                               "It will not be uninstalled",
                                               packageName));
            }

            return toErase;
        });

        if (packageNames.empty())
        {
            Utils::PrintPretty(Utils::MessageSeverity::ERROR,
                               "None of the specified packages can be uninstalled");
            std::exit(EXIT_FAILURE);
        }

        if (DefaultUninstall(packageManager.value(), packageNames))
            std::exit(EXIT_FAILURE);

        std::exit(EXIT_SUCCESS);
    }
} // namespace ggpkg::Commands
