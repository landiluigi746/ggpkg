#pragma once

#include "utils/Utils.hpp"

#include <filesystem>

namespace ggpkg::Config
{
#if defined(_WIN32)
    inline constexpr std::string_view CFG_PATH_ENV_VAR = "LOCALAPPDATA";
#elif defined(__linux__)
    inline constexpr std::string_view CFG_PATH_ENV_VAR = "HOME";
#else
#error "Unsupported platform"
#endif

    inline const std::filesystem::path HOME_PATH = Utils::GetEnv(CFG_PATH_ENV_VAR);

#if defined(_WIN32)
    inline const std::filesystem::path PACKAGE_MANAGER_CONFIG_PATH =
        HOME_PATH / "ggpkg" / "package-manager.json";
    inline const std::filesystem::path PACKAGES_CONFIG_PATH = HOME_PATH / "ggpkg" / "packages.json";
#elif defined(__linux__)
    inline const std::filesystem::path PACKAGE_MANAGER_CONFIG_PATH =
        HOME_PATH / ".config" / "ggpkg" / "package-manager.json";
    inline const std::filesystem::path PACKAGES_CONFIG_PATH =
        HOME_PATH / ".config" / "ggpkg" / "packages.json";
#else
#error "Unsupported platform"
#endif
} // namespace ggpkg::Config
