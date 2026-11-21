#include "commands/Commands.hpp"
#include "Package.hpp"
#include "PackageManager.hpp"
#include "utils/Utils.hpp"

#include <cstdlib>

namespace ggpkg::Commands
{
    void Configure()
    {
        auto packageManager = ggpkg::DetectPackageManager();
        Utils::Verify(packageManager.has_value(), packageManager.error());

        auto res = ggpkg::DownloadPackageDatabase(packageManager.value().cmd);
        Utils::Verify(res.has_value(), res.error());

        Utils::PrintPretty(Utils::MessageSeverity::OK, "Configuration completed successfully!");
        std::exit(EXIT_SUCCESS);
    }
} // namespace ggpkg::Commands
