#pragma once

#include "utils/Utils.hpp"
#include "utils/StringHash.hpp"

#include <string>
#include <unordered_map>

namespace ggpkg
{
    using Packages = std::unordered_map<std::string, std::string, Utils::StringHash, std::equal_to<>>;

    Utils::Result<void> DownloadPackageDatabase(std::string_view packageManager);
    Utils::Result<Packages> GetPackages();
} // namespace ggpkg
