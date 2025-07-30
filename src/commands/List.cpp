#include "commands/Commands.hpp"
#include "PackageManager.hpp"
#include "Package.hpp"

#include <print>
#include <cstdlib>
#include <ftxui/screen/screen.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>

namespace ggpkg::Commands
{
    static void InteractiveList(const Packages& packages, const PackageManagerInfo& packageManager)
    {
        auto packageNames = ftxui::Container::Vertical({});
        auto packageProviderNames = ftxui::Container::Vertical({});

        const auto style = ftxui::ButtonOption::Ascii();

        for (const auto& [name, providers] : packages)
        {
            if (!providers.contains(packageManager.cmd))
                continue;

            packageNames->Add(ftxui::Button(name, [] {}, style));
            packageProviderNames->Add(ftxui::Button(providers.at(packageManager.cmd), [] {}, style));
        }

        auto screen = ftxui::ScreenInteractive::Fullscreen();

        auto layout = ftxui::Container::Vertical({
            packageNames,
            packageProviderNames,
        });

        // clang-format off
        auto layoutRenderer = ftxui::Renderer(layout, [&]{
            return ftxui::hbox({
                packageNames->Render(),
                ftxui::separator(),
                packageProviderNames->Render()
            }) | ftxui::vscroll_indicator | ftxui::frame;
        });

        auto renderer = ftxui::Renderer(layoutRenderer, [&] {
            return ftxui::vbox({
                ftxui::hbox({
                    ftxui::text("Packages"),
                    ftxui::separator(),
                    ftxui::text(std::format("Name on {}", packageManager.cmd)),
                }),
                ftxui::separator(),
                layoutRenderer->Render()
            }) | ftxui::borderRounded | ftxui::center;
        });
        // clang-format on

        screen.Loop(renderer);
    }

    static void DefaultList(const Packages& packages, const PackageManagerInfo& packageManager)
    {
        ftxui::Elements packageNames;
        ftxui::Elements packageProviderNames;

        packageNames.reserve(packages.size());
        packageProviderNames.reserve(packages.size());

        for (const auto& [name, providers] : packages)
        {
            if (!providers.contains(packageManager.cmd))
                continue;

            packageNames.emplace_back(ftxui::text(name));
            packageProviderNames.emplace_back(ftxui::text(providers.at(packageManager.cmd)));
        }

        packageNames.shrink_to_fit();
        packageProviderNames.shrink_to_fit();

        // clang-format off
        auto document = ftxui::vbox({
            ftxui::hbox({
                ftxui::vbox({
                    ftxui::text("Packages"),
                    ftxui::separator(),
                    ftxui::vbox(packageNames),
                }),
                ftxui::separator(),
                ftxui::vbox({
                    ftxui::text(std::format("Name on {}", packageManager.cmd)),
                    ftxui::separator(),
                    ftxui::vbox(packageProviderNames),
                })
            }) | ftxui::borderRounded,
        });
        // clang-format on

        auto screen = ftxui::Screen::Create(ftxui::Dimension::Fit(document));
        ftxui::Render(screen, document);

        std::println("{}", screen.ToString());
    }

    void List(bool interactive)
    {
        auto packageManager = ggpkg::GetPackageManager();
        auto packages = ggpkg::GetPackages();

        if (!packageManager)
        {
            std::println("ERROR: {}", packageManager.error());
            std::exit(EXIT_FAILURE);
        }

        if (!packages)
        {
            std::println("ERROR: {}", packages.error());
            std::exit(EXIT_FAILURE);
        }

        if (interactive)
            InteractiveList(packages.value(), packageManager.value());
        else
            DefaultList(packages.value(), packageManager.value());

        std::exit(EXIT_SUCCESS);
    }
} // namespace ggpkg::Commands
