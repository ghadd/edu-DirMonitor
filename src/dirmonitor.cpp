#include "dirmonitor.h"

void DirMonitor::validatePath() {
  if (!monitoringDir_.exists() || monitoringDir_.absolutePath() == "") {
    throw PathError();
  }
}

QPair<QVector<FileInfo>, quint64> DirMonitor::applyMonitor() {
  QVector<FileInfo> infos;

  QDirIterator it(monitoringDir_.path(), QDir::Files,
                  QDirIterator::Subdirectories);
  QStringList filenames;

  while (it.hasNext())
    filenames << it.next();

  for (auto &filename : filenames) {
    QFileInfo finfo(filename);
    infos.append({finfo.absoluteFilePath(), finfo.birthTime(), finfo.size()});
  }

  quint64 totalFilesSize = std::accumulate(
      infos.begin(), infos.end(), 0LL, [](quint64 init, const FileInfo &finfo) {
        return init + finfo.fileSize;
      });

  return qMakePair(infos, totalFilesSize);
}
