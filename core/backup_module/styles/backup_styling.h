#ifndef BACKUP_STYLING_H
#define BACKUP_STYLING_H

// Styling rules for the backup view
namespace Styles {

// Styles for the main backup view container and its components
namespace BackupViewContainer {
constexpr auto STYLE = R"(
            #BackupViewContainer {
                border-radius: 10px;
                padding: 10px;
                border: 1px solid;
            }

            #DestinationTreeTitleLabel,
            #SourceTreeTitleLabel,
            #StagingListTitleLabel {
                font-weight: bold;
            }

            #DriveTreeView,
            #BackupStagingTreeView,
            #BackupDestinationView {
                border: 1px solid;
            }

            #MainToolBar QToolButton {
                border-radius: 16px;
                padding: 8px;
                margin: 4px;
                min-width: 40px;
                min-height: 40px;
                font-size: 14px;
                border: none;
            }

            #ChangeBackupDestinationButton,
            #DeleteBackupButton,
            #AddToBackupButton,
            #CreateBackupButton,
            #RemoveFromBackupButton {
                border: 1px solid;
                border-radius: 6px;
                padding: 6px 12px;
            }
        )";
}

// Independent styling for the test container
namespace DashboardContainer {
constexpr auto STYLE = R"(
            #DashboardContainer {
                border: 1px solid black;
                border-radius: 6px;
                padding: 10px;
                background: transparent;
            }
        )";
}

// Styling for the dashboard label
namespace DashboardLabel {
constexpr auto STYLE = R"(
            #DashboardLabel {
                font-size: 18px;
                font-weight: bold;
                padding: 0px;
            }
        )";
}

// Generic push button styling
namespace GeneralButton {
constexpr auto STYLE =
    "QPushButton { background: #444; color: white; border-radius: 5px; padding: 5px; }"
    "QPushButton:hover { background: #666; }"
    "QPushButton:pressed { background: #888; }";
}

}

#endif // BACKUP_STYLING_H
