#ifndef PROMPTDIALOGCONSTANTS_H
#define PROMPTDIALOGCONSTANTS_H

namespace PromptDialogConstants {

// Dialog dimensions (minimums; actual size adapts to content/screen)
constexpr int k_DIALOG_MIN_WIDTH   = 460;
constexpr int k_DIALOG_MIN_HEIGHT  = 160;

// Preferred base width for wrapping
constexpr int k_DIALOG_BASE_WIDTH  = 560;  // tweak to taste

// Icon configuration
constexpr int k_ICON_SIZE   = 48;
constexpr int k_ICON_MARGIN = 8;

// Layout spacing
constexpr int k_TEXT_SPACING    = 4;
constexpr int k_SECTION_SPACING = 8;
constexpr int k_OUTER_MARGIN    = 10;

} // namespace PromptDialogConstants

#endif // PROMPTDIALOGCONSTANTS_H
