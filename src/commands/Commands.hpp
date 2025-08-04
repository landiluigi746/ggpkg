#pragma once

#include <CLI/App.hpp>
#include <vector>
#include <string>

namespace ggpkg
{
    void SetupCommands(CLI::App& app);

    namespace Commands
    {
        [[noreturn]] void Configure();
        [[noreturn]] void Test();
        [[noreturn]] void UpdatePackages();
        [[noreturn]] void List(bool interactive);
        [[noreturn]] void Install(std::vector<std::string>& packageNames);
        [[noreturn]] void Update();
        [[noreturn]] void Uninstall(std::vector<std::string>& packageNames);
    } // namespace Commands
} // namespace ggpkg
