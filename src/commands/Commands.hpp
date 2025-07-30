#pragma once

#include <CLI/App.hpp>

namespace ggpkg
{
    void SetupCommands(CLI::App& app);

    namespace Commands
    {
        [[noreturn]] void Configure();
        [[noreturn]] void Test();
        [[noreturn]] void List(bool interactive);
    } // namespace Commands
} // namespace ggpkg
