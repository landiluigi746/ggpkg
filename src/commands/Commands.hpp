#pragma once

#include <string>
#include <functional>
#include <array>
#include <CLI/App.hpp>

namespace ggpkg
{
    void SetupCommands(CLI::App& app);

    namespace Commands
    {
        [[noreturn]] void Configure();
        [[noreturn]] void Test();
        [[noreturn]] void List();
    } // namespace Commands
} // namespace ggpkg