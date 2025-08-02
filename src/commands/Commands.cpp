#include "commands/Commands.hpp"
#include <memory>

namespace ggpkg
{
    void SetupCommands(CLI::App& app)
    {
        // clang-format off
        app.add_subcommand(
            "configure",
            "Detect compatible package manager automatically and save it in the config path"
        )->alias("autoconfig")->callback(&Commands::Configure);

        app.add_subcommand(
            "test",
            "Test package manager provided by the file in config path, if present"
        )->callback(&Commands::Test);

        auto listInteractive = std::make_shared<bool>(false);
        auto* listCmd = app.add_subcommand(
            "list",
            "List packages supported by ggpkg for your package manager"
        );
        listCmd->alias("ls");
        listCmd->add_flag("-i,--interactive", *listInteractive, "Enables interactive mode");
        listCmd->callback([listInteractive]{
            Commands::List(*listInteractive);
        });

        auto installPackages = std::make_shared<std::vector<std::string>>();
        auto* installCmd = app.add_subcommand(
            "install",
            "Install specified packages if they are available with your package manager"
        );
        installCmd->alias("add");
        installCmd->add_option("packages", *installPackages, "Packages to install");
        installCmd->callback([installPackages]{
            Commands::Install(*installPackages);
        });

        app.add_subcommand(
            "update",
            "Sync your package manager with remote repositories"
        )->callback(&Commands::Update);

        auto uninstallPackages = std::make_shared<std::vector<std::string>>();
        auto* uninstallCmd = app.add_subcommand(
            "uninstall",
            "Uninstall specified packages if they are available with your package manager"
        );
        uninstallCmd->alias("remove");
        uninstallCmd->add_option("packages", *uninstallPackages, "Packages to uninstall");
        uninstallCmd->callback([uninstallPackages]{
            Commands::Uninstall(*uninstallPackages);
        });
        // clang-format on
    }
} // namespace ggpkg
