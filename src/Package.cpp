#include "Package.hpp"
#include "Config.hpp"
#include "utils/Utils.hpp"

#include <cpr/status_codes.h>
#include <glaze/json/read.hpp>
#include <cpr/callback.h>
#include <cpr/response.h>
#include <cpr/session.h>
#include <fstream>
#include <print>

namespace ggpkg
{
    Utils::Result<void> DownloadPackageDatabase()
    {
        std::print("Downloading latest ggpkg package database ... ");

        std::string buffer;

        cpr::Session session;
        session.SetUrl(cpr::Url{Config::PACKAGE_DATABASE_URL});
        buffer.reserve(session.GetDownloadFileLength());

        cpr::Response res = session.Download(
            cpr::WriteCallback{[&buffer](const std::string_view& data, intptr_t userData) {
                buffer.append(data);
                return true;
            }});

        if (res.status_code != cpr::status::HTTP_OK)
        {
            std::println("Failed!");
            return Utils::Error("An error occurred while downloading the package database");
        }

        std::ofstream outputFile(Config::PACKAGES_CONFIG_PATH);

        if (!outputFile)
        {
            std::println("Failed!");
            return Utils::Error("Failed to open packages database file to write packages");
        }

        outputFile.write(buffer.data(), buffer.length());
        std::println("Completed!");

        return Utils::Result<void>();
    }

    Utils::Result<Packages> GetPackages()
    {
        Packages packages;
        std::string buffer;

        const std::string pathStr = Config::PACKAGES_CONFIG_PATH.string();

        if (!std::filesystem::is_regular_file(Config::PACKAGES_CONFIG_PATH))
            return Utils::Error(std::format("Packages config file ({}) does not exist. "
                                            "You can download it with `ggpkg update-db`",
                                            pathStr));

        if (auto ec = glz::read_file_json(packages, pathStr, buffer); ec)
            return Utils::Error("Failed to read packages info from configuration");

        return packages;
    }
} // namespace ggpkg
