#ifndef BACKUP_STYLING_H
#define BACKUP_STYLING_H

// General button styling
namespace Styles {
namespace BackupViewContainer {
constexpr auto STYLE = R"(
    #BackupViewContainer {
        border-radius: 10px;
        padding: 10px;
        border: 1px solid; /* Let theme override color */
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
namespace GeneralButton {
constexpr auto STYLE =
    "QPushButton { background: #444; color: white; border-radius: 5px; padding: 5px; }"
    "QPushButton:hover { background: #666; }"
    "QPushButton:pressed { background: #888; }";
}
}

#endif // BACKUP_STYLING_H
