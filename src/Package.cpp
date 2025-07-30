#include "Package.hpp"
#include "Config.hpp"

#include <glaze/json/read.hpp>

namespace ggpkg
{
    Utils::Result<Packages> GetPackages()
    {
        Packages packages;
        std::string buffer;

        const std::string pathStr = Config::PACKAGES_CONFIG_PATH.string();

        if (!std::filesystem::is_regular_file(Config::PACKAGES_CONFIG_PATH))
            return Utils::Error(std::format(
                "Packages config file does ({}) not exist. "
                "You can download it from "
                "https://raw.githubusercontent.com/landiluigi746/ggpkg/refs/heads/master/packages.json",
                pathStr));

        if (auto ec = glz::read_file_json(packages, pathStr, buffer); ec)
            return Utils::Error("Failed to read packages info from configuration");

        return packages;
    }
} // namespace ggpkg
