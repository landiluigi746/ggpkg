#include "Package.hpp"
#include "PackageManager.hpp"

#include "commands/Commands.hpp"

#include <CLI/CLI.hpp>
#include <cstdlib>
#include <exception>
#include <print>

int main(int argc, char* argv[])
{
    CLI::App app{"The package manager wrapper you never asked for 🚀", "ggpkg"};

    argv = app.ensure_utf8(argv);
    ggpkg::SetupCommands(app);

    CLI11_PARSE(app, argc, argv);

    std::println("{}", app.help());
    return EXIT_SUCCESS;
}
