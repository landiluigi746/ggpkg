#pragma once

#include <string>
#include <string_view>
#include <expected>

namespace ggpkg::Utils
{
    template<typename T>
    using Result = std::expected<T, std::string>;
    using Error = std::unexpected<std::string>;

    std::string GetEnv(std::string_view varName, std::string_view defaultValue = "");
    int SilentSystem(std::string_view command);
    int System(std::string_view command);
} // namespace ggpkg::Utils
