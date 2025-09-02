#pragma once

#include "utils/Utils.hpp"

#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <print>

namespace ggpkg::Utils
{
    template<typename... Args>
    inline void PrintPretty(MessageSeverity severity, const std::format_string<Args...>& formatString,
                            Args&&... args)
    {
        ftxui::Color color;
        std::string_view prefix;
        const std::string message = std::format(formatString, std::forward<Args>(args)...);

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

    inline void PrintPretty(MessageSeverity severity, const std::string_view& message)
    {
        PrintPretty(severity, "{}", message);
    }
} // namespace ggpkg::Utils