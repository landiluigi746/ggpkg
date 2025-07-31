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

        auto listInteractive = std::make_shared<bool>(false);
        auto* listCmd = app.add_subcommand(
            "list",
            "List packages supported by ggpkg for your package manager"
        );
        listCmd->add_flag("-i", *listInteractive, "Enables interactive mode");
        listCmd->callback([listInteractive]{
            Commands::List(*listInteractive);
        });

        auto installPackages = std::make_shared<std::vector<std::string>>();
        auto* installCmd = app.add_subcommand(
            "install",
            "Install specified packages if they are available with your package manager"
        );
        installCmd->add_option("packages", *installPackages, "Packages to install");
        installCmd->callback([installPackages]{
            Commands::Install(*installPackages);
        });
        // clang-format on
    }
} // namespace ggpkg
