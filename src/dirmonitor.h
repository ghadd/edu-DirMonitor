#ifndef DIRMONITOR_H
#define DIRMONITOR_H

#include <QDateTime>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QFileInfo>
#include <QString>
#include <QVector>
#include <QStringList>
#include <QException>

struct FileInfo {
  QString filename;
  QDateTime creationDate;
  qint64 fileSize;
};

class DirMonitor {
public:
    explicit DirMonitor(const QDir &dir)
        : monitoringDir_(dir) {};
    explicit DirMonitor(const QString &dirPath)
        : monitoringDir_(QDir(dirPath)) {};

  QPair<QVector<FileInfo>, quint64> applyMonitor();

  void validatePath();

private:
  QDir monitoringDir_;
};

class PathError : public QException {
public:
    void raise() const override { throw *this; }
};

#endif // DIRMONITOR_H
