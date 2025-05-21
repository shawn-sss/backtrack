#ifndef SETTINGSDIALOGSTYLING_H
#define SETTINGSDIALOGSTYLING_H

// Qt includes
#include <QLabel>
#include <QFont>

namespace SettingsDialogStyling {

// Style for Save button when in cooldown state
inline constexpr auto COOLDOWN_BUTTON_STYLE = R"(
    QPushButton#CooldownButton {
        background-color: #cccccc;
        color: #333;
        font-weight: bold;
        border: 1px solid #999;
    }
)";

// Style for the Clear App Data button
inline constexpr auto k_CLEAR_APP_BUTTON_STYLE = R"(
    background-color: #e74c3c;
    color: white;
    border: none;
    padding: 10px;
    border-radius: 6px;
    font-weight: bold;
)";

// Style for the Reset Backup Archive button
inline constexpr auto k_RESET_BACKUP_BUTTON_STYLE = R"(
    background-color: #f39c12;
    color: white;
    border: none;
    padding: 10px;
    border-radius: 6px;
    font-weight: bold;
)";

// Helper to create bold labels
inline QLabel* createBoldLabel(const QString& text, QWidget* parent = nullptr) {
    auto* label = new QLabel(text, parent);
    QFont font = label->font();
    font.setBold(true);
    label->setFont(font);
    return label;
}

// Helper to create gray, small, word-wrapped labels
inline QLabel* createGraySmallLabel(const QString& text, QWidget* parent = nullptr) {
    auto* label = new QLabel(text, parent);
    label->setStyleSheet("color: gray; font-size: 11px;");
    label->setWordWrap(true);
    return label;
}

} // namespace SettingsDialogStyling

#endif // SETTINGSDIALOGSTYLING_H
