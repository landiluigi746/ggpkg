#include "commands/Commands.hpp"
#include "Package.hpp"
#include "PackageManager.hpp"
#include "utils/Utils.hpp"

#include <cstdlib>
#include <ranges>
#include <unordered_set>

namespace ggpkg::Commands
{
    static int DefaultInstall(const PackageManagerInfo& packageManager, auto& packageNames)
    {
        std::string packagesStr;

        for (const std::string& packageName : packageNames)
            packagesStr += packageName + ' ';

        Utils::PrintPretty(Utils::MessageSeverity::OK,
                           std::format("The following packages will be installed: {}", packagesStr));

        return Utils::System(
            std::format("{} {} {}", packageManager.cmd, packageManager.install, packagesStr));
    }

    void Install(const std::vector<std::string>& packageNames)
    {
        if (packageNames.empty())
        {
            Utils::PrintPretty(Utils::MessageSeverity::ERROR, "No packages specified");
            std::exit(EXIT_FAILURE);
        }

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

        auto availablePackages = ggpkg::GetAvailablePackages(packages.value(), packageManager.value());

        std::unordered_set<std::string> availablePackageNames;
        availablePackageNames.reserve(std::ranges::distance(availablePackages));

        for (const auto& [name, _] : availablePackages)
            availablePackageNames.emplace(name);

        auto packagesToInstall =
            packageNames | std::views::filter([&availablePackageNames](const std::string& name) {
                bool toInstall = availablePackageNames.contains(name);

                if (!toInstall)
                {
                    Utils::PrintPretty(
                        Utils::MessageSeverity::WARNING,
                        std::format("Package {} was not found, it will not be installed", name));
                }

                return toInstall;
            });

        if (std::ranges::distance(packagesToInstall) == 0)
        {
            Utils::PrintPretty(Utils::MessageSeverity::ERROR,
                               "None of the specified package can be installed");
            std::exit(EXIT_FAILURE);
        }

        if (DefaultInstall(packageManager.value(), packagesToInstall))
            std::exit(EXIT_FAILURE);

        std::exit(EXIT_SUCCESS);
    }
} // namespace ggpkg::Commands
