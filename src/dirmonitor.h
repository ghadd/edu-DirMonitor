#ifndef DIRMONITOR_H
#define DIRMONITOR_H

#include <QDateTime>
#include <QDir>
#include <QDirIterator>
#include <QException>
#include <QFile>
#include <QFileInfo>
#include <QString>
#include <QStringList>
#include <QVector>

#include "sw/redis++/redis++.h"
#include <nlohmann/json.hpp>

using namespace std::chrono_literals;
using namespace sw::redis;
using nlohmann::json;

struct FileInfo {
  QString filename;
  QDateTime creationDate;
  qint64 fileSize;

  json to_json() const {
      json jsonObj = {
          { "filename", filename.toStdString() },
          { "creation_date", creationDate.toString(Qt::ISODate).toStdString() },
          { "file_size", fileSize }
      };
      return jsonObj;
  }

  static FileInfo from_json(const json &jsonObj) {
      FileInfo res;

      std::string filename, creationDate;
      jsonObj["filename"].get_to<std::string>(filename),
      jsonObj["creation_date"].get_to<std::string>(creationDate),
      jsonObj["file_size"].get_to<qint64>(res.fileSize);

      res.filename = QString::fromStdString(filename);
      res.creationDate = QDateTime::fromString(QString::fromStdString(creationDate), Qt::ISODate);

      return res;
  }
};

class DirMonitor {
public:
  static Redis redis;

  explicit DirMonitor(const QDir &dir) : monitoringDir_(dir){};
  explicit DirMonitor(const QString &dirPath) : monitoringDir_(QDir(dirPath)){};

  QPair<QVector<FileInfo>, quint64> applyMonitor();

  void validatePath() const;

  void saveExpiring() const;

  static json jsonify(const QPair<QVector<FileInfo>, quint64> &obj);

  void loadCachedResult();

private:
  QDir monitoringDir_;
  QPair<QVector<FileInfo>, quint64> lastResult_;
};

class PathError : public QException {
public:
  void raise() const override { throw *this; }
};

#endif // DIRMONITOR_H
