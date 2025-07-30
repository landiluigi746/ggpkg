#include "commands/Commands.hpp"
#include "PackageManager.hpp"

#include <print>
#include <cstdlib>

namespace ggpkg::Commands
{
    void Test()
    {
        if (auto ec = ggpkg::GetPackageManager(); !ec)
        {
            std::println("ERROR: {}", ec.error());
            std::exit(EXIT_FAILURE);
        }

        std::println("Test completed successfully!");
        std::exit(EXIT_SUCCESS);
    }
} // namespace ggpkg::Commands
