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
        auto availablePackages = GetAvailablePackages(packages, packageManager);

        std::string searchQuery;
        std::vector<std::string> packageNames;
        std::vector<std::string> packageProviderNames;
        int packageSelected = 0;

        auto searchPackages = [&availablePackages, &packageNames, &packageProviderNames,
                               &packageSelected, &searchQuery] {
            packageNames.clear();
            packageProviderNames.clear();
            packageSelected = 0;

            for (const auto& [name, providerName] : availablePackages)
            {
                if (searchQuery.empty() || name.contains(searchQuery))
                {
                    packageNames.emplace_back(name);
                    packageProviderNames.emplace_back(providerName);
                }
            }
        };

        searchPackages();

        auto packageNamesLayout = ftxui::Menu(&packageNames, &packageSelected);
        auto packageProviderNamesLayout = ftxui::Menu(&packageProviderNames, &packageSelected);

        auto searchInput = ftxui::Input(ftxui::InputOption{
            .content = &searchQuery,
            .placeholder = "Search packages by name",
            .multiline = false,
            .on_enter = searchPackages,
        });

        auto screen = ftxui::ScreenInteractive::Fullscreen();
        Utils::DisableCursor(screen);

        auto layout = ftxui::Container::Vertical({
            searchInput,
            packageNamesLayout,
            packageProviderNamesLayout,
        });

        // clang-format off
        auto layoutRenderer = ftxui::Renderer(layout, [&] {
            return ftxui::hbox({
                packageNamesLayout->Render(),
                ftxui::separator(),
                packageProviderNamesLayout->Render(),
            }) | ftxui::vscroll_indicator | ftxui::frame | ftxui::size(ftxui::HEIGHT, ftxui::LESS_THAN, screen.dimy() / 2);
        });

        auto renderer = ftxui::Renderer(layoutRenderer, [&] {
            auto renderedNames = packageNamesLayout->Render();
            renderedNames->ComputeRequirement();

            std::string namesLabel = "Packages";
            int minLabelWidth = std::max((int)namesLabel.length(), renderedNames->requirement().min_x);

            return ftxui::vbox({
                Utils::Banner(),
                ftxui::separator(),
                searchInput->Render(),
                ftxui::separator(),
                ftxui::hbox({
                    ftxui::text("q - Quit | Enter - Search"),
                    ftxui::separator(),
                    ftxui::text(std::format("Packages found: {}", packageNames.size())),
                }),
                ftxui::separator(),
                ftxui::hbox({
                    ftxui::text(namesLabel) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, minLabelWidth),
                    ftxui::separator(),
                    ftxui::text(std::format("Name on {}", packageManager.cmd)),
                }),
                ftxui::separator(),
                (!packageNames.empty() 
                ? ftxui::vbox({
                    layoutRenderer->Render(),
                })
                : ftxui::emptyElement()
                ),
                
            }) | ftxui::borderRounded | ftxui::center;
        });
        // clang-format on

        renderer |= ftxui::CatchEvent([&screen](const ftxui::Event& event) {
            if (event == ftxui::Event::Character('q'))
            {
                screen.Exit();
                return true;
            }

            return false;
        });

        screen.Loop(renderer);
    }

    static void DefaultList(const Packages& packages, const PackageManagerInfo& packageManager)
    {
        auto availablePackages = GetAvailablePackages(packages, packageManager);

        ftxui::Elements packageNames;
        ftxui::Elements packageProviderNames;

        for (const auto& [name, providerName] : availablePackages)
        {
            packageNames.emplace_back(ftxui::text(name));
            packageProviderNames.emplace_back(ftxui::text(providerName));
        }

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
                }),
                ftxui::separator(),
                ftxui::text(std::format("Packages found: {}", packageNames.size())),
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
