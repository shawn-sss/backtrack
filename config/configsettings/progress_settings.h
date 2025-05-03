#ifndef PROGRESS_SETTINGS_H
#define PROGRESS_SETTINGS_H

// Progress bar behavior settings
namespace ProgressSettings {
constexpr auto k_PROGRESS_BAR_INITIAL_MESSAGE = "Waiting for backup activity...";
constexpr auto k_PROGRESS_BAR_COMPLETION_MESSAGE = "Operation has completed!";
constexpr bool k_PROGRESS_BAR_DEFAULT_VISIBILITY = false;
constexpr bool k_PROGRESS_BAR_TEXT_VISIBLE = true;
constexpr int k_PROGRESS_BAR_MIN_VALUE = 0;
constexpr int k_PROGRESS_BAR_MAX_VALUE = 100;
constexpr int k_PROGRESS_BAR_DEFAULT_VALUE = k_PROGRESS_BAR_MIN_VALUE;
constexpr int k_PROGRESS_BAR_DELAY_MS = 1000;
}

// Progress UI appearance settings
namespace ProgressUI {
constexpr int k_STATUS_LIGHT_SIZE = 8;
constexpr int k_PROGRESS_BAR_HEIGHT = 20;
}

#endif // PROGRESS_SETTINGS_H
