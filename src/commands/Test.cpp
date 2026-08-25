#include "commands/Commands.hpp"
#include "PackageManager.hpp"
#include "utils/Utils.hpp"

#include <cstdlib>

namespace ggpkg::Commands
{
    void Test()
    {
        auto res = ggpkg::GetPackageManager();
        Utils::Verify(res.has_value(), res.error());

        Utils::PrintPretty(Utils::MessageSeverity::OK, "Test completed successfully!");
        std::exit(EXIT_SUCCESS);
    }
} // namespace ggpkg::Commands
