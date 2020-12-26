#ifndef DIRMONITOR_H
#define DIRMONITOR_H

#include <QString>
#include <QDir>

class DirMonitor
{
public:
    explicit DirMonitor(const QDir &dir)
        : monitoringDir_(dir) {};
    explicit DirMonitor(const QString &dirPath)
        : monitoringDir_(dirPath) {};

private:
    QDir monitoringDir_;
};

#endif // DIRMONITOR_H
