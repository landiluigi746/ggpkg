#include "commands/Commands.hpp"
#include "PackageManager.hpp"
#include "Package.hpp"

#include <print>
#include <cstdlib>

namespace ggpkg::Commands
{
    void List()
    {
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

        for (const auto& [name, providers] : packages.value())
        {
            if (providers.contains(packageManager.value().cmd))
                std::println("{}", name);
        }

        std::exit(EXIT_SUCCESS);
    }
} // namespace ggpkg::Commands