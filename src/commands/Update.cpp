#include "commands/Commands.hpp"
#include "PackageManager.hpp"
#include "utils/Utils.hpp"

#include <cstdlib>

namespace ggpkg::Commands
{
    void Update()
    {
        auto packageManager = ggpkg::GetPackageManager();

        Utils::Verify(packageManager.has_value(), packageManager.error());

        if (Utils::System(std::format("{} {}", packageManager->cmd, packageManager->update)))
            std::exit(EXIT_FAILURE);

        std::exit(EXIT_SUCCESS);
    }
} // namespace ggpkg::Commands
