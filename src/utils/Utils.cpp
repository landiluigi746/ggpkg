#include "utils/Utils.hpp"

#include <cstdlib>
#include <iostream>

namespace ggpkg::Utils
{
    std::string GetEnv(std::string_view varName, std::string_view defaultValue)
    {
        const char* val = std::getenv(varName.data());
        return (val && val[0] != '\0') ? std::string(val) : std::string(defaultValue);
    }

    int SilentSystem(std::string_view command)
    {
#if defined(_WIN32)
        return std::system(std::format("{} > NUL 2>&1", command).c_str());
#elif defined(__linux__)
        return std::system(std::format("{} > /dev/null 2>&1", command).c_str());
#endif
    }

    int System(std::string_view command)
    {
        std::cout << std::flush;
        return std::system(command.data());
    }
} // namespace ggpkg::Utils
