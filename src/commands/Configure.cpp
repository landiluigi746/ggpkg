#include "commands/Commands.hpp"
#include "Package.hpp"
#include "PackageManager.hpp"
#include "utils/Utils.hpp"

#include <cstdlib>

namespace ggpkg::Commands
{
    void Configure()
    {
        auto res = ggpkg::DetectPackageManager();
        Utils::Verify(res.has_value(), res.error());

        res = ggpkg::DownloadPackageDatabase();
        Utils::Verify(res.has_value(), res.error());

        Utils::PrintPretty(Utils::MessageSeverity::OK, "Configuration completed successfully!");
        std::exit(EXIT_SUCCESS);
    }
} // namespace ggpkg::Commands
