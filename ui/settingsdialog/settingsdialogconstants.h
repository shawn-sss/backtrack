#ifndef SETTINGSDIALOGCONSTANTS_H
#define SETTINGSDIALOGCONSTANTS_H

// Qt includes
#include <QString>

namespace SettingsDialogConstants {

// UI labels
inline const QString k_LABEL_BACKUP_PREFIX = QStringLiteral("Backup Prefix:");
inline const QString k_LABEL_BACKUP_PREFIX_TOOLTIP = QStringLiteral("Modify the prefix used when creating backups.");
inline const QString k_LABEL_THEME = QStringLiteral("Theme:");
inline const QString k_LABEL_THEME_SYSTEM_DEFAULT = QStringLiteral("System Default");
inline const QString k_LABEL_THEME_LIGHT_MODE = QStringLiteral("Light Mode");
inline const QString k_LABEL_THEME_DARK_MODE = QStringLiteral("Dark Mode");

// Save button labels and text width reference
inline const QString k_BUTTON_SAVE_TEXT = QStringLiteral("Save");
inline const QString k_BUTTON_SAVED_TEXT = QStringLiteral("✔️ Saved");
inline const QString k_BUTTON_SAVE_WIDTH_TEXT = QStringLiteral("✔️ Saved");

// Timing constants
constexpr int k_SAVE_FEEDBACK_COOLDOWN_MS = 3000;

// Layout and sizing constants
constexpr int k_CATEGORY_LIST_WIDTH = 150;
constexpr int k_DIALOG_MIN_WIDTH = 600;
constexpr int k_DIALOG_MIN_HEIGHT = 400;
constexpr int k_MAIN_MARGIN = 10;
constexpr int k_MAIN_SPACING = 10;

} // namespace SettingsDialogConstants

#endif // SETTINGSDIALOGCONSTANTS_H
