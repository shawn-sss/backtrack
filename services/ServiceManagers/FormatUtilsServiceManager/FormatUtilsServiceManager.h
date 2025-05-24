#ifndef FORMATTINGUTILS_H
#define FORMATTINGUTILS_H

// Qt includes
#include <QDateTime>
#include <QString>

namespace Shared::Formatting {

// Converts size in bytes to human-readable string with units
QString formatSize(qint64 size);

// Converts duration in milliseconds to a readable string
QString formatDuration(qint64 milliseconds);

// Formats QDateTime using custom format string
QString formatTimestamp(const QDateTime& datetime, const QString& format);

// Formats QDateTime using Qt::DateFormat enum
QString formatTimestamp(const QDateTime& datetime, Qt::DateFormat format);

} // namespace Shared::Formatting

#endif // FORMATTINGUTILS_H
