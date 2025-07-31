#pragma once

#include <string>
#include <string_view>
#include <expected>

#include <ftxui/dom/node.hpp>
#include <ftxui/component/screen_interactive.hpp>

namespace ggpkg::Utils
{
    template<typename T>
    using Result = std::expected<T, std::string>;
    using Error = std::unexpected<std::string>;

    std::string GetEnv(const std::string_view& varName, const std::string_view& defaultValue = "");
    int SilentSystem(const std::string_view& command);
    int System(const std::string_view& command);

    ftxui::Element Banner();
    void DisableCursor(ftxui::ScreenInteractive& screen);
} // namespace ggpkg::Utils
