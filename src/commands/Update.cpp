#include "commands/Commands.hpp"
#include "PackageManager.hpp"
#include "utils/Utils.hpp"

#include <cstdlib>

namespace ggpkg::Commands
{
    static int DefaultUpdate(const PackageManagerInfo& packageManager)
    {
        return Utils::System(std::format("{} {}", packageManager.cmd, packageManager.update));
    }

    void Update()
    {
        auto packageManager = ggpkg::GetPackageManager();

        if (!packageManager)
        {
            Utils::PrintPretty(Utils::MessageSeverity::ERROR, packageManager.error());
            std::exit(EXIT_FAILURE);
        }

        if (DefaultUpdate(packageManager.value()))
            std::exit(EXIT_FAILURE);

        std::exit(EXIT_SUCCESS);
    }
} // namespace ggpkg::Commands
