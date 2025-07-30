#include "commands/Commands.hpp"
#include "PackageManager.hpp"

#include <print>
#include <cstdlib>

namespace ggpkg::Commands
{
    void Configure()
    {
        if (auto ec = ggpkg::DetectPackageManager(); !ec)
        {
            std::println("ERROR: {}", ec.error());
            std::exit(EXIT_FAILURE);
        }

        std::println("Configuration completed successfully!");
        std::exit(EXIT_SUCCESS);
    }
} // namespace ggpkg::Commands