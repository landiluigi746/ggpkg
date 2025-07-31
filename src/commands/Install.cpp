#include "commands/Commands.hpp"
#include "Package.hpp"
#include "PackageManager.hpp"

#include <cstdlib>
#include <iterator>
#include <print>
#include <ranges>
#include <unordered_set>

namespace ggpkg::Commands
{
    void Install(std::vector<std::string>& packageNames)
    {
        if (packageNames.empty())
        {
            std::println("ERROR: No packages specified");
            std::exit(EXIT_FAILURE);
        }

        auto packageManager = ggpkg::GetPackageManager();
        auto packages = ggpkg::GetPackages();

        if (!packageManager)
        {
            std::println("ERROR: {}", packageManager.error());
            std::exit(EXIT_FAILURE);
        }

        if (!packages)
        {
            std::println("ERROR: {}", packages.error());
            std::exit(EXIT_FAILURE);
        }

        auto availablePackages = ggpkg::GetAvailablePackages(packages.value(), packageManager.value());

        std::unordered_set<std::string> availablePackageNames;
        availablePackageNames.reserve(std::ranges::distance(availablePackages));

        for (const auto& [name, _] : availablePackages)
            availablePackageNames.emplace(name);

        auto packagesToInstall =
            packageNames | std::views::filter([&availablePackageNames](const std::string& name) {
                bool toInstall = availablePackageNames.contains(name);

                if (!toInstall)
                    std::println("WARNING: Package {} was not found, it will not be installed", name);

                return toInstall;
            });

        std::println("The following packages will be installed: {}", packagesToInstall);
        std::exit(EXIT_SUCCESS);
    }
} // namespace ggpkg::Commands
