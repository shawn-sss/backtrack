#ifndef INTERFACE_CONFIG_H
#define INTERFACE_CONFIG_H

namespace UI::Resources {
constexpr auto k_ICON_PATH = ":/resources/app_icon.png";
}

namespace UI::TreeView {
constexpr auto k_STAGING_COLUMN_NAME = "Name";
constexpr int k_START_HIDDEN_COLUMN  = 1;
constexpr int k_DEFAULT_COLUMN_COUNT = 4;
}

namespace UI::Progress {
constexpr auto k_PROGRESS_BAR_INITIAL_MESSAGE    = "Waiting for backup activity...";
constexpr auto k_PROGRESS_BAR_COMPLETION_MESSAGE = "Operation has completed!";
constexpr bool k_PROGRESS_BAR_DEFAULT_VISIBILITY = false;
constexpr bool k_PROGRESS_BAR_TEXT_VISIBLE       = true;
constexpr int k_PROGRESS_BAR_MIN_VALUE           = 0;
constexpr int k_PROGRESS_BAR_MAX_VALUE           = 100;
constexpr int k_PROGRESS_BAR_DEFAULT_VALUE       = k_PROGRESS_BAR_MIN_VALUE;
constexpr int k_PROGRESS_BAR_DELAY_MS            = 1000;
}

namespace UI::ProgressDetails {
constexpr int k_STATUS_LIGHT_SIZE   = 8;
constexpr int k_PROGRESS_BAR_HEIGHT = 20;
}

#endif // INTERFACE_CONFIG_H
