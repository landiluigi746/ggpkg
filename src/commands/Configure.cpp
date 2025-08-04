#include "Package.hpp"
#include "commands/Commands.hpp"
#include "PackageManager.hpp"
#include "utils/Utils.hpp"

#include <cstdlib>

namespace ggpkg::Commands
{
    void Configure()
    {
        if (auto ec = ggpkg::DetectPackageManager(); !ec)
        {
            Utils::PrintPretty(Utils::MessageSeverity::ERROR, ec.error());
            std::exit(EXIT_FAILURE);
        }

        if (auto ec = ggpkg::DownloadPackageDatabase(); !ec)
        {
            Utils::PrintPretty(Utils::MessageSeverity::ERROR, ec.error());
            std::exit(EXIT_FAILURE);
        }

        Utils::PrintPretty(Utils::MessageSeverity::OK, "Configuration completed successfully!");
        std::exit(EXIT_SUCCESS);
    }
} // namespace ggpkg::Commands
