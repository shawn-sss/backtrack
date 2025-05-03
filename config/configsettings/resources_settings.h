#ifndef RESOURCES_SETTINGS_H
#define RESOURCES_SETTINGS_H

// Application icon resources
namespace Resources::Application {
constexpr auto k_ICON_PATH = ":/resources/app_icon.png";
}

// Toolbar icon resources
namespace Resources::Toolbar {
constexpr auto k_SETTINGS_ICON_PATH = ":/resources/icons/settings.svg";
constexpr auto k_HELP_ICON_PATH    = ":/resources/icons/help.svg";
constexpr auto k_ABOUT_ICON_PATH   = ":/resources/icons/about.svg";
constexpr auto k_EXIT_ICON_PATH    = ":/resources/icons/exit.svg";
}

// Theme stylesheet resources
namespace Resources::Themes {
constexpr auto k_DARK_THEME_PATH  = ":/resources/themes/dark.qss";
constexpr auto k_LIGHT_THEME_PATH = ":/resources/themes/light.qss";
}

#endif // RESOURCES_SETTINGS_H
