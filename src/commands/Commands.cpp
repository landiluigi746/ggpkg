#include "commands/Commands.hpp"

namespace ggpkg
{
    void SetupCommands(CLI::App& app)
    {
        // clang-format off
        app.add_subcommand(
            "configure",
            "Detect compatible package manager automatically and save it in the config path"
        )->callback(&Commands::Configure);

        app.add_subcommand(
            "test",
            "Test package manager provided by the file in config path, if present"
        )->callback(&Commands::Test);

        app.add_subcommand(
            "list",
            "List packages supported by ggpkg for your package manager"
        )->callback(&Commands::List);
        // clang-format on
    }
} // namespace ggpkg