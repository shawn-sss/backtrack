#ifndef TOOLBARSERVICEMANAGERCONSTANTS_H
#define TOOLBARSERVICEMANAGERCONSTANTS_H

// Icon paths for toolbar service actions
namespace Resources::ToolbarService {
inline constexpr auto k_SETTINGS_ICON_PATH = ":/resources/icons/settings.svg";
inline constexpr auto k_HELP_ICON_PATH     = ":/resources/icons/help.svg";
inline constexpr auto k_ABOUT_ICON_PATH    = ":/resources/icons/about.svg";
inline constexpr auto k_EXIT_ICON_PATH     = ":/resources/icons/exit.svg";
}

namespace Labels::Toolbar {
inline constexpr auto k_SETTINGS = "Settings";
inline constexpr auto k_HELP     = "Help";
inline constexpr auto k_ABOUT    = "About";
inline constexpr auto k_EXIT     = "Exit";
}

#endif // TOOLBARSERVICEMANAGERCONSTANTS_H
