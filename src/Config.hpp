#pragma once

#include "utils/Utils.hpp"

#include <filesystem>

namespace ggpkg::Config
{
    inline const std::filesystem::path HOME_PATH = Utils::GetEnv("HOME");
    inline const std::filesystem::path PACKAGE_MANAGER_CONFIG_PATH =
        HOME_PATH / ".config" / "ggpkg" / "package-manager.json";
} // namespace ggpkg::Config
