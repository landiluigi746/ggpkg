#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <print>

using ftxui::vbox, ftxui::hbox, ftxui::text, ftxui::border, ftxui::flex, ftxui::gauge, ftxui::color,
    ftxui::Color;

int main()
{
    auto document = vbox({
        hbox({
            text("one") | border,
            text("two") | border | flex,
            text("three") | border | flex,
        }),

        gauge(0.25) | color(Color::Red),
        gauge(0.50) | color(Color::White),
        gauge(0.75) | color(Color::Blue),
    });

    auto screen = ftxui::Screen::Create(ftxui::Dimension::Fit(document));
    ftxui::Render(screen, document);

    std::println("{}\n", screen.ToString());

    return 0;
}
