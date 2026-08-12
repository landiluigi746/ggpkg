#include "commands/Commands.hpp"
#include "Package.hpp"
#include "PackageManager.hpp"
#include "utils/Utils.hpp"

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

        RemoveDuplicatePackages(packageNames);

        auto packageManager = ggpkg::GetPackageManager();
        auto packages = ggpkg::GetPackages();

        if (!packageManager)
        {
            Utils::PrintPretty(Utils::MessageSeverity::ERROR, packageManager.error());
            std::exit(EXIT_FAILURE);
        }

        if (packageManager->actions[(size_t) PackageManagerActionType::Upgrade].cmd.empty())
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

        FilterPackages(PackageManagerActionType::Upgrade, availablePackages, packageNames);

        if (packageNames.empty())
        {
            Utils::PrintPretty(Utils::MessageSeverity::ERROR,
                               "None of the specified packages can be upgraded");
            std::exit(EXIT_FAILURE);
        }

        if (PerformPackageManagerAction(PackageManagerActionType::Upgrade, packageManager.value(),
                                        availablePackages, packageNames))
            std::exit(EXIT_FAILURE);

        std::exit(EXIT_SUCCESS);
    }
} // namespace ggpkg::Commands
