#include "commands/Commands.hpp"
#include "PackageManager.hpp"
#include "utils/Utils.hpp"

#include <cstdlib>

namespace ggpkg::Commands
{
    void Test()
    {
        if (auto ec = ggpkg::GetPackageManager(); !ec)
        {
            Utils::PrintPretty(Utils::MessageSeverity::ERROR, ec.error());
            std::exit(EXIT_FAILURE);
        }

        Utils::PrintPretty(Utils::MessageSeverity::OK, "Test completed successfully!");
        std::exit(EXIT_SUCCESS);
    }
} // namespace ggpkg::Commands
