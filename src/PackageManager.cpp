#include "PackageManager.hpp"
#include "Config.hpp"
#include "utils/Utils.hpp"

#include <atomic>
#include <filesystem>
#include <print>
#include <system_error>
#include <glaze/json/read.hpp>
#include <glaze/json/write.hpp>

namespace ggpkg
{
    static std::atomic_bool s_Initialized = false;
    static PackageManagerInfo s_PackageManagerInfo;

    static bool TestPackageManager(const PackageManagerInfo& packageManager)
    {
        std::print("Testing {} ... ", packageManager.cmd);

        if (Utils::SilentSystem(std::format("{} {}", packageManager.cmd, packageManager.version)))
        {
            std::println("Failed");
            return false;
        }

        std::println("Passed !");
        return true;
    }

    static Utils::Result<void> SavePackageManager(const PackageManagerInfo& packageManager)
    {
        std::string buffer;

        std::error_code dirEc;
        std::filesystem::create_directories(Config::PACKAGE_MANAGER_CONFIG_PATH.parent_path(), dirEc);

        if (dirEc)
            return Utils::Error(dirEc.message());

        if (auto ec = glz::write_file_json(packageManager, Config::PACKAGE_MANAGER_CONFIG_PATH.string(),
                                           buffer);
            ec)
        {
            return Utils::Error("Failed to write package manager configuration");
        }

        return Utils::Result<void>();
    }

    Utils::Result<void> DetectPackageManager()
    {
        std::string errorStr;
        auto options = std::to_array<PackageManagerInfo>({
            PackageManagerInfo{.cmd = "pacman", .version = "-V", .install = "-S"},
        });

        for (const auto& option : options)
        {
            if (!TestPackageManager(option))
                continue;

            return SavePackageManager(option);
        }

        return Utils::Error("None of the supported package managers is available");
    }

    Utils::Result<PackageManagerInfo> GetPackageManager()
    {
        if (s_Initialized)
            return s_PackageManagerInfo;

        PackageManagerInfo packageManagerInfo;
        const std::string pathStr = Config::PACKAGE_MANAGER_CONFIG_PATH.string();

        if (!std::filesystem::is_regular_file(Config::PACKAGE_MANAGER_CONFIG_PATH))
            return Utils::Error(std::format("Package manager config file does ({}) not exist. "
                                            "Configure it automatically using `ggpkg configure`",
                                            pathStr));

        std::string buffer;
        if (auto ec = glz::read_file_json(packageManagerInfo, pathStr, buffer); ec)
            return Utils::Error("Failed to read package manager info from configuration");

        if (auto ec = TestPackageManager(packageManagerInfo); !ec)
            return Utils::Error(std::format(
                "The package manager in your configuration file ({}) did not pass the test", pathStr));

        s_PackageManagerInfo = packageManagerInfo;
        s_Initialized = true;
        return s_PackageManagerInfo;
    }
} // namespace ggpkg
