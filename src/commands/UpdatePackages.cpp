#include "commands/Commands.hpp"
#include "Package.hpp"
#include "utils/Utils.hpp"

#include <cstdlib>

namespace ggpkg::Commands
{
    void UpdatePackages()
    {
        auto res = ggpkg::DownloadPackageDatabase();
        Utils::Verify(res.has_value(), res.error());

        Utils::PrintPretty(Utils::MessageSeverity::OK,
                           "Package database update completed successfully!");
        std::exit(EXIT_SUCCESS);
    }
} // namespace ggpkg::Commands
