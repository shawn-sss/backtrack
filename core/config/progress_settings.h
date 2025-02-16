#ifndef PROGRESS_SETTINGS_H
#define PROGRESS_SETTINGS_H

namespace ProgressConfig {
// Progress Bar Settings
constexpr int STATUS_LIGHT_SIZE = 8;
constexpr int PROGRESS_BAR_HEIGHT = 20;
constexpr int PROGRESS_BAR_MAX_VALUE = 100;
constexpr int PROGRESS_BAR_MIN_VALUE = 0;
constexpr bool PROGRESS_BAR_TEXT_VISIBLE = true;
constexpr bool PROGRESS_BAR_DEFAULT_VISIBILITY = false;
constexpr int PROGRESS_BAR_DEFAULT_VALUE = PROGRESS_BAR_MIN_VALUE;
constexpr int PROGRESS_BAR_DELAY_MS = 1000;
}

#endif // PROGRESS_SETTINGS_H
