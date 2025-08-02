#include "utils/Utils.hpp"

#include <cstdlib>
#include <iostream>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <print>

namespace ggpkg::Utils
{
    std::string GetEnv(const std::string_view& varName, const std::string_view& defaultValue)
    {
        const char* val = std::getenv(varName.data());
        return (val && val[0] != '\0') ? std::string(val) : std::string(defaultValue);
    }

    int SilentSystem(const std::string_view& command)
    {
#if defined(_WIN32)
        return std::system(std::format("{} > NUL 2>&1", command).c_str());
#elif defined(__linux__)
        return std::system(std::format("{} > /dev/null 2>&1", command).c_str());
#endif
    }

    int System(const std::string_view& command)
    {
        std::cout << std::flush;
        return std::system(command.data());
    }

    ftxui::Element Banner()
    {
        // clang-format off
        return ftxui::text("ggpkg") | ftxui::hcenter | ftxui::bold | ftxui::color(ftxui::Color::DarkOrange);
        // clang-format on
    }

    void DisableCursor(ftxui::ScreenInteractive& screen)
    {
        screen.SetCursor(ftxui::Screen::Cursor{
            .shape = ftxui::Screen::Cursor::Shape::Hidden,
        });
    }

    void PrintPretty(MessageSeverity severity, const std::string_view& message)
    {
        ftxui::Color color;
        std::string_view prefix;

        switch (severity)
        {
            case MessageSeverity::OK:
                color = ftxui::Color::Green;
                prefix = "OK: ";
                break;
            case MessageSeverity::WARNING:
                color = ftxui::Color::Yellow2;
                prefix = "WARNING: ";
                break;
            case MessageSeverity::ERROR:
                color = ftxui::Color::Red1;
                prefix = "ERROR: ";
                break;
            default:
                color = ftxui::Color::GrayLight;
                prefix = "UNKNOWN: ";
                break;
        }

        auto document = ftxui::hbox({
            ftxui::text(prefix.data()) | ftxui::color(color),
            ftxui::paragraph(message.data()),
        });
        auto screen = ftxui::Screen::Create(ftxui::Dimension::Fit(document));

        ftxui::Render(screen, document);

        std::println("{}", screen.ToString());
    }
} // namespace ggpkg::Utils
