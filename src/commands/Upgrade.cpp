#include "commands/Commands.hpp"
#include "Package.hpp"
#include "PackageManager.hpp"
#include "utils/Utils.hpp"

#include <algorithm>
#include <cstdlib>

namespace ggpkg::Commands
{
    void Upgrade(std::vector<std::string>& packageNames)
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

        if (packageManager->upgrade.empty())
        {
            Utils::PrintPretty(Utils::MessageSeverity::ERROR,
                               "Your package manager ({}) does not support package-specific upgrades",
                               packageManager->cmd);
            std::exit(EXIT_FAILURE);
        }

        if (!packages)
        {
            Utils::PrintPretty(Utils::MessageSeverity::ERROR, packages.error());
            std::exit(EXIT_FAILURE);
        }

        AvailablePackages availablePackages =
            GetAvailablePackages(packages.value(), packageManager.value());

        std::erase_if(packageNames, [&availablePackages](std::string& packageName) {
            bool toErase = !availablePackages.contains(packageName);

            if (toErase)
            {
                Utils::PrintPretty(Utils::MessageSeverity::WARNING,
                                   "Package {} is not available for your package manager. "
                                   "It will not be upgraded",
                                   packageName);
            }

            return toErase;
        });

        if (packageNames.empty())
        {
            Utils::PrintPretty(Utils::MessageSeverity::ERROR,
                               "None of the specified packages can be upgraded");
            std::exit(EXIT_FAILURE);
        }

        if (UpgradePackages(packageManager.value(), availablePackages, packageNames))
            std::exit(EXIT_FAILURE);

        std::exit(EXIT_SUCCESS);
    }
} // namespace ggpkg::Commands
