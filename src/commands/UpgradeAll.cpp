#include "commands/Commands.hpp"
#include "PackageManager.hpp"
#include "utils/Utils.hpp"

#include <cstdlib>

namespace ggpkg::Commands
{
    void UpgradeAll()
    {
        auto packageManager = ggpkg::GetPackageManager();
        Utils::Verify(packageManager.has_value(), packageManager.error());

        if (Utils::System(std::format("{} {}", packageManager->cmd, packageManager->upgradeAll)))
            std::exit(EXIT_FAILURE);

        std::exit(EXIT_SUCCESS);
    }
} // namespace ggpkg::Commands
