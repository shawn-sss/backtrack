#ifndef FORMATTINGUTILS_H
#define FORMATTINGUTILS_H

// Qt includes
#include <QDateTime>
#include <QString>

namespace Shared::Formatting {

// Format size in bytes to human-readable string
[[nodiscard]] QString formatSize(qint64 size);

// Format duration in milliseconds to human-readable string
[[nodiscard]] QString formatDuration(qint64 milliseconds);

// Format timestamp with custom string format
[[nodiscard]] QString formatTimestamp(const QDateTime& datetime, const QString& format);

// Format timestamp with Qt::DateFormat enum
[[nodiscard]] QString formatTimestamp(const QDateTime& datetime, Qt::DateFormat format);

} // namespace Shared::Formatting

#endif
