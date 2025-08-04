#pragma once

#include "utils/Utils.hpp"

#include <filesystem>

namespace ggpkg::Config
{
#if defined(_WIN32)
    inline const std::filesystem::path BASE_CONFIG_PATH = Utils::GetEnv("LOCALAPPDATA");
#elif defined(__linux__)
    inline const std::string SUDO_USER = Utils::GetEnv("SUDO_USER");
    inline const std::filesystem::path BASE_CONFIG_PATH =
        (SUDO_USER.empty()) ? std::filesystem::path(Utils::GetEnv("HOME"))
                            : std::filesystem::path("/home") / SUDO_USER;
#else
#error "Unsupported platform"
#endif

#if defined(_WIN32)
    inline const std::filesystem::path PACKAGE_MANAGER_CONFIG_PATH =
        BASE_CONFIG_PATH / "ggpkg" / "package-manager.json";
    inline const std::filesystem::path PACKAGES_CONFIG_PATH =
        BASE_CONFIG_PATH / "ggpkg" / "packages.json";
#elif defined(__linux__)
    inline const std::filesystem::path PACKAGE_MANAGER_CONFIG_PATH =
        BASE_CONFIG_PATH / ".config" / "ggpkg" / "package-manager.json";
    inline const std::filesystem::path PACKAGES_CONFIG_PATH =
        BASE_CONFIG_PATH / ".config" / "ggpkg" / "packages.json";
#else
#error "Unsupported platform"
#endif

    inline const std::string PACKAGE_DATABASE_URL =
        "https://raw.githubusercontent.com/landiluigi746/ggpkg/refs/heads/master/packages.json";
} // namespace ggpkg::Config
