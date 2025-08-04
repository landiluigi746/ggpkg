#include "commands/Commands.hpp"
#include "Package.hpp"
#include "utils/Utils.hpp"

#include <cstdlib>

namespace ggpkg::Commands
{
    void UpdatePackages()
    {
        if (auto ec = ggpkg::DownloadPackageDatabase(); !ec)
        {
            Utils::PrintPretty(Utils::MessageSeverity::ERROR, ec.error());
            std::exit(EXIT_FAILURE);
        }

        Utils::PrintPretty(Utils::MessageSeverity::OK,
                           "Package database update completed successfully!");
        std::exit(EXIT_SUCCESS);
    }
} // namespace ggpkg::Commands
