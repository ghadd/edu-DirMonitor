#include "dirmonitor.h"

Redis DirMonitor::redis = Redis("tcp://127.0.0.1:6379");

void DirMonitor::validatePath() const {
  if (!monitoringDir_.exists() || monitoringDir_.absolutePath() == "") {
    throw PathError();
  }
}

void DirMonitor::saveExpiring() const {
  std::string key = monitoringDir_.absolutePath().toStdString();
  std::string value = jsonify(lastResult_).dump();

  redis.set(key, value);
  redis.expire(key, 5s);
}

nlohmann::json
DirMonitor::jsonify(const QPair<QVector<FileInfo>, quint64> &obj) {
  json jsonObj = {{"files", {}}, {"total", obj.second}};

  for (auto &finfo : obj.first) {
    jsonObj["files"].push_back(finfo.to_json());
  }

  return jsonObj;
}

void DirMonitor::loadCachedResult() {
  std::string key = monitoringDir_.absolutePath().toStdString();
  auto result = redis.get(key);
  if (!result)
    throw std::runtime_error("Non existing value");

  std::string sResult = *result;
  json jsonObj = json::parse(sResult);

  QVector<FileInfo> infos;
  auto infosJson = jsonObj["files"];
  for (auto &infoJson : infosJson) {
    infos.append(FileInfo::from_json(infoJson));
  }

  lastResult_ = qMakePair(infos, jsonObj["total"]);
}

QPair<QVector<FileInfo>, quint64> DirMonitor::applyMonitor() {
  try {
    loadCachedResult();
  } catch (const std::runtime_error &) {
    QVector<FileInfo> infos;

    QDirIterator it(monitoringDir_.path(), QDir::Files,
                    QDirIterator::Subdirectories);
    QStringList filenames;

    while (it.hasNext())
      filenames << it.next();

    for (auto &filename : filenames) {
      QFileInfo finfo(filename);
      infos.append(
          {finfo.absoluteFilePath(), finfo.metadataChangeTime(), finfo.size()});
    }

    quint64 totalFilesSize =
        std::accumulate(infos.begin(), infos.end(), 0LL,
                        [](quint64 init, const FileInfo &finfo) {
                          return init + finfo.fileSize;
                        });

    lastResult_ = qMakePair(infos, totalFilesSize);
    saveExpiring();
  }
  return lastResult_;
}
