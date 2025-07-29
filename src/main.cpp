#include "Package.hpp"
#include "PackageManager.hpp"

#include <argparse/argparse.hpp>
#include <cstdlib>
#include <exception>
#include <print>

int main(const int argc, const char* argv[])
{
    argparse::ArgumentParser program("ggpkg", "0.1", argparse::default_arguments::all);
    program.add_description("The package manager wrapper you never asked for 🔥");

    argparse::ArgumentParser configure("configure");
    configure.add_description(
        "Detects compatible package manager automatically and saves it in the config path");

    argparse::ArgumentParser test("test");
    test.add_description("Test package manager provided by the file in config path, if present");

    argparse::ArgumentParser list("list");
    list.add_description("List packages supported by ggpkg for your package manager");

    program.add_subparser(configure);
    program.add_subparser(test);
    program.add_subparser(list);

    try
    {
        program.parse_args(argc, argv);
    }
    catch (const std::exception& err)
    {
        std::println("{}", program.help().str());
        return EXIT_FAILURE;
    }

    if (program.is_subcommand_used("configure"))
    {
        if (auto ec = ggpkg::DetectPackageManager(); !ec)
        {
            std::println("ERROR: {}", ec.error());
            return EXIT_FAILURE;
        }

        std::println("Configuration completed successfully!");
        return EXIT_SUCCESS;
    }

    if (program.is_subcommand_used("test"))
    {
        if (auto ec = ggpkg::GetPackageManager(); !ec)
        {
            std::println("ERROR: {}", ec.error());
            return EXIT_FAILURE;
        }

        std::println("Test completed successfully!");
        return EXIT_SUCCESS;
    }

    if (program.is_subcommand_used("list"))
    {
        auto packageManager = ggpkg::GetPackageManager();
        auto packages = ggpkg::GetPackages();

        if (!packageManager)
        {
            std::println("ERROR: {}", packageManager.error());
            return EXIT_FAILURE;
        }

        if (!packages)
        {
            std::println("ERROR: {}", packages.error());
            return EXIT_FAILURE;
        }

        for (const auto& [name, providers] : packages.value())
        {
            if (providers.contains(packageManager.value().cmd))
                std::println("{}", name);
        }

        return EXIT_SUCCESS;
    }

    std::println("{}", program.help().str());
    return EXIT_SUCCESS;
}
