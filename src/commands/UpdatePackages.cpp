#include "PackageManager.hpp"
#include "commands/Commands.hpp"
#include "Package.hpp"
#include "utils/Utils.hpp"

#include <cstdlib>

namespace ggpkg::Commands
{
    void UpdatePackages()
    {
        auto packageManager = ggpkg::GetPackageManager();
        Utils::Verify(packageManager.has_value(), packageManager.error());

        auto res = ggpkg::DownloadPackageDatabase(packageManager.value().cmd);
        Utils::Verify(res.has_value(), res.error());

        Utils::PrintPretty(Utils::MessageSeverity::OK,
                           "Package database update completed successfully!");
        std::exit(EXIT_SUCCESS);
    }
} // namespace ggpkg::Commands
