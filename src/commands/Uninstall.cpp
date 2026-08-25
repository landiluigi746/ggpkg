#include "commands/Commands.hpp"
#include "Package.hpp"
#include "PackageManager.hpp"
#include "utils/Utils.hpp"

#include <cstdlib>

namespace ggpkg::Commands
{
    void Uninstall(std::vector<std::string>& packageNames)
    {
        Utils::Verify(!packageNames.empty(), "No packages specified");

        RemoveDuplicatePackages(packageNames);

        auto packageManager = ggpkg::GetPackageManager();
        auto packages = ggpkg::GetPackages();

        Utils::Verify(packageManager.has_value(), packageManager.error());
        Utils::Verify(packages.has_value(), packages.error());

        AvailablePackages availablePackages =
            GetAvailablePackages(packages.value(), packageManager.value());

        FilterPackages(PackageManagerActionType::Uninstall, availablePackages, packageNames);

        Utils::Verify(!packageNames.empty(), "None of the specified packages can be installed");

        if (PerformPackageManagerAction(PackageManagerActionType::Uninstall, packageManager.value(),
                                        availablePackages, packageNames))
            std::exit(EXIT_FAILURE);

        std::exit(EXIT_SUCCESS);
    }
} // namespace ggpkg::Commands
